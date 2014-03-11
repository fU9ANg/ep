#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"
#include "Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"

void CHandleMessage::handleGetClassInfo(Buf* p) 
{
#ifdef __DEBUG_HANDLE_HEAD_
	cout << "CT_GetClassInfo\n";
#endif

	sGetClassInfo tmp;

	epClassroom* p_classroom = const_cast<epClassroom*>(EPMANAGER->getClassroomByFd(p->getfd()));
        CHECK_P(p_classroom);

        epClass* p_class = p_classroom->class_;
        CHECK_P(p_class);

        tmp.set_class_id(p_class->id_);

        Buf* pBuf = packet(ST_GetClassInfo, tmp, p->getfd());
        CHECK_P(pBuf);
        SINGLE->sendqueue.enqueue(pBuf);

        RETURN(p);
}
