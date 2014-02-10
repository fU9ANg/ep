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
        if (!unpacket(p, clogin)) { // 解包失败。
#ifdef __DEBUG__
                printf("[DEBUG] %s : unpacket fail!\n", __func__);
#endif
                SINGLE->bufpool.free(p);
                return;
        }
#ifdef __DEBUG__
        printf("clogin.type    = %d\n", clogin.type());
        printf("clogin.account = %s\n", clogin.account().c_str());
        printf("clogin.passwd  = %s\n", clogin.passwd().c_str());
#endif

        /*
        epUser* pUser = const_cast<epUser*>(EPMANAGER->getUserByAccount(clogin.account()));
        if (NULL == pUser) { // 该客户不处于游离状态。
                pUser = getUserByFdFromClassroom(clogin.account());
        }
        // if ((NULL==pUser) ? TRUE : pUser->getType()!=clogin.type()) {
        */

        // epUser* pUser = const_cast<epUser*>(EPMANAGER->getUserByFd(p->getfd()));
        epUser* pUser = const_cast<epUser*>(EPMANAGER->getUserByAccount(clogin.account()));
        if (NULL == pUser) { // 该客户不处于游离状态。
                pUser = const_cast<epUser*>(EPMANAGER->getUserByAccountFromClassroom(clogin.account()));
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
                        case LT_WHITEBOARD :
                                pUser = new epWhiteBoard();
                                break;
                        default :
                                break;
                        }

                        if (NULL != pUser) { // 登录类型是否超出规定范围。
                                result = pUser->init(clogin.account(), clogin.passwd());
                                if (!result) {
                                        printf("[INFO] handleLogin : epUser init fault!\n");
                                        result = false;
                                        delete pUser;
                                        pUser = NULL;
                                } else {
                                        pUser->fd_         = p->getfd();
                                        pUser->funcType_   = FT_INVALID;
                                        pUser->userStatus_ = US_ONLINE;
                                        EPMANAGER->insertUser(p->getfd(), pUser);
                                        result = true;
                                }
                        }
                } else {
                        printf("[DEBUG] CHandleMessage::handleLogin : in Classroom!\n");
                        Buf* pBuf = NULL;
                        epClassroom* pClassroom = NULL;
                        switch (pUser->userStatus_) {
                        case US_ONLINE :
                                result = false;
                                break;
                        case US_OFFLINE :
                                result = true;
                                pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
                                if (NULL != pClassroom) {
                                        pBuf = packet(ST_UpdateGroupStatus, p->getfd());
                                        CHECK_BUF(pBuf, p);
                                        pClassroom->sendtoAll(pBuf);
                                        enter_classroom = true;
                                        pUser->userStatus_ = US_ONLINE;

                                        std::map<int, time_t>::iterator it = DestroyClassroomTask::desMap_.find(pClassroom->id_);
                                        if (DestroyClassroomTask::desMap_.end() != it) {
                                                DestroyClassroomTask::desMap_.erase(it);
                                        }
                                }
                                break;
                        default :
                                result = false;
                                break;
                        }
                }
        } else {
                printf("[DEBUG] CHandleMessage::handleLogin : NULL != pUser\n");
                result = false;
        }

#ifdef __DEBUG_DUMP__
        EPMANAGER->dumpUser();
#endif

        sLogin slogin;
        if (result) {
                slogin.set_result(result);
        }
        if (enter_classroom) {
                slogin.set_enter_classroom(enter_classroom);
        }
        if (NULL != dynamic_cast<epStudent*>(pUser)) {
                slogin.set_student_id(pUser->id_);
        }

        Buf* pBuf = packet(ST_Login, slogin, p->getfd());
        if (NULL != pBuf) {
                SINGLE->sendqueue.enqueue(pBuf);
        }
        SINGLE->bufpool.free(p);
        return;
}
