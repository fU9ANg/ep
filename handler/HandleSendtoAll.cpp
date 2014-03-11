#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"

void CHandleMessage::handleSendtoAll(Buf* p)
{
        enum CommandType type = ((MSG_HEAD*)p->ptr())->cType;
        switch (type) {
        case CT_Lock :
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_Lock\n";
#endif
                type = ST_Lock;
                break;
        case CT_unLock :
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_unLock\n";
#endif
                type = ST_unLock;
                break;
        default :
                break;
        }

        ((MSG_HEAD*)p->ptr())->cType = type;
        Buf* p_buf = NULL;
        CLONE_BUF(p_buf, p);
        EPMANAGER->sendtoAllClassroom(p_buf);
        EPMANAGER->sendtoAllUser(p);
        return;
}
