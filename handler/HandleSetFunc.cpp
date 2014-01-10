#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../protocol.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../content/epUser.h"

void CHandleMessage::handleSetFunc (Buf* p) {
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_SetFunc\n";
#endif
        // TODO:

        cSetFunc setFunc;
        unpacket(p, setFunc);

        sSetFunc tmp;
        epUser* pUser = EPMANAGER->getUserByFd(p->getfd());
        tmp.set_result((NULL==pUser) ? false : pUser->setFuncType((enum FuncType)setFunc.func_type()));
        SINGLE->sendqueue.enqueue(packet(ST_SetFunc, tmp, p->getfd()));
        SINGLE->bufpool.free(p);
}
