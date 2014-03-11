#include "HandleMessage.h"
#include "../netdef.h"
#include "../global_functions.h"
#include "../content/epUser.h"
#include "../content/epManager.h"
#include "../message/proto/protocol.pb.h"

void
CHandleMessage::handleUpdateCourseware(Buf* p) {
#ifdef __DEBUG_HANDLE_HEAD_
        std::cout << "CT_UpdateCourseware" << std::endl;
#endif
        epClassroom* p_classroom = const_cast<epClassroom*>(EPMANAGER->getClassroomByFd(p->getfd()));
        CHECK_P(p_classroom);

        const epUser* p_user = p_classroom->getUserByFd(p->getfd());
        CHECK_P(p_classroom);

        epClass* p_class = p_classroom->class_;
        CHECK_P(p_class);

        UpdateCourseware cuc;
        UNPACKET(p, cuc);

        Buf* p_buf = NULL;
        CLONE_BUF(p_buf, p);
        ((MSG_HEAD*)p_buf->ptr())->cType = ST_UpdateCourseware;
        switch (p_user->getType()) {
        case LT_TEACHER :
                p_class->sendtoAllStudent(p_buf);
                break;
        case LT_STUDENT :
                p_classroom->sendtoTeacher(p_buf);
                break;
        default :
                break;
        }

        RETURN(p);
}
