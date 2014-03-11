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
        CHECK_P(pUser);

        epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
        CHECK_P(pClassroom);

        ((MSG_HEAD*)p->ptr())->cType = ST_UpdateDrawSet;

        Buf* p1 = NULL;
        CLONE_BUF(p1, p);

        pClassroom->sendtoAllClass(p1, true);
        pClassroom->sendtoWhiteBoard(p);

        return;
}
