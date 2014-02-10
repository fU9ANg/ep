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
 
void CHandleMessage::handleGetPublicBooksList (Buf *p) 
{
#ifdef __DEBUG_HANDLE_HEAD_
	cout << "CT_GetPublicBooksList" << endl;
#endif	
        const epUser* pUser = EPMANAGER->getUserByFd(p->getfd());
        if (NULL == pUser) {
                printf("[DEBUG] CHandleMessage::handleGetPublicBooksList : NULL == pUser\n");
                SINGLE->bufpool.free(p);
                return;
        }

        cGetPublicBooksList gpbl;
        if (!unpacket(p, gpbl)) { // 解包失败。
                printf("[DEBUG] CHandleMessage::handleGetPublicBooksList : unpacket fail!\n");
                SINGLE->bufpool.free(p);
                return;
        }

        sGetPublicBooksList tmp;
        bookNode* book_node;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_COURSE_LIST_BY_AUTHOR_ID_AND_TYPE_PUBLISH);
                ResultSet* prst = pstmt->executeQuery ();
                while (prst->next ()) {
                        book_node = tmp.add_book_list();
                        book_node->set_book_id  (prst->getInt   ("book_id"));
                        book_node->set_book_name(prst->getString("book_name"));
                        book_node->set_book_type(prst->getInt   ("book_type"));
                        book_node->set_res_path (prst->getString("res_path"));
                }
                delete prst;
                delete pstmt;
        }catch (SQLException e) {
                printf("[DEBUG] CHandleMessage::handleGetPublicBooksList : SQLException = %s\n", e.what());
                SINGLE->bufpool.free(p);
                return;
        }

        Buf* pBuf = packet_list(ST_GetPublicBooksList, tmp, p->getfd());
        if (NULL != pBuf) {
                SINGLE->sendqueue.enqueue(pBuf);
        } else {
                printf("[DEBUG] CHandleMessage::handleGetPublicBooksList : NULL == pBuf\n");
        }
        SINGLE->bufpool.free(p);
        return;
}
