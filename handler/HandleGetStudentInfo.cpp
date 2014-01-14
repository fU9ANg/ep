#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"
#include "Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"
#include "../netdef.h" // for EPCLASSROOM_INVALID_CLASSROOM_ID

#include "../content/epManager.h"

void CHandleMessage::handleGetStudentInfo (Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetStudentInfo\n";
#endif
        // TODO:

        cGetStudentInfo gsi;
        if (!unpacket(p, gsi)) { // 解包失败。
#ifdef __DEBUG__
                printf("[DEBUG] %s : unpacket fail!\n", __func__);
#endif
                SINGLE->bufpool.free(p);
                return;
        }

        sGetStudentInfo tmp;
        const epStudent* pStudent = EPMANAGER->getStudentById(gsi.id());
        if (NULL == pStudent) { // 没有该学生。到数据库中进行查找。
                string strpwd;
                string Account;
                try {
                        MutexLockGuard guard(DATABASE->m_mutex);
                        PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_STU_INFO_BY_ID);
                        pstmt->setInt (1, gsi.id());
                        ResultSet* prst = pstmt->executeQuery ();
                        while (prst->next ()) {
                                tmp.set_id        (prst->getInt("student_id"));
                                tmp.set_number    (prst->getString("number"));
                                tmp.set_name      (prst->getString("last_name") + prst->getString("first_name"));
                                tmp.set_sex       (prst->getString("sex"));
                                tmp.set_race      (prst->getString("race_name"));
                                tmp.set_birthday  (prst->getString("native_name"));
                                tmp.set_native    (prst->getString("native_name"));
                                tmp.set_class_name(prst->getString("class_name"));
                        }
                        delete prst;
                        delete pstmt;
                }catch (SQLException e) {
                }
        } else {
                tmp.set_id        (pStudent->getId());
                tmp.set_number    (pStudent->getStudentNum());
                tmp.set_name      (pStudent->getName());
                tmp.set_sex       (pStudent->getSex());
                tmp.set_race      (pStudent->getRace());
                tmp.set_birthday  (pStudent->getBirthday());
                tmp.set_native    (pStudent->getNative());
                tmp.set_class_name(pStudent->getClassName());
        }

        Buf* pBuf = packet(ST_GetStudentInfo, tmp, p->getfd());
        if (NULL != pBuf) {
                SINGLE->sendqueue.enqueue(pBuf);
        }
        SINGLE->bufpool.free(p);
}
