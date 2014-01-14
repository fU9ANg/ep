#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"
#include "Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

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
        std::vector<sGetStudentList> vc;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_STU_LIST_BY_CLASSID);
                pstmt->setInt (1, gfl.class_id());
                printf("[DEBUG] %s : class id = %d\n", __func__, gfl.class_id());
                ResultSet* prst = pstmt->executeQuery ();
                while (prst->next ()) {
#if 0
                        tmp.set_id      (prst->getInt   ("id"));
                        tmp.set_name    (prst->getString("name"));
                        tmp.set_res_path(prst->getString("res_path"));//res for icon
#endif
                        tmp.set_id      (prst->getInt   ("student_id"));
                        tmp.set_name    (prst->getString("first_name"));
                        tmp.set_res_path(prst->getString("last_name"));//res for icon


                        vc.push_back(tmp);
                }
                delete prst;
                delete pstmt;
        }catch (SQLException e) {
        }

        Buf* pBuf = packet(ST_GetStudentList, vc, p->getfd());
        if (NULL != pBuf) {
                SINGLE->sendqueue.enqueue(pBuf);
        }
        SINGLE->bufpool.free(p);
}
