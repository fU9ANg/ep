#include "HandleMessage.h"
#include "../global_functions.h"
#include "../netdef.h"
#include "message/proto/protocol.pb.h"

#include "../database.h"

#include "../content/epUser.h"
#include "../content/epManager.h"

void
CHandleMessage::handlePublish(Buf* p) {
#ifdef __DEBUG_HANDLE_HEAD_
        std::cout << "CT_Publish" << std::endl;
#endif
        const epUser* pUser = EPMANAGER->getUserByFd(p->getfd());
        CHECK_P(pUser);

        cPublish cub;
        UNPACKET(p, cub);

        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement \
                                           ("CALL epdb.sql_sp_publish(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
                pstmt->setInt   (1, cub.book_type());
                pstmt->setString(2, cub.book_name());
                pstmt->setString(3, cub.time());
                pstmt->setInt   (4, cub.belongs());
                pstmt->setInt   (5, cub.has_art() ? cub.art() : 0);
                pstmt->setInt   (6, cub.has_language() ? cub.language() : 0);
                pstmt->setInt   (7, cub.has_community() ? cub.community() : 0);
                pstmt->setInt   (8, cub.has_health() ? cub.health() : 0);
                pstmt->setInt   (9, cub.has_science() ? cub.science() : 0);
                pstmt->setString(10,cub.has_res_path() ? cub.res_path() : "");
                pstmt->setInt   (11,pUser->id_);
                pstmt->setInt   (12,pUser->getType());
                pstmt->setInt   (13,cub.course_type());

#ifdef __DEBUG__
                std::cout << "cub.book_type()   = " << cub.book_type()   << std::endl;
                std::cout << "cub.book_name()   = " << cub.book_name()   << std::endl;
                std::cout << "cub.time()        = " << cub.time()        << std::endl;
                std::cout << "cub.belongs()     = " << cub.belongs()     << std::endl;
                std::cout << "cub.art()         = " << cub.art()         << std::endl;
                std::cout << "cub.language()    = " << cub.language()    << std::endl;
                std::cout << "cub.community()   = " << cub.community()   << std::endl;
                std::cout << "cub.health()      = " << cub.health()      << std::endl;
                std::cout << "cub.science()     = " << cub.science()     << std::endl;
                std::cout << "cub.res_path()    = " << cub.res_path()    << std::endl;
                std::cout << "pUser->id_        = " << pUser->id_        << std::endl;
                std::cout << "pUser->getType()  = " << pUser->getType()  << std::endl;
                std::cout << "cub.course_type() = " << cub.course_type() << std::endl;
#endif

                pstmt->execute ();
                delete pstmt;
        } catch (SQLException e) {
                PRINT_CATCH(e);
                RETURN(p);
        }

        bool result = true;
        std::string msg = "插入成功！";

        sResult tmp;
        if (result)
                tmp.set_result(result);
        if (!msg.empty())
                tmp.set_msg(msg);

        Buf* pBuf = packet(ST_Publish, tmp, p->getfd());
        CHECK_P(pBuf);
        SINGLE->sendqueue.enqueue(pBuf); 

        RETURN(p);
}
