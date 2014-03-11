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
        CHECK_P(pUser);

        epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
        CHECK_P(pClassroom);

        sCourseware tmp;
        UNPACKET(p, tmp);

        int idx = 0;
        if (tmp.has_idx())
                idx = tmp.idx();
        pClassroom->changeCoursewareStatus(idx, tmp.cs());
        printf("[DEBUG] CHandleMessage::handleCourseware ：course_id = %d\n", tmp.course_id());

        ((MSG_HEAD*)p->ptr())->cType = ST_Courseware;
        Buf* pBuf = NULL;
        CLONE_BUF(pBuf, p);
        pClassroom->sendtoAllClass(pBuf);
        pClassroom->sendtoWhiteBoard(p);

        return;
}
