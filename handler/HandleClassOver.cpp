#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"

void CHandleMessage::handleClassOver(Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_ClassOver\n";
#endif

        epUser* pUser = const_cast<epUser*>(EPMANAGER->getUserByFdFromClassroom(p->getfd()));
        CHECK_P(pUser);

        const epTeacher* pTeacher = dynamic_cast<epTeacher*>(pUser);
        CHECK_P(pTeacher);

        epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
        CHECK_P(pClassroom);

        Buf* pBuf = packet(ST_ClassOver, p->getfd());
        CHECK_P(pBuf);
        Buf* p1 = NULL;
        CLONE_BUF(p1, pBuf);
        pClassroom->sendtoAll(p1);

        EPMANAGER->deleteClassroomById(pClassroom->id_);
        pClassroom = NULL;

        RETURN(p);
}
