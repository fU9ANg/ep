#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"
#include "../netdef.h"

#include "../content/epManager.h"

void CHandleMessage::handleRelay(Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_Relay\n";
#endif

        const epUser* pUser = EPMANAGER->getUserByFdFromClassroom(p->getfd());
        if (NULL == pUser) {
                printf("[DEBUG] CHandleMessage::handleRelay : NULL == pUser\n");
                SINGLE->bufpool.free(p);
                return;
        }

        epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
        if (NULL == pClassroom) {
                printf("[DEBUG] CHandleMessage::handleRelay : NULL == pClassroom\n");
                SINGLE->bufpool.free(p);
                return;
        }

        epGroup* pGroup = const_cast<epGroup*>(pClassroom->getGroupByFd(p->getfd()));
        if (NULL == pGroup) {
                printf("[DEBUG] CHandleMessage::handleRelay : NULL == pGroup\n");
                SINGLE->bufpool.free(p);
                return;
        }

        sRelay tmp;
        tmp.set_student_id(pGroup->getNextIdByFd(p->getfd()));

        Buf* p_1 = packet(ST_Relay, tmp, p->getfd());
        if (NULL != p_1) {
                pClassroom->sendtoAll(p_1);
        }

        SINGLE->bufpool.free(p);
        return;
}
