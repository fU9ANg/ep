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

#define CHECK_USER(obj, from, by, opt) \
        e##obj* obj = EPMANAGER->get##obj##From##fromBy##by(opt); \
        if (NULL == obj) { \
                printf("[DEBUG] %s : NULL == "#obj, __func__); \
                SINGLE->bufpool.free(p); \
                return; \
        }

void CHandleMessage::handleGetCourseList (Buf* p)
{
        /**
         * @brief 教师请求所选年级的课程列表。
         *        1. 检查用户类型是否为教师。
         *        2. 根据所选择的年级ID，查找数据库，返回该年级所对应的课程列表。
         */
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetCourseList\n";
#endif

        const epUser* pUser = EPMANAGER->getUserByFd(p->getfd());
        if (NULL == pUser) {
                printf("[DEBUG] %s : NULL == pUser\n", __func__);
                SINGLE->bufpool.free(p);
                return;
        }

        const epTeacher* pTeacher = dynamic_cast<const epTeacher*>(pUser);
        if (NULL == pTeacher) {
                printf("[DEBUG] %s : NULL == pTeacher\n", __func__);
                SINGLE->bufpool.free(p);
                return;
        }

        cGetCourseList gcl;
        if (!unpacket(p, gcl)) { // 解包失败。
#ifdef __DEBUG__
                printf("[DEBUG] %s : unpacket fail!\n", __func__);
#endif
                SINGLE->bufpool.free(p);
                return;
        }

        int grade_id = gcl.grade_id();
        sGetCourseList tmp;
        CourseNode* cn;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_COURSE_LIST_BY_GRADE_ID);
                pstmt->setInt (1, grade_id);
                ResultSet* prst = pstmt->executeQuery ();
                while (prst->next ()) {
                        cn = tmp.add_course_list();
                        /*
                        cn->set_course_id  (prst->getInt   ("course_id"));
                        cn->set_course_name(prst->getString("course_name"));
                        cn->set_grade_id   (grade_id);
                        cn->set_language   (prst->getInt   ("language"));
                        cn->set_art        (prst->getInt   ("art"));
                        cn->set_community  (prst->getInt   ("community"));
                        cn->set_health     (prst->getInt   ("health"));
                        cn->set_science    (prst->getInt   ("science"));
                        cn->set_course_type((CourseNode_CourseType)prst->getInt("course_type_id"));
                        // cn->set_res_path   (prst->getString("resPath"));
                        cn->set_res_path   ("InvalidPath");
                        */

                        cn->set_course_id  (prst->getInt   ("course_id"));
                        cn->set_course_name(prst->getString("course_name"));
                        cn->set_grade_id   (grade_id);
                        SET_OPTION(cn, Int, language)
                        SET_OPTION(cn, Int, art)
                        SET_OPTION(cn, Int, community)
                        SET_OPTION(cn, Int, health)
                        SET_OPTION(cn, Int, science)
                        cn->set_course_type((CourseNode_CourseType)prst->getInt("course_type_id"));
                        // cn->set_res_path   (prst->getString("resPath"));
                        cn->set_res_path   ("InvalidPath");
                        /*
                        std::string xml_path = prst->getString("xml_path");
                        if (xml_path.size() > 0) {
                                cn->set_xml_path(sml_path);
                        }
                        */
                        cn->set_xml_path(prst->getString("xmlPath"));
                }
                delete prst;
                delete pstmt;
        } catch (SQLException e) {
                printf("[DEBUG] CHandleMessage::handleGetCourseList : SQLException = %s\n", e.what());
                SINGLE->bufpool.free(p);
                return;
        }

        CourseNode cn1;
        for (int i=0; i<tmp.course_list_size(); ++i) {
                cn1 = tmp.course_list(i);
                printf("[DEBUG] CHandleMessage::handleGetCourseList : course_id = %d\n", cn1.course_id());
                printf("[DEBUG] CHandleMessage::handleGetCourseList : course_name = %s\n", cn1.course_name().c_str());
                printf("[DEBUG] CHandleMessage::handleGetCourseList : grade_id = %d\n", cn1.grade_id());
                printf("[DEBUG] CHandleMessage::handleGetCourseList : language = %d\n", cn1.language());
                printf("[DEBUG] CHandleMessage::handleGetCourseList : art = %d\n", cn1.art());
                printf("[DEBUG] CHandleMessage::handleGetCourseList : community = %d\n", cn1.community());
                printf("[DEBUG] CHandleMessage::handleGetCourseList : health = %d\n", cn1.health());
                printf("[DEBUG] CHandleMessage::handleGetCourseList : science = %d\n", cn1.science());
                printf("[DEBUG] CHandleMessage::handleGetCourseList : course_type = %d\n", cn1.course_type());
                printf("[DEBUG] CHandleMessage::handleGetCourseList : res_path = %s\n", cn1.res_path().c_str());
                printf("[DEBUG] CHandleMessage::handleGetCourseList : xml_path = %s\n", cn1.xml_path().c_str());
        }

        Buf* pBuf = packet_list(ST_GetCourseList, tmp, p->getfd());
        CHECK_BUF(pBuf, p);
        SINGLE->sendqueue.enqueue(pBuf);

        SINGLE->bufpool.free(p);
        return;
}
