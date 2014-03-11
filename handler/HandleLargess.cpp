#include "HandleMessage.h"
#include "../netdef.h"
#include "../global_functions.h"
#include "../content/epUser.h"
#include "../content/epManager.h"
#include "../database.h"

void
CHandleMessage::handleLargess(Buf* p) {
#ifdef __DEBUG_HANDLE_HEAD_
        std::cout << "CT_Largess" << std::endl;
#endif
        const epUser* p_user = EPMANAGER->getUserByFd(p->getfd());
        CHECK_P(p_user);

        cChangeOwner cco;
        UNPACKET(p, cco);

        std::string auth_type;
        switch (p_user->getType()) {
        case LT_TEACHER :
                auth_type = "教师";
                break;
        case LT_STUDENT :
                auth_type = "学生";
                break;
        case LT_HEADMASTER :
                auth_type = "院长";
                break;
        case LT_PARENTS :
                auth_type = "家长";
        default :
                DEBUG_INFO;
                break;
        }

        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement \
                                           ("CALL epdb.sql_sp_largess(?, ?, ?, ?)");
                pstmt->setInt   (1, cco.book_id());
                pstmt->setInt   (2, p_user->id_);
                pstmt->setString(3, auth_type);
                pstmt->setString(4, cco.to_auth_account());

#ifdef __DEBUG__
                std::cout << "book_id               = " << cco.book_id()            << std::endl;
                std::cout << "p_user->id_           = " << p_user->id_              << std::endl;
                std::cout << "auth_type             = " << auth_type                << std::endl;
                std::cout << "cco.to_auth_account() = " << cco.to_auth_account() << std::endl;
#endif

                pstmt->execute ();
                delete pstmt;
        } catch (SQLException e) {
                PRINT_CATCH(e);
                RETURN(p);
        }

        bool result = true;
        std::string msg = "赠送成功！";

        sResult tmp;
        if (result)
                tmp.set_result(result);
        if (!msg.empty())
                tmp.set_msg(msg);

        Buf* pBuf = packet(ST_Largess, tmp, p->getfd());
        CHECK_P(pBuf);
        SINGLE->sendqueue.enqueue(pBuf);

        RETURN(p);
}
