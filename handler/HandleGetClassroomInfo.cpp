#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"

void CHandleMessage::handleGetClassroomInfo(Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetClassroomInfo\n";
#endif

        epClassroom* p_classroom = const_cast<epClassroom*>(EPMANAGER->getClassroomByFd(p->getfd()));
        if (NULL == p_classroom) {
                printf("[DEBUG] CHandleMessage::handleGetClassroomInfo : NULL == p_classroom\n");
                SINGLE->bufpool.free(p);
                return;
        }

        sGetClassroomInfo tmp;
        tmp.set_class_id(p_classroom->getClassId());
        tmp.set_grade_id(p_classroom->gradeId_);
        Buf* p_buf = packet(ST_GetClassroomInfo, tmp, p->getfd());
        CHECK_BUF(p_buf, p);
        SINGLE->sendqueue.enqueue(p_buf);
        SINGLE->bufpool.free(p);
        return;
}
