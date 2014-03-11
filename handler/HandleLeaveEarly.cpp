#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"

void CHandleMessage::handleLeaveEarly(Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_LeaveEarly\n";
#endif

        epTeacher* p_teacher = const_cast<epTeacher*>(EPMANAGER->getTeacherByFd(p->getfd()));
        CHECK_P(p_teacher);

        cLeaveEarly tmp;
        UNPACKET(p, tmp);

        epClassroom* p_classroom = EPMANAGER->getClassroomByFd(p->getfd());
        CHECK_P(p_classroom);

        const epStudent* p_student = EPMANAGER->getStudentByIdFromClassroom(tmp.student_id());
        CHECK_P(p_student);
        Buf* p_buf_1 = packet(ST_LeaveEarly, p_student->fd_);
        CHECK_P(p_buf_1);
        SINGLE->sendqueue.enqueue(p_buf_1);

        sUpdateStudentStatus suss;
        suss.set_student_id(tmp.student_id());
        suss.set_us(US_OFFLINE);
        suss.set_login_type(LT_STUDENT);

        Buf* p_buf = packet(ST_UpdateStudentStatus, suss, p->getfd());
        CHECK_P(p_buf);
        p_classroom->sendtoAll(p_buf);
        EPMANAGER->moveStudentToUser(tmp.student_id());

        RETURN(p);
}
