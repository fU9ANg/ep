/*
 * HandleClassRoom.cpp
 */

#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"
#include "../netdef.h" // for __DEBUG__

#include "../content/epManager.h"
#include "../content/epUser.h"
#include "../content/epStudent.h"
#include "../content/epTeacher.h"
#include "../content/epParents.h"
#include "../content/epHeadmaster.h"
#include "../content/epWhiteBoard.h"
#include "../DestroyClassroomTask.h"


void CHandleMessage::handleLogin (Buf* p) {
        /**
         * @brief 用户登录处理函数。
         *        1. 用户发送用户类型，帐号，密码请求登录。
         *        2. 创建相应类型的登录对象。
         *        3. 使用数据库初始化对象。
         *        4. 返回登录成功与否。
         *        5. 如果登录成功，则将对象加入到游离列表中。
         */
        bool result = false;
        bool enter_classroom = false;

        cLogin clogin;
        UNPACKET(p, clogin);

#ifdef __DEBUG__
        printf("clogin.type    = %d\n", clogin.type());
        printf("clogin.account = %s\n", clogin.account().c_str());
        printf("clogin.passwd  = %s\n", clogin.passwd().c_str());
#endif

        sLogin slogin;
        Buf* p_buf = NULL;
        epUser* pUser = const_cast<epUser*>(EPMANAGER->getUserByAccount(clogin.account(), (enum LoginType)clogin.type()));
        if (NULL == pUser) { // 该客户不处于游离状态。
                pUser = const_cast<epUser*>(EPMANAGER->getUserByAccountFromClassroom(clogin.account(), (enum LoginType)clogin.type()));
                if (NULL == pUser) { // 在教室里找不到。
                        switch (clogin.type()) {
                        case LT_STUDENT :
                                pUser = new epStudent();
                                break;
                        case LT_HEADMASTER :
                                pUser = new epHeadmaster();
                                break;
                        case LT_TEACHER :
                                pUser = new epTeacher();
                                break;
                        case LT_PARENTS :
                                // pUser = new epParents();
                                break;
                        case LT_WHITEBOARD : // 如果通过账号，在教室里没有找到白板，则直接返回失败。
                                slogin.set_result(false);
                                slogin.set_enter_classroom(false);
                                slogin.set_login_type(LT_WHITEBOARD);

                                p_buf = packet(ST_Login, slogin, p->getfd());
                                CHECK_P(p_buf);
                                SINGLE->sendqueue.enqueue(p_buf);

                                RETURN(p);
                                break;
                        default :
                                break;
                        }

                        if (NULL != pUser) { // 登录类型是否超出规定范围。
                                result = pUser->init(clogin.account(), clogin.passwd(), p->getfd(), US_ONLINE);
                                if (!result) {
#ifdef __DEBUG__
                                        printf("[INFO] handleLogin : epUser init fault!\n");
#endif
                                        result = false;
                                        delete pUser;
                                        pUser = NULL;
                                } else {
                                        EPMANAGER->insertUser(p->getfd(), pUser);
                                        result = true;
                                }
                        }
                } else { // found
#ifdef __DEBUG__
                        printf("[DEBUG] CHandleMessage::handleLogin : in Classroom!\n");
#endif
                        epClassroom* pClassroom = NULL;
                        epClass*     p_class    = NULL;
                        epStudent*   p_student  = NULL;
                        std::map<int, time_t>::iterator it;

                        switch (pUser->userStatus_) {
                        case US_OFFLINE :
                                DEBUG_INFO;
                                result = true;
                                // pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
                                pClassroom = EPMANAGER->getClassroomByUserAccount(clogin.account(), (enum LoginType)clogin.type());
                                if (NULL != pClassroom) {
                                        DEBUG_INFO;
                                        switch (pUser->getType()) {
                                        case LT_TEACHER :
                                                it = DestroyClassroomTask::desMap_.find(pClassroom->id_);
                                                if (DestroyClassroomTask::desMap_.end() != it) {
                                                        DestroyClassroomTask::desMap_.erase(it);
                                                }
                                                break;
                                        case LT_STUDENT :
                                                p_student = dynamic_cast<epStudent*>(pUser);
                                                if (NULL != p_student) {
                                                        p_class = pClassroom->getClassById(p_student->classId_);
                                                        if (NULL != p_class) {
#ifdef __DEBUG__
                                                                printf("[DEBUG] CHandleMessage::handleLogin : NULl 1= p_class\n");
#endif
                                                                p_class->moveStudentFromListToMapByAccount(p->getfd(), std::string(clogin.account()));
                                                        }
                                                }
                                                break;
                                        case LT_WHITEBOARD :
                                        DEBUG_INFO;
                                                result = pUser->check(clogin.account(), clogin.passwd());
#ifdef __DEBUG__
                                                printf("[DEBUG] CHandleMessage::handleLogin : result = %s\n", result ? "true" : "false");
#endif
                                                break;
                                        default :
                                                break;
                                        }

                                        enter_classroom = true;
                                        pUser->userStatus_ = US_ONLINE;
                                        pUser->fd_ = p->getfd();
                                        DEBUG_INFO;
                                        if (dynamic_cast<epWhiteBoard*>(pUser))
                                                printf("[DEBUG] CHandleMessage::handleLogin : pUser->fd_ = %d\n", pClassroom->whiteboard_->fd_);

                                        sUpdateStudentStatus tmp;
                                        tmp.set_student_id(pUser->id_);
                                        tmp.set_us(US_ONLINE);
                                        tmp.set_login_type(pUser->getType());

                                        Buf* p_buf = packet(ST_UpdateStudentStatus, tmp, p->getfd());
                                        CHECK_P(p_buf);
                                        pClassroom->sendtoAll(p_buf);
                                }
                                break;
                        case US_ONLINE :
                                result = false;
                                break;
                        default :
                                result = false;
                                break;
                        }
                }
        } else {
#ifdef __DEBUG__
                printf("[DEBUG] CHandleMessage::handleLogin : NULL != pUser\n");
#endif
                result = false;
        }

#ifdef __DEBUG_DUMP__
        EPMANAGER->dumpUser();
#endif

        if (NULL != pUser) {
                slogin.set_login_type(pUser->getType());
                slogin.set_student_id(pUser->id_);
#ifdef __DEBUG__
                printf("[DEBUG] CHandleMessage::handleLogin : user_login_type = %d\n", pUser->getType());
                printf("[DEBUG] CHandleMessage::handleLogin : user_id         = %d\n", pUser->id_);
#endif
        } else {
                slogin.set_login_type(LT_USER);
                slogin.set_student_id(EPSTUDENT_INVALIED_STUDENT_ID);
        }

        if (result) {
#ifdef __DEBUG__
                printf("[DEBUG] CHandleMessage::handleLogin : result = true\n");
#endif
                slogin.set_result(result);
        }

        if (enter_classroom) slogin.set_enter_classroom(enter_classroom);

        Buf* pBuf = packet(ST_Login, slogin, p->getfd());
        CHECK_P(pBuf);
        SINGLE->sendqueue.enqueue(pBuf);

        RETURN(p);
}
