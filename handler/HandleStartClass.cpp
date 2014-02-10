#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"
#include "Single.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"

void CHandleMessage::handleStartClass (Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_StartClass\n";
#endif

        const epUser* pUser = EPMANAGER->getUserByFdFromClassroom(p->getfd());
        if (NULL == pUser) { // 该教师不在上课。
                printf("[DEBUG] CHandleMessage::handleStartClass : NULL == pUser\n");
                SINGLE->bufpool.free(p);
                return;
        }

        epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
        if (NULL == pClassroom) {
                printf("[DEBUG] CHandleMessage::handleStartClass : NULL == pClassroom\n");
                SINGLE->bufpool.free(p);
                return;
        }

        Buf* pBuf1 = packet(ST_StartClass, p->getfd());
        if (NULL != pBuf1) {
                pClassroom->sendtoAllClass(pBuf1);
        }

        Buf* pBuf2 = packet(ST_StartClass, p->getfd());
        if (NULL != pBuf2) {
                pClassroom->sendtoWhiteBoard(pBuf2);
        }

        SINGLE->bufpool.free(p);
        return;
}
