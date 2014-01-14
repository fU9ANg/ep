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

void CHandleMessage::handleTransferBook(Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_TransferBook\n";
#endif

        cTransferBook ctb;
        if (!unpacket(p, ctb)) {
                printf("[DEBUG] CHandleMessage::handleTransferBook : unpacket fail!\n");
                SINGLE->bufpool.free(p);
                return;
        }

        // 必须是处于游离状态的用户才能作用该协议。
        const epUser* fromUser = EPMANAGER->getUserByFd(p->getfd());
        if (NULL == fromUser) {
                printf("[DEBUG] CHandleMessage::handleTransferBook : NULL == fromUser\n");
                SINGLE->bufpool.free(p);
                return;
        }

#if 0
        // from Teacher and Student
        std::string first_name;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (XXX);
                pstmt->setInt (1, ctb.account());
                ResultSet* prst = pstmt->executeQuery ();
                while (prst->next ()) {
                        first_name = prst->getString("first_name");
                }
                delete prst;
                delete pstmt;
        }catch (SQLException e) {
                printf("[DEBUG] CHandleMessage::handleGetClassList : SQLException = %s\n", e.what());
                SINGLE->bufpool.free(p);
                return;
        }

        bool result = false;
        std::string msg;
        if (0 == first_name.size()) {
                result = false;
                msg = ctb.account + "用户不存在。";
        } else {
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

        Buf* pBuf = packet(ST_GetClassList, vc, p->getfd());
        if (NULL != pBuf) {
                SINGLE->sendqueue.enqueue(pBuf);
        }
        SINGLE->bufpool.free(p);
#endif
}
