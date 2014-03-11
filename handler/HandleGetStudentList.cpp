#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"
#include "Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"
#include "../netdef.h"

#include "../content/epClassroom.h"
#include "../content/epManager.h"

void CHandleMessage::handleGetStudentList (Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetStudentList\n";
#endif
        cGetStudentList gfl;
        UNPACKET(p, gfl);

        int class_id = gfl.class_id();
        printf("[DEBUG] CHandleMessage::handleGetStudentList : class_id = %d\n", class_id);
        if (EPCLASS_INVALID_CLASS_ID == class_id) {
                epClassroom* p_classroom = EPMANAGER->getClassroomByFd(p->getfd());
                if (NULL!=p_classroom && NULL!=p_classroom->class_) {
                        class_id = p_classroom->class_->id_;
                        printf("[DEBUG] in classroom class_id = %d\n", p_classroom->class_->id_);
                }
        }

        sGetStudentList tmp;
        StudentListNode* sn;
        bool isvalue = false;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_STU_LIST_BY_CLASSID);
                pstmt->setInt (1, class_id);
                printf("[DEBUG] %s : class id = %d\n", __func__, class_id);
                ResultSet* prst = pstmt->executeQuery ();
                isvalue = prst->next();
                int cnt = 0;
                while (isvalue) {
                        printf("[DEBUG] CHandleMessage::handleGetStudentList : cnt = %d\n", ++cnt);
                        sn = tmp.add_student_list();
                        sn->set_id      (prst->getInt   ("student_id"));
                        sn->set_name    (prst->getString("last_name") + prst->getString("first_name"));
                        sn->set_res_path(prst->getString("res_path"));//res for icon
                        isvalue = prst->next();
                }
                delete prst;
                delete pstmt;
        } catch (SQLException e) {
                printf("[DEBUG] CHandleMessage::handleGetStudentList : SQLException = %s\n", e.what());
                SINGLE->bufpool.free(p);
                return;
        }

        Buf* pBuf = packet_list(ST_GetStudentList, tmp, p->getfd());
        CHECK_P(pBuf);

        /*
           sGetStudentList temp;
           unpacket(pBuf, temp);
           for (int i=0; i<temp.student_list_size(); ++i) {
           StudentListNode node = temp.student_list(i);
           printf("[DEBUG] CHandleMessage::handleGetStudentList : student_id = %d\n", node.id());
           printf("[DEBUG] CHandleMessage::handleGetStudentList : name       = %s\n", node.name().c_str());
           printf("[DEBUG] CHandleMessage::handleGetStudentList : res_path   = %s\n", node.res_path().c_str());
           }
           */

        SINGLE->sendqueue.enqueue(pBuf);

        RETURN(p);
}
