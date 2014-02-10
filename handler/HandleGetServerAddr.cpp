#include "HandleMessage.h"

void
CHandleMessage::handleGetServerAddr(Buf* p) {
        SINGLE->bufpool.free(p);
        return;
}
