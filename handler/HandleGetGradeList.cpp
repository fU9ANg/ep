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

void CHandleMessage::handleGetGradeList (Buf* p)
{
        /**
         * @brief 教师请求其所在学校的年级列表。
         *        1. 查检该用户类型是否为教师。
         *        2. 如果是教师，则根据该教室所在学校，查找数据库，返回年级列表。
         */

#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetGradeList\n";
#endif
        // TODO:

        // 必须是处于游离状态的用户才能作用该协议。
        const epUser* pUser = EPMANAGER->getUserByFd(p->getfd());
        if (NULL == pUser) {
                printf("[DEBUG] %s : NULL == pUser\n", __func__);
                SINGLE->bufpool.free(p);
                return;
        }

        // 必须是教室才能使用该协议。
        const epTeacher* pTeacher = dynamic_cast<const epTeacher*>(pUser);
        if (NULL == pTeacher) {
                printf("[DEBUG] %s : NULL == pTeacher\n", __func__);
                SINGLE->bufpool.free(p);
                return;
        }

        sGetGradeList tmp;
        ClassListNode* cln;
#ifdef  __DEBUG__
        int cnt = 0;
#endif
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_ALL_GRADE_LIST_BY_SCHOOL_ID);
                pstmt->setInt (1, pTeacher->schoolId_);
                ResultSet* prst = pstmt->executeQuery ();
                while (prst->next ()) {
                        cln = tmp.add_grade_list();
                        cln->set_id  (prst->getInt   ("grade_id"));
                        cln->set_name(prst->getString("grade_name"));
#ifdef __DEBUG__
                        ++cnt;
#endif
                }
                delete prst;
                delete pstmt;
        }catch (SQLException e) {
                printf("[DEBUG] %s : %s\n", __func__, e.what());
                SINGLE->bufpool.free(p);
                return;
        }

#ifdef __DEBUG__
        printf("[DEBUG] CHandleMessage::handleGetGradeList : grade cnt = %d\n", cnt);
#endif
        Buf* pBuf = packet_list(ST_GetGradeList, tmp, p->getfd());
        CHECK_BUF(pBuf, p);
        SINGLE->sendqueue.enqueue(pBuf);

        SINGLE->bufpool.free(p);
        return;
}
