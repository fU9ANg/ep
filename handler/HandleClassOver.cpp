#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"

void CHandleMessage::handleClassOver(Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_ClassOver\n";
#endif

        epUser* pUser = const_cast<epUser*>(EPMANAGER->getUserByFdFromClassroom(p->getfd()));
        if (NULL == pUser) {
                printf("[DEBUG] CHandleMessage::handleClassOver : NULL == pUser\n");
                SINGLE->bufpool.free(p);
                return;
        }

        const epTeacher* pTeacher = dynamic_cast<epTeacher*>(pUser);
        if (NULL == pTeacher) {
                printf("[DEBUG] CHandleMessage::handleClassOver : not teacher!\n");
                SINGLE->bufpool.free(p);
                return;
        }

        epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
        if (NULL == pClassroom) {
                printf("[DEBUG] CHandleMessage::handleClassOver : NULl == pClassroom\n");
                SINGLE->bufpool.free(p);
                return;
        }

        Buf* pBuf = packet(ST_ClassOver, p->getfd());
        CHECK_BUF(pBuf, p);
        Buf* p1 = NULL;
        CLONE_BUF(p1, pBuf);
        pClassroom->sendtoAllClass(p1);
        pClassroom->sendtoWhiteBoard(pBuf);

        delete pClassroom;
        pClassroom = NULL;

        SINGLE->bufpool.free(p);
        return;
}
