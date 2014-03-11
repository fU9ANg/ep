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

        cGetStudentInfo gsi;
        UNPACKET(p, gsi);

        sGetStudentInfo tmp;
        const epStudent* pStudent = EPMANAGER->getStudentById(gsi.id());
        if (NULL == pStudent) { // 没有该学生。到数据库中进行查找。
                string strpwd;
                string Account;
                try {
                        MutexLockGuard guard(DATABASE->m_mutex);
                        PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_STU_INFO_BY_ID);
                        pstmt->setInt (1, gsi.id());
                        printf("[DEBUG] CHandleMessage::handleGetStudentInfo : student_id = %d\n", gsi.id());
                        ResultSet* prst = pstmt->executeQuery ();
                        while (prst->next ()) {
                                tmp.set_id             (prst->getInt   ("student_id"));
                                tmp.set_number         (prst->getString("number"));
                                tmp.set_name           (prst->getString("last_name") + prst->getString("first_name"));
                                tmp.set_sex            (prst->getString("sex"));
                                tmp.set_race           (prst->getString("race_name"));
                                tmp.set_birthday       (prst->getString("birthday"));
                                tmp.set_native         (prst->getString("native_name"));
                                tmp.set_class_id       (prst->getInt   ("class_id"));
                                tmp.set_class_name     (prst->getString("class_name"));
                                tmp.set_grade_id       (prst->getInt   ("grade_id"));
                                tmp.set_grade_name     (prst->getString("grade_name"));
                                tmp.set_guardian       (prst->getString("guard_last_name") + prst->getString("guard_first_name"));
                                tmp.set_guardian_phone (prst->getString("telephone"));
                                tmp.set_account        (prst->getString("account"));
                                tmp.set_address        (prst->getString("address"));
                        }
                        delete prst;
                        delete pstmt;
                }catch (SQLException e) {
                        printf("[DEBUG] CHandleMessage::handleGetStudentInfo : SQLException = %s\n", e.what());
                        SINGLE->bufpool.free(p);
                        return;
                }
        } else {
                printf("[DEBUG] NULL != pStudent\n");
                tmp.set_id             (pStudent->id_);
                tmp.set_number         (pStudent->studentNum_);
                tmp.set_name           (pStudent->lastName_ + pStudent->firstName_);
                tmp.set_sex            (pStudent->sex_);
                tmp.set_race           (pStudent->race_);
                tmp.set_birthday       (pStudent->birthday_);
                tmp.set_native         (pStudent->native_);
                tmp.set_class_id       (pStudent->classId_);
                tmp.set_class_name     (pStudent->className_);
                tmp.set_grade_id       (pStudent->gradeId_);
                tmp.set_grade_name     (pStudent->gradeName_);
                tmp.set_guardian       (pStudent->guardian_);
                tmp.set_guardian_phone (pStudent->guardian_phone_);
                tmp.set_account        (pStudent->account_);
                tmp.set_address        (pStudent->address_);

#ifdef __DEBUG__
                printf("[DEBUG] CHandleMessage::handleGetStudentInfo : packet before id = %d\n", tmp.id());
                printf("[DEBUG] CHandleMessage::handleGetStudentInfo : packet before number = %s\n", tmp.number().c_str());
                printf("[DEBUG] CHandleMessage::handleGetStudentInfo : packet before name = %s\n", tmp.name().c_str());
                printf("[DEBUG] CHandleMessage::handleGetStudentInfo : packet before sex = %s\n", tmp.sex().c_str());
                printf("[DEBUG] CHandleMessage::handleGetStudentInfo : packet before race = %s\n", tmp.race().c_str());
                printf("[DEBUG] CHandleMessage::handleGetStudentInfo : packet before birthday = %s\n", tmp.birthday().c_str());
                printf("[DEBUG] CHandleMessage::handleGetStudentInfo : packet before native = %s\n", tmp.native().c_str());
                printf("[DEBUG] CHandleMessage::handleGetStudentInfo : packet before class_id = %d\n", tmp.class_id());
                printf("[DEBUG] CHandleMessage::handleGetStudentInfo : packet before class_name = %s\n", tmp.class_name().c_str());
                std::cout << "[DEBUG] CHandleMessage::handleGetStudentInfo : packet before address = " << tmp.address() << std::endl;
#endif
        }

//        printf("[DEBUG] CHandleMessage::handleGetStudentInfo : packet before class_id = %d\n", tmp.class_id());
        Buf* pBuf = packet(ST_GetStudentInfo, tmp, p->getfd());
        CHECK_P(pBuf);
                /*
                sGetStudentInfo sgsl;
                unpacket(pBuf, sgsl);
#ifdef __DEBUG__
                printf("[DEBUG] CHandleMessage::handleGetStudentInfo : id = %d\n", sgsl.id());
                printf("[DEBUG] CHandleMessage::handleGetStudentInfo : number = %s\n", sgsl.number().c_str());
                printf("[DEBUG] CHandleMessage::handleGetStudentInfo : name = %s\n", sgsl.name().c_str());
                printf("[DEBUG] CHandleMessage::handleGetStudentInfo : sex = %s\n", sgsl.sex().c_str());
                printf("[DEBUG] CHandleMessage::handleGetStudentInfo : race = %s\n", sgsl.race().c_str());
                // printf("[DEBUG] CHandleMessage::handleGetStudentInfo : birthday = %s\n", sgsl.birthday().c_str());
                printf("[DEBUG] CHandleMessage::handleGetStudentInfo : native = %s\n", sgsl.native().c_str());
                printf("[DEBUG] CHandleMessage::handleGetStudentInfo : class_id = %d\n", sgsl.class_id());
                printf("[DEBUG] CHandleMessage::handleGetStudentInfo : class_name = %s\n", sgsl.class_name().c_str());
#endif
*/
                SINGLE->sendqueue.enqueue(pBuf);

                RETURN(p);
}
