#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"
#include "Single.h"
#include "../message/proto/protocol.pb.h"

void CHandleMessage::handleSetFunc (Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
    cout << "CT_SetFunc\n";
#endif
    // TODO:
}
