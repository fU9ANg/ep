#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"

void CHandleMessage::handleGetCoursewareListStatus(Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetCoursewareListStatus\n";
#endif

        epClassroom* p_classroom = EPMANAGER->getClassroomByFd(p->getfd());
        CHECK_P(p_classroom);

        EPCOURSEWARE_VECTOR cs = p_classroom->getCoursewareListStatus();
        EPCOURSEWARE_VECTOR::iterator it = cs.begin();
        EPCOURSEWARE_VECTOR::const_iterator cie = cs.end();

        sGetCoursewareListStatus gcls;
        CoursewareStatusNode* csn;
        for (int i=0; cie!=it; ++it, ++i) {
                printf("[DEBUG] CHandleMessage::handleGetCoursewareListStatus : idx = %d\n", i);
                csn = gcls.add_course_status_list();
                // csn->set_course_id(it->first);
                if (0 != i)
                        csn->set_idx(i);
                csn->set_cs       (*it);
        }

        Buf* p_buf = packet(ST_GetCoursewareListStatus, gcls, p->getfd());
        CHECK_BUF(p_buf, p);
        SINGLE->sendqueue.enqueue(p_buf);

        RETURN(p);
}
