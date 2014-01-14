#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"

void CHandleMessage::handleGetActiveStudentList (Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetClassList\n";
#endif
        // TODO:

        cGetActiveStudentList casl;
        if (!unpacket(p, casl)) { // 解包失败。
#ifdef __DEBUG__
                printf("[DEBUG] %s : unpacket fail!\n", __func__);
#endif
                SINGLE->bufpool.free(p);
                return;
        }

        std::vector<int> vi = EPMANAGER->getActiveStudentListFromClass(casl.class_id());
        std::vector<sGetActiveStudentList> vg;
        for (int i=0; i<(signed)vi.size(); ++i) {
                sGetActiveStudentList tmp;
                tmp.set_id(vi[i]);
                vg.push_back(tmp);
        }

        Buf* pBuf = packet(ST_GetActiveStudentList, vg, p->getfd());

        if (NULL != pBuf) {
                SINGLE->sendqueue.enqueue(pBuf);
        }
        SINGLE->bufpool.free(p);
}
