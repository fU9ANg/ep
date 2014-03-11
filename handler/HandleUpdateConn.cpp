#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"

void CHandleMessage::handleUpdateConn(Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_UpdateConn\n";
#endif

        epClassroom* p_classroom = EPMANAGER->getClassroomByFd(p->getfd());
        CHECK_P(p_classroom);

        epGroup* p_group = const_cast<epGroup*>(p_classroom->getGroupByFd(p->getfd()));
        CHECK_P(p_group);

        ((MSG_HEAD*)p->ptr())->cType = ST_UpdateConn;
        p_classroom->sendtoAll(p, true);

        return;
}
