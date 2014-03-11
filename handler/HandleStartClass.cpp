#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"
#include "Single.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"

void CHandleMessage::handleStartClass (Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_StartClass\n";
#endif

        const epUser* pUser = EPMANAGER->getUserByFdFromClassroom(p->getfd());
        CHECK_P(pUser);

        epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
        CHECK_P(pUser);

        pClassroom->classroomStatus_ = CS_START_CLASS;

        Buf* pBuf1 = packet(ST_StartClass, p->getfd());
        CHECK_P(pBuf1);
        pClassroom->sendtoAllClass(pBuf1);

        Buf* pBuf2 = packet(ST_StartClass, p->getfd());
        CHECK_P(pBuf2);
        pClassroom->sendtoWhiteBoard(pBuf2);

        RETURN(p);
}
