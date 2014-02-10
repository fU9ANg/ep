#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"
#include "Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"
#include "../netdef.h"

void CHandleMessage::handleGetStudentList (Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetStudentList\n";
#endif
        // TODO:

        cGetStudentList gfl;
        if (!unpacket(p, gfl)) { // 解包失败。
#ifdef __DEBUG__
                printf("[DEBUG] %s : unpacket fail!\n", __func__);
#endif
                SINGLE->bufpool.free(p);
                return;
        }

        sGetStudentList tmp;
        StudentListNode* sn;
        bool isvalue = false;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_STU_LIST_BY_CLASSID);
                pstmt->setInt (1, gfl.class_id());
                // printf("[DEBUG] %s : class id = %d\n", __func__, gfl.class_id());
                ResultSet* prst = pstmt->executeQuery ();
                isvalue = prst->next();
                while (isvalue) {
                        sn = tmp.add_student_list();
                        sn->set_id      (prst->getInt   ("student_id"));
                        sn->set_name    (prst->getString("first_name"));
                        sn->set_res_path(prst->getString("last_name"));//res for icon
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
        CHECK_BUF(pBuf, p);
        SINGLE->sendqueue.enqueue(pBuf);

        SINGLE->bufpool.free(p);
        return;
}
