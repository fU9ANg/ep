#include "HandleMessage.h"
#include "../netdef.h"

void
CHandleMessage::handlePublish(Buf* p) {
        SINGLE->bufpool.free(p);
        return;
}
