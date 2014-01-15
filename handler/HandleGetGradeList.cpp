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
                SINGLE->bufpool.free(p);
                return;
        }

        // 必须是教室才能使用该协议。
        const epTeacher* pTeacher = dynamic_cast<const epTeacher*>(pUser);
        if (NULL == pTeacher) {
                SINGLE->bufpool.free(p);
                return;
        }

        int school_id = pTeacher->getSchoolId();
        sGetGradeList tmp;
        std::vector<sGetGradeList> vc;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_ALL_GRADE_LIST_BY_SCHOOL_ID);
                pstmt->setInt (1, school_id);
                ResultSet* prst = pstmt->executeQuery ();
                while (prst->next ()) {
                        tmp.set_grade_id(prst->getInt("grade_id"));
                        tmp.set_grade_name(prst->getString("grade_name"));
                        vc.push_back(tmp);
                }
                delete prst;
                delete pstmt;
        }catch (SQLException e) {
                printf("[DEBUG] %s : %s\n", __func__, e.what());
        }

        /*
        for (int i=0; i<(signed)vc.size(); ++i) {
                printf("[INFO] grade_id   = %d\n", vc[i].grade_id());
                printf("[INFO] grade_name = %s\n", vc[i].grade_name().c_str());
        }
        */

        tmp.ByteSize();

        for (int i=0; i<(signed)vc.size(); ++i) {
                Buf* pBuf = packet(ST_GetGradeList, vc[i], p->getfd());
                if (NULL != pBuf) {
                        /*
                           sGetGradeList ggl;
                           std::string tmp = (char*)pBuf->ptr() + MSG_HEAD_LEN + sizeof(int);
                           ggl.ParseFromString(tmp);
                           printf("[INFO] grade_id = %d\n", ggl.grade_id());
                           tmp = (char*)pBuf->ptr() + MSG_HEAD_LEN + sizeof(int) + 128;
                           ggl.ParseFromString(tmp);
                           printf("[INFO] grade_id = %d\n", ggl.grade_id());
                           tmp = (char*)pBuf->ptr() + MSG_HEAD_LEN + sizeof(int) + 128*2;
                           ggl.ParseFromString(tmp);
                           printf("[INFO] grade_id = %d\n", ggl.grade_id());

                           char* pc = (char*)pBuf->ptr() + MSG_HEAD_LEN + sizeof(int);
                           for (int i=0; i<32; ++i) {
                           printf("%0#X ", *(pc+i));
                           }
                           printf("\n");
                           */
                        *(int*)((char*)pBuf->ptr() + MSG_HEAD_LEN) = vc.size();
                        SINGLE->sendqueue.enqueue(pBuf);
                }
        }
        SINGLE->bufpool.free(p);
}
