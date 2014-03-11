#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"

void CHandleMessage::handleGetSchoolAccountList(Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetSchoolAccountList\n";
#endif
#if 0
        const epUser* p_user = EPMANAGER->getUserByFd(p->getfd());
        if (NULL == p_user) {
                SINGLE->bufpool.free(p);
                return;
        }
#endif
        cGetSchoolAccountList gsal;
        UNPACKET(p, gsal);

        sGetSchoolAccountList tmp;
        AccountNode* an;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_SCHOOL_ACCOUNT_LIST_BY_SCHOOL_ID);
                printf ("gsal.school_id=%d\n", gsal.school_id());
                pstmt->setInt (1, gsal.school_id());
                pstmt->setInt (2, gsal.school_id());
                pstmt->setInt (3, gsal.school_id());
                pstmt->setInt (4, gsal.school_id());
                ResultSet* prst = pstmt->executeQuery ();
                while (prst->next ()) {
                        an = tmp.add_account_list();
                        an->set_account  (prst->getString("account"));
                        an->set_user_type((enum LoginType)prst->getInt   ("client_type_id"));
                        printf("[DEBUG] CHandleMessage::handleGetSchoolAccountList : an->account()=%s, an->client_type_id=%d\n", an->account().c_str(), an->user_type());
                }
                delete prst;
                delete pstmt;
        }catch (SQLException e) {
                printf("[DEBUG] %s : %s\n", __func__, e.what());
                SINGLE->bufpool.free(p);
                return;
        }

        Buf* p_buf = packet(ST_GetSchoolAccountList, tmp, p->getfd());
        CHECK_P(p_buf);
        SINGLE->sendqueue.enqueue(p_buf);

        RETURN(p);
}
