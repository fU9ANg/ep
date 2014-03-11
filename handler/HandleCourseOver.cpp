#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"

void CHandleMessage::handleCourseOver(Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_CourseOver\n";
#endif

        const epUser* pUser = EPMANAGER->getUserByFdFromClassroom(p->getfd());
        CHECK_P(pUser);

        epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
        CHECK_P(pClassroom);

        cCourseOver cco;
        UNPACKET(p, cco);
        int idx = 0;
        if (cco.has_idx())
                idx = cco.idx();
        pClassroom->changeCoursewareStatus(idx, CS_OVER);
        pClassroom->removeAllGroup();
        ((MSG_HEAD*)p->ptr())->cType = ST_CourseOver;
        Buf* pBuf = NULL;
        CLONE_BUF(pBuf, p);
        pClassroom->sendtoAllClass(pBuf);
        pClassroom->sendtoWhiteBoard(p);

        return;
}
