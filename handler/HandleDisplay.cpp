#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"

void CHandleMessage::handleDisplay(Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
    cout << "CT_Display\n";
#endif

    epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
    CHECK_P(pClassroom);

    pClassroom->undisplay_ = true;
    ((MSG_HEAD*)p->ptr())->cType = ST_Display;
    pClassroom->sendtoWhiteBoard(p);

    return;
}
