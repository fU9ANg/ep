#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epUser.h"
#include "../content/epTeacher.h"
#include "../content/epManager.h"

void CHandleMessage::handleGetCourseList (Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetCourseList\n";
#endif
        // TODO:

        cGetCourseList gcl;
        unpacket(p, gcl);
        int grade_id = gcl.grade_id();

        sGetCourseList tmp;
        std::vector<sGetCourseList> vc;
        string strpwd;
        string Account;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_COURSE_LIST_BY_GRADE_ID);
                pstmt->setInt (1, grade_id);
                ResultSet* prst = pstmt->executeQuery ();
                while (prst->next ()) {
                        tmp.set_course_id  (prst->getInt   ("course_id"));
                        tmp.set_course_name(prst->getString("course_name"));
                        tmp.set_language   (prst->getInt   ("language"));
                        tmp.set_art        (prst->getInt   ("art"));
                        tmp.set_community  (prst->getInt   ("community"));
                        tmp.set_health     (prst->getInt   ("health"));
                        tmp.set_science    (prst->getInt   ("science"));
                        tmp.set_res_path   (prst->getString("resPath"));
                        vc.push_back(tmp);
                }
                delete prst;
                delete pstmt;
        }catch (SQLException e) {
        }

        SINGLE->sendqueue.enqueue(packet(ST_GetCourseList, vc, p->getfd()));
}
