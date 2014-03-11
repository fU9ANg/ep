#include "HandleMessage.h"
#include "../netdef.h"

void
CHandleMessage::handleUpdatePublic(Buf* p) {
#ifdef __DEBUG_HANDLE_HEAD_
        std::cout << "CT_UpdatePublic" << std::endl;
#endif
        RETURN(p);
}
