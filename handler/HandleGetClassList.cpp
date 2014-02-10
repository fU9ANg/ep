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

void CHandleMessage::handleGetClassList (Buf* p)
{
        /**
         * @brief 教师根据所选择的年级ID，请求该年级所对象的班级列表。
         *        1. 检查该用户类型是否为教师。
         *        2. 根据教师所选择的年级ID，查找数据库，返回该年级所对应的班级列表。
         */

#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetClassList\n";
#endif

        // 必须是处于游离状态的用户才能作用该协议。
        const epUser* pUser = EPMANAGER->getUserByFd(p->getfd());
        if (NULL == pUser) {
                printf("[DEBUG] CHandleMessage::HandleMessage : NULL == pUser\n");
                SINGLE->bufpool.free(p);
                return;
        }

        // 必须是教师才能使用该协议。
        const epTeacher* pTeacher = dynamic_cast<const epTeacher*>(pUser);
        if (NULL == pTeacher) {
                printf("[DEBUG] CHandleMessage::HandleMessage : NULL == pTeacher\n");
                SINGLE->bufpool.free(p);
                return;
        }

        cGetClassList gcrl;
        if (!unpacket(p, gcrl)) { // 解包失败。
#ifdef __DEBUG__
                printf("[DEBUG] %s : unpacket fail!\n", __func__);
#endif
                SINGLE->bufpool.free(p);
                return;
        }

        sGetClassList tmp;
        ClassListNode* cln;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_CLASS_LIST_BY_GRADE_ID);
                pstmt->setInt (1, gcrl.grade_id());
                ResultSet* prst = pstmt->executeQuery ();
                while (prst->next ()) {
                        cln = tmp.add_class_list();
                        cln->set_id  (prst->getInt   ("class_id"));
                        cln->set_name(prst->getString("class_name"));
                }
                delete prst;
                delete pstmt;
        } catch (SQLException e) {
                printf("[DEBUG] CHandleMessage::handleGetClassList : SQLException = %s\n", e.what());
                SINGLE->bufpool.free(p);
                return;
        }

        Buf* pBuf = packet_list(ST_GetClassList, tmp, p->getfd());
        CHECK_BUF(pBuf, p);
        SINGLE->sendqueue.enqueue(pBuf);

        SINGLE->bufpool.free(p);
        return;
}
