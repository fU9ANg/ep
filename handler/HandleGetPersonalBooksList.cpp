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

void CHandleMessage::handleGetPersonalBooksList (Buf *p) 
{
        /**
         * @brief 教师请求个人资源列表 
         *        1.检查用户类型是否是教师。
         *        2.根据教编号num，查找数据库，返回该教师所有的资源列表。
         */

#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetPersonalBooksList" << endl;
#endif
        const epUser* pUser = EPMANAGER->getUserByFd(p->getfd());
        if (NULL == pUser) {
                printf("[DEBUG] CHandleMessage::handleGetPersonalBooksList : NULL == pUser\n");
                SINGLE->bufpool.free(p);
                return;
        }

        cGetPersonalBooksList gpbl;
        if (!unpacket(p, gpbl)) { // 解包失败。
                printf("[DEBUG] CHandleMessage::handleGetPersonalBooksList unpacket fail!\n");
                SINGLE->bufpool.free(p);
                return;
        }

        sGetPersonalBooksList tmp;
        bookNode *book_node;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_COURSE_LIST_BY_AUTHOR_ID_AND_TYPE_PERSONAL);
                pstmt->setInt (1, pUser->id_);
                pstmt->setInt (2, pUser->getType());
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
        } catch (SQLException e) {
                printf("[DEBUG] CHandleMessage::handleGetPersonalBooksList : SQLException = %s\n", e.what());
                SINGLE->bufpool.free(p);
                return;
        }

        Buf* pBuf = packet_list(ST_GetPersonalBooksList, tmp, p->getfd());
        if (NULL != pBuf) {
                SINGLE->sendqueue.enqueue(pBuf);
        } else {
                printf("[DEBUG] CHandleMessage::handleGetPersonalBooksList : NULL == pBuf\n");
        }
        SINGLE->bufpool.free(p);
        return;
}
