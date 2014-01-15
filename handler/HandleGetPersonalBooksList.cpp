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
        std::vector<sGetPersonalBooksList> vc;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_COURSE_LIST_BY_AUTHOR_ID_AND_TYPE_PERSONAL);
                pstmt->setInt (1, pUser->getId());
                pstmt->setInt (2, pUser->getType());
                ResultSet* prst = pstmt->executeQuery ();
                while (prst->next ()) {
                        tmp.set_book_id  (prst->getInt   ("book_id"));
                        tmp.set_book_name(prst->getString("book_name"));
                        tmp.set_book_type(prst->getInt   ("book_type"));
                        tmp.set_res_path (prst->getString("res_path"));
                        vc.push_back(tmp);
                }
                delete prst;
                delete pstmt;
        }catch (SQLException e) {
                printf("[DEBUG] CHandleMessage::handleGetPersonalBooksList : SQLException = %s\n", e.what());
                SINGLE->bufpool.free(p);
                return;
        }

        for (int i=0; i<(signed)vc.size(); ++i) {
                Buf* pBuf = packet(ST_GetPersonalBooksList, vc[i], p->getfd());
                if (NULL != pBuf) {
                        *(int*)((char*)pBuf->ptr() + MSG_HEAD_LEN) = vc.size();
                        SINGLE->sendqueue.enqueue(pBuf);
                }
        }
}
