#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"

void CHandleMessage::handleCourseware(Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_Courseware\n";
#endif

        const epUser* pUser = EPMANAGER->getUserByFdFromClassroom(p->getfd());
        if (NULL == pUser) {
                printf("[DEBUG] CHandleMessage::handleCourseware : NULL == pUser\n");
                SINGLE->bufpool.free(p);
                return;
        }

        epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
        if (NULL == pClassroom) {
                printf("[DEBUG] CHandleMessage::handleCourseware : NULL == pClassroom\n");
                SINGLE->bufpool.free(p);
                return;
        }

        ((MSG_HEAD*)p->ptr())->cType = ST_Courseware;
        Buf* pBuf = NULL;
        CLONE_BUF(pBuf, p);
        pClassroom->sendtoAllClass(pBuf);
        pClassroom->sendtoWhiteBoard(p);

        return;
}
