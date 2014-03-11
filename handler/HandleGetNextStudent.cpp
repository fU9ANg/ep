#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"
#include "../content/epGroup.h"

void CHandleMessage::handleGetNextStudent(Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
	cout << "CT_GetNextStudent\n";
#endif

	epGroup* pGroup = const_cast<epGroup*>(EPMANAGER->getGroupByFd(p->getfd()));
	CHECK_P(pGroup);

        const epStudent* p_student = pGroup->getNextStudentByFd(p->getfd());
        CHECK_P(p_student);

        cGetNextStudent from;
        UNPACKET(p, from);

        sGetNextStudent to;
        to.set_opt        (from.opt());
        to.set_pos_x      (from.pos_x());
        to.set_pos_y      (from.pos_y());
        to.set_student_id (p_student->id_);

        Buf* p_buf = packet(ST_GetNextStudent, to, p_student->fd_);
        CHECK_P(p_buf);
        SINGLE->sendqueue.enqueue(p_buf);

        epClassroom* pClassroom = const_cast<epClassroom*>(EPMANAGER->getClassroomByFd(p->getfd()));
        CHECK_P(pClassroom);

        Buf* p_buf_1 = packet(ST_GetNextStudent, to, 0);
        CHECK_P(p_buf_1);
        pClassroom->sendtoTeacher(p_buf_1);

        Buf* p_buf_2 = packet(ST_GetNextStudent, to, 0);
        CHECK_P(p_buf_2);
        pClassroom->sendtoWhiteBoard(p_buf_2);
        
        RETURN(p);
}

