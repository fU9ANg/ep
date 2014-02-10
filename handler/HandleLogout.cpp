#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"
#include "Single.h"
#include "global_functions.h"
#include "../message/proto/protocol.pb.h"
#include "../netdef.h"

#include "../content/epManager.h"
#include "../netdef.h"
#include "../DestroyClassroomTask.h"

void CHandleMessage::handleLogout (Buf* p)
{
        /**
         * @brief 客户端断线时，服务器会根据心跳包主动发出该协议。
         *        1，如果该用户在游离状态，则将其对象析构掉。
         *        2，如果该用户在上课状态，则将其状态设置为离线，不析构其对象，并广播给全教室。
         */

#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_Logout\n";
#endif
        epUser* pUser = const_cast<epUser*>(EPMANAGER->getUserByFd(p->getfd()));
        if (NULL != pUser) { // 该退出的用户处于游离状态。
                printf("[DEBUG] %s : NULL != pUser\n", __func__);
                EPMANAGER->removeUserByFd(p->getfd());
                delete pUser;
                pUser = NULL;
                SINGLE->bufpool.free(p);
                return;
        }

        // 该客户端正处于上课状态。
        sLogout tmp;
        pUser = const_cast<epUser*>(EPMANAGER->getUserByFdFromClassroom(p->getfd()));
        if (NULL == pUser) {
                printf("[DEBUG] %s : not in classroom\n", __func__);
                SINGLE->bufpool.free(p);
                return;
        }

        epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
        if (NULL == pClassroom) {
                printf("[DEBUG] %s : NULL == pClassroom\n", __func__);
                SINGLE->bufpool.free(p);
                return;
        }

        enum LoginType type = pUser->getType();
        tmp.set_login_type(type);
        tmp.set_id(pUser->id_);
        Buf* pBuf = packet(ST_Logout, tmp, p->getfd());
        CHECK_BUF(pBuf, p);

        Buf* p1 = NULL;
        Buf* p2 = NULL;

        // epGroup* pGroup = NULL;
        switch (type) {
        case LT_STUDENT :
                CLONE_BUF(p1, pBuf);
                pClassroom->sendtoAll(p1, false); // 不发送给自己。:

                /*
                pGroup = const_cast<epGroup*>(pClassroom->getGroupByFd(p->getfd()));
                if (NULL != pGroup) {
                        pGroup->removeStudentByFd(p->getfd());
                }
                */
                break;
        case LT_TEACHER :
                CLONE_BUF(p1, pBuf);
                pClassroom->sendtoAllClass(p1); // 不发送给自己。

                CLONE_BUF(p2, pBuf);
                pClassroom->sendtoWhiteBoard(p2);

                DestroyClassroomTask::desMap_.insert(std::make_pair<int, time_t>(pClassroom->id_, time(NULL)));
                break;
        case LT_WHITEBOARD :
                CLONE_BUF(p1, pBuf);
                pClassroom->sendtoAllClass(p1);

                CLONE_BUF(p2, pBuf);
                pClassroom->sendtoTeacher(p2);
                break;
        default :
                break;
        }

        pUser->userStatus_ = US_OFFLINE;

#ifdef __DEBUG_DUMP__
        EPMANAGER->dumpClassroom();
#endif

        SINGLE->bufpool.free(p);
        return;
}
