#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"
#include "Single.h"
#include "../message/proto/protocol.pb.h"

void CHandleMessage::handleGetStudentList (Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetStudentList\n";
#endif
        // TODO:

        cGetFuncList gfl;
        unpacket(p, gfl);

        sGetFuncList tmp;
        std::vector<sGetFuncList> vc;
        string strpwd;
        string Account;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_FUNC_LIST_BY_TYPE);
                pstmt->setInt (1, gfl.class_id());
                ResultSet* prst = pstmt->executeQuery ();
                while (prst->next ()) {
                        tmp.set_id(prst->getInt("id"));
                        tmp.set_name(prst->getString("name"));
                        tmp.set_res_path(prst->getString("res_path"));
                        vc.push_back(tmp);
                }
                delete prst;
                delete pstmt;
        }catch (SQLException e) {
        }

        SINGLE->sendqueue.enqueue(packet(ST_GetFuncList, vc, p->getfd()));
        SINGLE->bufpool.free(p);
}
