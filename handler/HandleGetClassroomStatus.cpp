#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"

void CHandleMessage::handleGetClassroomStatus(Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetClassroomStatus\n";
#endif

        epClassroom* p_classroom = const_cast<epClassroom*>(EPMANAGER->getClassroomByFd(p->getfd()));
        CHECK_P(p_classroom);

        sGetClassroomStatus tmp;
        tmp.set_cs(p_classroom->classroomStatus_);

        Buf* p_buf = packet(ST_GetClassroomStatus, tmp, p->getfd());
        CHECK_P(p_buf);
        SINGLE->sendqueue.enqueue(p_buf);

        RETURN(p);
}
