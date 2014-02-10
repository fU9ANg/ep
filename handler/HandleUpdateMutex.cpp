#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"

void CHandleMessage::handleUpdateMutex (Buf* p) {
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_UpdateMutex\n";
#endif

        epGroup* pGroup = const_cast<epGroup*>(EPMANAGER->getGroupByFd(p->getfd()));
        if (NULL == pGroup) {
                printf("[DEBUG] CHandleMessage::handleUpdateMutex : NULL == pGroup\n");
                SINGLE->bufpool.free(p);
                return;
        }

        cUpdateMutex cum;
        if (!unpacket(p, cum)) {
                printf("[DEBUG] CHandleMessage::handleUpdateMutex : unpacket fail!\n");
                SINGLE->bufpool.free(p);
                return;
        }

        sUpdateMutex tmp;
        tmp.set_result(pGroup->setLock(cum.lock()) ? TRUE : FALSE);
        Buf* pBuf = packet(ST_UpdateMutex, tmp, p->getfd());
        if (NULL != pBuf) {
                SINGLE->sendqueue.enqueue(pBuf);
        }
        SINGLE->bufpool.free(p);
        return;
}
