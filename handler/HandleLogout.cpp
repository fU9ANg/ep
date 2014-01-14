#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"
#include "Single.h"
#include "global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"

void CHandleMessage::handleLogout (Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_Logout\n";
#endif

        epUser* pUser = const_cast<epUser*>(EPMANAGER->getUserByFd(p->getfd()));
        if (NULL != pUser) { // 该退出的用户处于游离状态。
                EPMANAGER->removeUserByFd(p->getfd());
                delete pUser;
                SINGLE->bufpool.free(p);
                return;
        }

        sLogout tmp;
        pUser = const_cast<epUser*>(EPMANAGER->getUserByFdFromClassroom(p->getfd()));
        if (NULL != pUser) { // 该客户端正处于上课状态。
                printf ("============== send logout ===============\n");
                epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
                if (NULL != pClassroom) {
                        enum LoginType type =  pUser->getType();
                        tmp.set_login_type(type);
                        tmp.set_num(pUser->getNum());
                        if (LT_STUDENT == type) {
                                Buf* p1 = packet(ST_Logout, tmp, p->getfd());
                                if (NULL != p1) {
                                        pClassroom->sendtoAllStudent(p1, false); // 不发送给自己。
                                }

                                Buf* p2 = packet(ST_Logout, tmp, p->getfd());
                                if (NULL != p2) {
                                pClassroom->sendtoTeacher(p2);
                                }

                                Buf* p3 = packet(ST_Logout, tmp, p->getfd());
                                if (NULL != p3) {
                                pClassroom->sendtoWhiteBoard(p3);
                                }
                        }

                        if (LT_TEACHER == type) {
                                Buf* p1 = packet(ST_Logout, tmp, p->getfd());
                                if (NULL != p1) {
                                pClassroom->sendtoAllStudent(p1); // 不发送给自己。
                                }

                                Buf* p2 = packet(ST_Logout, tmp, p->getfd());
                                if (NULL != p2) {
                                pClassroom->sendtoWhiteBoard(p2);
                                }

#ifdef __DEBUG_DUMP__
                                EPMANAGER->dumpClassroom();
#endif
                                // 销毁电子教室。
                                EPMANAGER->deleteClassroomById(pClassroom->getId());
                        }

                        if (LT_WHITEBOARD == type) {
                                Buf* p1 = packet(ST_Logout, tmp, p->getfd());
                                if (NULL != p1) {
                                pClassroom->sendtoAllStudent(p1);
                                }

                                Buf* p2 = packet(ST_Logout, tmp, p->getfd());
                                if (NULL != p2) {
                                pClassroom->sendtoTeacher(p2);
                                }
                        }
                        pClassroom->deleteUserByFd(p->getfd());
                }

#ifdef __DEBUG_DUMP__
                EPMANAGER->dumpClassroom();
#endif
        }
}
