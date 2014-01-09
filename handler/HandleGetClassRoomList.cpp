#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"
#include "Single.h"
#include "../message/proto/protocol.pb.h"

void CHandleMessage::handleGetClassRoomList (Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
    cout << "CT_GetClassRoomList\n";
#endif
    // TODO:
}
