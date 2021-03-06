#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"
#include "Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"

void CHandleMessage::handleGetTeacherInfo (Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetTeacherInfo\n";
#endif
        cGetTeacherInfo gti;
        UNPACKET(p, gti);

        sGetTeacherInfo tmp;
        int teacher_id = gti.id();
        if (EPTEACHER_INVALID_TEACHER_ID == teacher_id) {
                epClassroom* p_classroom = EPMANAGER->getClassroomByFd(p->getfd());
                if (NULL != p_classroom) {
                        if (NULL != p_classroom->teacher_) {
                                teacher_id = p_classroom->teacher_->id_;
                        }
                }
        }

        /*
           const epTeacher* pTeacher = EPMANAGER->getTeacherById(gti.id());
           if (NULL == pTeacher) { // 没有找到教师信息。到数据库中进行查找。
           */
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_TEA_INFO_BY_ID);
                pstmt->setInt (1, teacher_id);
                printf("[DEBUG] CHandleMessage::handleGetTeacherInfo : teacher_id = %d\n", gti.id());
                ResultSet* prst = pstmt->executeQuery ();
                while (prst->next ()) {
                        tmp.set_id         (prst->getInt   ("teacher_id"));
                        tmp.set_number     (prst->getString("number"));
                        tmp.set_name       (prst->getString("last_name") + prst->getString("first_name"));
                        tmp.set_sex        (prst->getString("sex"));
                        tmp.set_race       (prst->getString("race_name"));
                        tmp.set_birthday   (prst->getString("native_name"));
                        tmp.set_native     (prst->getString("native_name"));
                        tmp.set_school_name(prst->getString("school_name"));
                        tmp.set_res_path   (prst->getString("res_path"));
                        printf("[DEBUG] CHandleMessage::handleGetTeacherInfo : res_path = %s\n", tmp.res_path().c_str());
                }
                delete prst;
                delete pstmt;
        }catch (SQLException e) {
                printf("[INFO] CHandleMessage::handleGetTeacherInfo : SQLException = %s\n", e.what());
                SINGLE->bufpool.free(p);
                return;
        }
        /*
           } else {
           tmp.set_id        (pTeacher->getId());
           tmp.set_number    (pTeacher->getTeacherNum());
           tmp.set_name      (pTeacher->getName());
           tmp.set_sex       (pTeacher->getSex());
           tmp.set_race      (pTeacher->getRace());
           tmp.set_birthday  (pTeacher->getBirthday());
           tmp.set_native    (pTeacher->getNative());
           tmp.set_school_name(pTeacher->getSchoolName());
           }
           */

        Buf* pBuf = packet(ST_GetTeacherInfo, tmp, p->getfd());
        CHECK_P(pBuf);
        SINGLE->sendqueue.enqueue(pBuf);

        RETURN(p);
}
