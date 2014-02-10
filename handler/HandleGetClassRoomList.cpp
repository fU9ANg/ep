#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"
#include "../netdef.h"

#include "../content/epUser.h"
#include "../content/epTeacher.h"
#include "../content/epManager.h"

void CHandleMessage::handleGetClassRoomList (Buf* p)
{
        /**
         * @brief 教师请求所在学校的教室列表。
         *        1. 检查用户类型是否为教师。
         *        2. 根据教师所在学校ID，查找数据库，返回该学校所有教室的列表。
         */

#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetClassRoomList\n";
#endif
        const epUser* pUser = EPMANAGER->getUserByFd(p->getfd());
        if (NULL == pUser) {
                printf("[DEBUG] CHandleMessage::handleGetClassRoomList : NULL == pUser\n");
                SINGLE->bufpool.free(p);
                return;
        }

        const epTeacher* pTeacher = dynamic_cast<const epTeacher*>(pUser);
        if (NULL == pTeacher) {
                printf("[DEBUG] CHandleMessage::handleGetClassRoomList : NULL == pTeacher\n");
                SINGLE->bufpool.free(p);
                return;
        }

        int school_id = pTeacher->schoolId_;

        sGetClassRoomList tmp;
        ClassListNode* cln;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_CLASSROOM_LIST_BY_SCHOOL_ID);
                pstmt->setInt (1, school_id);
                ResultSet* prst = pstmt->executeQuery ();
                while (prst->next ()) {
                        cln = tmp.add_classroom_list();
                        cln->set_id  (prst->getInt   ("classroom_id"));
                        cln->set_name(prst->getString("classroom_name"));
                }
                delete prst;
                delete pstmt;
        }catch (SQLException e) {
                printf("[DEBUG] CHandleMessage::handleGetClassRoomList : SQLException = %s\n", e.what());
                SINGLE->bufpool.free(p);
                return;
        }

        Buf* pBuf = packet_list(ST_GetClassRoomList, tmp, p->getfd());
        CHECK_BUF(pBuf, p);
        SINGLE->sendqueue.enqueue(pBuf);

        SINGLE->bufpool.free(p);
        return;
}
