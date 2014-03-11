#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"

void CHandleMessage::handleLockConn(Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_LockConn\n";
#endif
        epClassroom* p_classroom = EPMANAGER->getClassroomByFd(p->getfd());
        CHECK_P(p_classroom);

        epGroup* p_group = const_cast<epGroup*>(p_classroom->getGroupByFd(p->getfd()));
        CHECK_P(p_group);

        cUpdateMutex cum;
        UNPACKET(p, cum);
        bool result = false;
        switch (cum.ms()) {
        case MS_LOCK :
                result = p_group->lock(cum.lock_id());
                break;
        case MS_UNLOCK :
                result = p_group->unlock(cum.lock_id());
                break;
        default :
                break;
        }

        sUpdateMutex tmp;
        tmp.set_student_id(cum.student_id());
        tmp.set_lock_id(cum.lock_id());
        tmp.set_ms(cum.ms());
        if (result)
                tmp.set_result(result);

        std::cout << "student_id = " << cum.student_id() << std::endl;
        std::cout << "lock_id    = " << cum.lock_id() << std::endl;
        std::cout << "ms         = " << cum.ms() << std::endl;

        Buf* p_buf = packet(ST_LockConn, tmp, p->getfd());
        CHECK_P(p_buf);
        p_group->sendtoAllStudent(p_buf, true);

        RETURN(p);
}
