#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"

void CHandleMessage::handleUpdateDrawSet(Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_UpdateDrawSet\n";
#endif

        const epUser* pUser = EPMANAGER->getUserByFdFromClassroom(p->getfd());
        if (NULL == pUser) {
                printf("[DEBUG] CHandleMessage::handleUpdateDrawSet : NULL == pUser\n");
                SINGLE->bufpool.free(p);
                return;
        }

        epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
        if (NULL == pClassroom) {
                printf("[DEBUG] CHandleMessage::handleUpdateDrawSet : NULl == pClassroom\n");
                SINGLE->bufpool.free(p);
                return;
        }

        ((MSG_HEAD*)p->ptr())->cType = ST_UpdateDrawSet;

        Buf* pBuf = SINGLE->bufpool.malloc();
        CHECK_BUF(pBuf, p);
        Buf* p1 = NULL;
        CLONE_BUF(p1, pBuf);
        pClassroom->sendtoAllClass(p1, true);
        pClassroom->sendtoWhiteBoard(pBuf);

        SINGLE->bufpool.free(p);
        return;
}
