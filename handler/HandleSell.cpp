#include "HandleMessage.h"
#include "../netdef.h"
#include "../global_functions.h"
#include "../content/epUser.h"
#include "../content/epManager.h"
#include "../database.h"

void
CHandleMessage::handleSell(Buf* p) {
#ifdef __DEBUG_HANDLE_HEAD_
        std::cout << "CT_Sell" << std::endl;
#endif
        const epUser* p_user = EPMANAGER->getUserByFd(p->getfd());
        CHECK_P(p_user);

        cSell cs;
        UNPACKET(p, cs);

        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement \
                                           ("CALL epdb.sql_sp_sell(?, ?, ?)");
                pstmt->setInt   (1, cs.book_id());
                pstmt->setInt   (2, cs.auth_id());
                pstmt->setString(3, cs.auth_type());

#ifdef __DEBUG__
                std::cout << "cs.book_id        = " << cs.book_id()   << std::endl;
                std::cout << "cs.to_auth_id()   = " << cs.auth_id()   << std::endl;
                std::cout << "cs.to_auth_type() = " << cs.auth_type() << std::endl;
#endif

                pstmt->execute ();
                delete pstmt;
        } catch (SQLException e) {
                PRINT_CATCH(e);
                RETURN(p);
        }

        bool result = true;
        std::string msg = "出售成功！";

        sResult tmp;
        if (result)
                tmp.set_result(result);
        if (!msg.empty())
                tmp.set_msg(msg);

        Buf* pBuf = packet(ST_Sell, tmp, p->getfd());
        CHECK_P(pBuf);
        SINGLE->sendqueue.enqueue(pBuf);

        RETURN(p);
}
