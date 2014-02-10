#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"

void CHandleMessage::handleGetActiveStudentList_1 (Buf* p) {
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetActiveStudentList_1\n";
#endif
        cGetActiveStudentList casl;
        if (!unpacket(p, casl)) { // 解包失败。
#ifdef __DEBUG__
                printf("[DEBUG] %s : unpacket fail!\n", __func__);
#endif
                SINGLE->bufpool.free(p);
                return;
        }

        std::vector<int> vi = EPMANAGER->getActiveStudentListFromClass(casl.class_id());
        for (int i=0; i<(signed)vi.size(); ++i) {
                printf("[DEBUG] CHandleMessage::handleGetActiveStudentList fd[%d]: idx = %d\n", p->getfd(), vi[i]);
        }

        printf("[DEBUG] CHandleMessage::handleGetActiveStudentList : ST_GetActiveStudentList_1 = %d\n", ST_GetActiveStudentList_1);
        Buf* pBuf = packet(ST_GetActiveStudentList_1, vi, p->getfd());
        if (NULL != pBuf) {
                printf("[DEBUG] CHandleMessage::handleGetActiveStudentList packet type = %d\n", ((MSG_HEAD*)pBuf->ptr())->cType);
                SINGLE->sendqueue.enqueue(pBuf);
        }

        SINGLE->bufpool.free(p);
        return;
}
