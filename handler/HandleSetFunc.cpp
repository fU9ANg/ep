#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../protocol.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../content/epUser.h"

void CHandleMessage::handleSetFunc (Buf* p) {
        /**
         * @brief 用户设置功能列表。
         *        1. 用户发送所选择的功能ID。
         *        2. 返回设置该功能成功与否。
         *        3. 如果该用户类型为学生，并选择了学校功能，则：
         *              1）如果该学生所在班级正在上课，则将学生由游离列表加入到上课的班级。
         *              2）将该学生上线状态通知该班其它上线学生，上课教师，所在教室的白板。
         *              3）将学生从游离列表中删除。
         */

#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_SetFunc\n";
#endif
        // TODO:

        cSetFunc setFunc;
        if (!unpacket(p, setFunc)) { // 解包失败。
#ifdef __DEBUG__
                printf("[DEBUG] %s : unpacket fail!\n", __func__);
#endif
                SINGLE->bufpool.free(p);
                return;
        }

        sSetFunc tmp;
        epUser* pUser = const_cast<epUser*>(EPMANAGER->getUserByFd(p->getfd()));
        tmp.set_result((NULL==pUser) ? false : pUser->setFuncType((enum FuncType)setFunc.func_type()));
        SINGLE->sendqueue.enqueue(packet(ST_SetFunc, tmp, p->getfd()));

        if (LT_STUDENT == pUser->getType() && FT_SCHOOL==setFunc.func_type()) { // 学生选择的是学校功能。
                epStudent* pStudent = dynamic_cast<epStudent*>(pUser);
                if (NULL != pStudent) {
                        epClass* pClass = EPMANAGER->getClassById(pStudent->getClassId());
                        if (NULL != pClass) { // 如果该班正在上课。
                                // 将该学生上线状态通知给该班其它学生及教师和白板。
                                sUpdateStudentStatus uss;
                                uss.set_student_id(pStudent->getId());

                                printf ("send setfunc to all student.\n");
                                Buf* pBuf1 = packet(ST_UpdateStudentStatus, uss, p->getfd());
                                if (NULL != pBuf1) {
                                        pClass->sendtoAllStudent(pBuf1);
                                }

                                epClassroom* pClassroom = EPMANAGER->getClassroomByClassId(pClass->getId());
                                if (NULL != pClassroom) {
                                        printf ("send setfunc to teacher.\n");
                                        Buf* pBuf2 = packet(ST_UpdateStudentStatus, uss, p->getfd());
                                        if (NULL != pBuf2) {
                                                pClassroom->sendtoTeacher(pBuf2);
                                        }
                                        Buf* pBuf3 = packet(ST_UpdateStudentStatus, uss, p->getfd());
                                        if (NULL != pBuf3) {
                                                pClassroom->sendtoWhiteBoard(pBuf3);
                                        }
                                }

                                // 将学生由游离状态加入该班级。
                                pClass->insertStudent(pStudent->getFd(), pStudent);
                                printf("[DEBUG] CHandleMessage::handleSetFunc : pStudent = %p\n", pStudent);

                                // 将该学生从游离列表移除。
                                EPMANAGER->removeUserByFd(p->getfd());
                                /*
                                   delete pUser;
                                   pUser = NULL;
                                   */
                        }
                }
        }

#ifdef __DEBUG__
        EPMANAGER->dumpClassroom();
#endif

        SINGLE->bufpool.free(p);
}
