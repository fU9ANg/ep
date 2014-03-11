#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"
#include "../netdef.h"

#include "../content/epManager.h"

void CHandleMessage::handleRelay(Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_Relay\n";
#endif

        const epUser* pUser = EPMANAGER->getUserByFdFromClassroom(p->getfd());
        CHECK_P(pUser);

        epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
        CHECK_P(pClassroom);

        epGroup* pGroup = const_cast<epGroup*>(pClassroom->getGroupByFd(p->getfd()));
        CHECK_P(pGroup);

        sRelay tmp;
        tmp.set_student_id(pGroup->getNextIdByFd(p->getfd()));

        Buf* p_1 = packet(ST_Relay, tmp, p->getfd());
        CHECK_P(p_1);
        pClassroom->sendtoAll(p_1);

        RETURN(p);
}
