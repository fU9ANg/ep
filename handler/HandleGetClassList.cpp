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
                SINGLE->bufpool.free(p);
                return;
        }

        // 必须是教师才能使用该协议。
        const epTeacher* pTeacher = dynamic_cast<const epTeacher*>(pUser);
        if (NULL == pTeacher) {
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

        int grade_id = gcrl.grade_id();
        sGetClassList tmp;
        std::vector<sGetClassList> vc;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_CLASS_LIST_BY_GRADE_ID);
                pstmt->setInt (1, grade_id);
                ResultSet* prst = pstmt->executeQuery ();
                while (prst->next ()) {
                        tmp.set_class_id (prst->getInt   ("class_id"));
                        tmp.set_class_name(prst->getString("class_name"));
                        vc.push_back(tmp);
                }
                delete prst;
                delete pstmt;
        }catch (SQLException e) {
                printf("[DEBUG] CHandleMessage::handleGetClassList : SQLException = %s\n", e.what());
                SINGLE->bufpool.free(p);
                return;
        }

        for (int i=0; i<(signed)vc.size(); ++i) {
                Buf* pBuf = packet(ST_GetClassList, vc[i], p->getfd());
                if (NULL != pBuf) {
                        *(int*)((char*)pBuf->ptr() + MSG_HEAD_LEN) = vc.size();
                        SINGLE->sendqueue.enqueue(pBuf);
                }
        }

        SINGLE->bufpool.free(p);
}
