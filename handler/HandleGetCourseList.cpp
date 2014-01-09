#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"
#include "Single.h"
#include "../message/proto/protocol.pb.h"

void CHandleMessage::handleGetCourseList (Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
    cout << "CT_GetCourseList\n";
#endif
    // TODO:
}
