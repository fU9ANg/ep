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

void CHandleMessage::handleGetFuncList (Buf* p) {

        /**
         * @brief 用户请求功能列表。
         *        1. 检查该用户是否在游离列表中，并有一定的类型（教师，学生等）。
         *        2. 根据用户类型，查找数据库，返回功能列表。
         */

#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetFuncList\n";
#endif
        const epUser* pUser = EPMANAGER->getUserByFd(p->getfd());
        CHECK_P(pUser);

        if (LT_USER == pUser->getType()) {
                DEBUG_INFO;
                RETURN(p);
        }

        sGetFuncList tmp;
        FuncNode* fn;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_FUNC_LIST_BY_TYPE);
                pstmt->setInt (1, pUser->getType());
                ResultSet* prst = pstmt->executeQuery ();
                while (prst->next ()) {
                        fn = tmp.add_func_list();
                        fn->set_id      (prst->getInt("id"));
                        fn->set_name    (prst->getString("name"));
                        fn->set_res_path(prst->getString("res_path"));
                }
                delete prst;
                delete pstmt;
        } catch (SQLException e) {
                printf("[DEBUG] %s : SQLException : %s\n", __func__, e.what());
                SINGLE->bufpool.free(p);
                return;
        }

        epStudent* pStudent = dynamic_cast<epStudent*>(const_cast<epUser*>(pUser));
        if (NULL != pStudent) {
                epClassroom* pClassroom = EPMANAGER->getClassroomByClassId(pStudent->classId_);
                if (NULL != pClassroom) {
                        printf("[DEBUG] CHandleMessage::handleGetFuncList : NULL != pClassroom\n");
                        fn = tmp.add_func_list();
                        fn->set_id      (FT_SCHOOL);
                        fn->set_name    ("教室");
                        fn->set_res_path("abc");
                }
        }

        Buf* pBuf = packet_list(ST_GetFuncList, tmp, p->getfd());
        CHECK_P(pBuf);
        SINGLE->sendqueue.enqueue(pBuf);

        RETURN(p);
}
