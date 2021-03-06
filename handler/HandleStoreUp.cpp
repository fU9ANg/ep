#include "HandleMessage.h"
#include "../netdef.h"
#include "../global_functions.h"
#include "../content/epUser.h"
#include "../content/epManager.h"
#include "../database.h"

void
CHandleMessage::handleStoreUp(Buf* p) {
#ifdef __DEBUG_HANDLE_HEAD_
        std::cout << "CT_StoreUp" << std::endl;
#endif
        const epUser* p_user = EPMANAGER->getUserByFd(p->getfd());
        CHECK_P(p_user);

        bookNode bn;
        UNPACKET(p, bn);

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
                                           ("CALL epdb.sql_sp_storeup(?, ?, ?, ?, ?)");
                pstmt->setInt   (1, bn.book_id());
                pstmt->setInt   (2, p_user->id_);
                pstmt->setString(3, auth_type);

#ifndef __DEBUG__
                std::cout << "book_id   = " << bn.book_id() << std::endl;
                std::cout << "auth_id   = " << p_user->id_  << std::endl;
                std::cout << "auth_type = " << auth_type    << std::endl;
#endif

                pstmt->execute ();
                delete pstmt;
        } catch (SQLException e) {
                PRINT_CATCH(e);
                RETURN(p);
        }

        Buf* p_buf = packet(ST_UpdatePublic, bn, p->getfd());
        CHECK_P(p_buf);
        SINGLE->sendqueue.enqueue(p_buf);

        bool result = true;
        std::string msg = "收藏成功！";

        sResult tmp;
        if (result)
                tmp.set_result(result);
        if (!msg.empty())
                tmp.set_msg(msg);

        Buf* p_buf_1 = packet(ST_Purchase, tmp, p->getfd());
        CHECK_P(p_buf_1);
        SINGLE->sendqueue.enqueue(p_buf_1);

        RETURN(p);
}
