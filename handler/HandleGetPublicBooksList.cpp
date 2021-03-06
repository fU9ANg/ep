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
#include "../netdef.h"
 
void CHandleMessage::handleGetPublicBooksList (Buf *p) 
{
#ifdef __DEBUG_HANDLE_HEAD_
	cout << "CT_GetPublicBooksList" << endl;
#endif	
        const epUser* pUser = EPMANAGER->getUserByFd(p->getfd());
        CHECK_P(pUser);

        cGetPublicBooksList cgbl;
        UNPACKET(p, cgbl);

        bookList book_list;
        bookNode* book_node;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_COURSE_LIST_BY_OWNERID_AND_TYPE_PUBLIC);
#ifdef __DEBUG__
                std::cout << "[DEBUG] owner_id = " << cgbl.owner_id() << std::endl;
                std::cout << "[DEBUG] owner_type = " << cgbl.owner_type() << std::endl;
#endif
                pstmt->setInt(1, cgbl.owner_id());
                pstmt->setInt(2, cgbl.owner_id());
                pstmt->setInt(3, cgbl.owner_id());
                pstmt->setString(4, cgbl.owner_type());
                ResultSet* prst = pstmt->executeQuery ();
                while (prst->next ()) {
                        book_node = book_list.add_book_list();
                        book_node->set_book_id  (prst->getInt   ("book_id"));
                        book_node->set_book_name(prst->getString("book_name"));
                        book_node->set_book_type(prst->getInt   ("book_type"));
                        book_node->set_auth_id  (prst->getInt   ("auth_id"));
                        book_node->set_auth_type((enum LoginType)prst->getInt("auth_type"));

#ifdef __DEBUG__
                        std::cout << "book_node->book_id  () = " << book_node->book_id  () << std::endl;
                        std::cout << "book_node->book_name() = " << book_node->book_name() << std::endl;
                        std::cout << "book_node->book_type() = " << book_node->book_type() << std::endl;
                        std::cout << "book_node->auth_id  () = " << book_node->auth_id  () << std::endl;
                        std::cout << "book_node->auth_type() = " << book_node->auth_type() << std::endl;
#endif
                }
                delete prst;
                delete pstmt;
        } catch (SQLException e) {
                PRINT_CATCH(e);
                RETURN(p);
        }

        Buf* pBuf = packet_list(ST_GetPublicBooksList, book_list, p->getfd());
        CHECK_P(pBuf);
        SINGLE->sendqueue.enqueue(pBuf);

        RETURN(p);
}
