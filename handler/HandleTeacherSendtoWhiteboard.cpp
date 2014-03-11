#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"

void CHandleMessage::handleTeacherSendtoWhiteboard(Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_TeacherSendtoWhiteboard\n";
#endif

        epClassroom* p_classroom = EPMANAGER->getClassroomByFd(p->getfd());
        CHECK_P(p_classroom);

        enum CommandType type = ((MSG_HEAD*)p->ptr())->cType;
        switch (type) {
        case CT_Enlarge :
#ifdef __DEBUG__
                printf("CT_Enlarge\n");
#endif
                type = ST_Enlarge;
                break;
        case CT_Reduce :
#ifdef __DEBUG__
                printf("CT_Reduce\n");
#endif
                type = ST_Reduce;
                break;
        case CT_unDisplay :
#ifdef __DEBUG__
                printf("CT_unDisplay\n");
#endif
                type = ST_unDisplay;
                p_classroom->undisplay_ = false;
                break;
        default :
                break;
        }

        ((MSG_HEAD*)p->ptr())->cType = type;
        p_classroom->sendtoWhiteBoard(p);
        return;
}
