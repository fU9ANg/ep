#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../protocol.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../content/epUser.h"
#include "../netdef.h"

void CHandleMessage::handleSetFunc (Buf* p) {
        /**
         * @brief 用户设置功能列表。
         *        1. 用户发送所选择的功能ID。
         *        2. 返回设置该功能成功与否。
         *        3. 如果该用户类型为学生，并选择了学校功能，则：
         *              1）如果该学生所在班级正在上课，则将学生由游离列表加入到上课的班级。
         *              2）将该学生上线状态通知该班其它上线学生，上课教师，所在教室的白板。
         *              3）将学生从游离列表中删除。
         */

#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_SetFunc\n";
#endif
        // TODO:

        cSetFunc setFunc;
        if (!unpacket(p, setFunc)) { // 解包失败。
#ifdef __DEBUG__
                printf("[DEBUG] %s : unpacket fail!\n", __func__);
#endif
                SINGLE->bufpool.free(p);
                return;
        }

        sSetFunc tmp;
        epUser* pUser = const_cast<epUser*>(EPMANAGER->getUserByFd(p->getfd()));
        if (NULL == pUser) {
                printf("[DEBUG] CHandleMessage::handleSetFunc : NULL == pUser\n");
                SINGLE->bufpool.free(p);
                return;
        }

        printf("[DEBUG] CHandleMessage::handleSetFunc : setFunc.func_type() = %d\n", setFunc.func_type());
        pUser->funcType_ = (enum FuncType)setFunc.func_type();
        tmp.set_result(true);
        Buf* pBuf = packet(ST_SetFunc, tmp, p->getfd());
        CHECK_BUF(pBuf, p);
        SINGLE->sendqueue.enqueue(pBuf);

        epStudent*   pStudent   = NULL;
        epClassroom* pClassroom = NULL;
        Buf* p1 = NULL;
        sUpdateStudentStatus suss;
        switch (pUser->getType()) {
        case LT_STUDENT :
                pStudent = dynamic_cast<epStudent*>(pUser);
                if (NULL == pStudent) {
                        printf("[DEBUG] %s : NULL == pStudent\n", __func__);
                        SINGLE->bufpool.free(p);
                        return;
                }

                switch (setFunc.func_type()) {
                case FT_SCHOOL :
                        printf("[DEBUG] CHandleMessage::handleSetFunc : FT_SCHOOL : class id = %d\n", pStudent->classId_);
                        pClassroom = EPMANAGER->getClassroomByClassId(pStudent->classId_);
                        if (NULL == pClassroom) {
                                printf("[DEBUG] %s : NULL == pClassroom\n", __func__);
                                SINGLE->bufpool.free(p);
                                return;
                        }

                        suss.set_student_id(pStudent->id_);

                        printf("[DEBUG] CHandleMessage::handleSetFunc : send ST_UpdateStudentStatus\n");
                        p1 = packet(ST_UpdateStudentStatus, suss, p->getfd());
                        CHECK_BUF(p1, p);
                        pClassroom->sendtoAll(p1);

                        // 将学生由游离状态加入该班级。
                        pClassroom->insertStudent(pStudent);
                        // 将该学生从游离列表移除。
                        EPMANAGER->removeUserByFd(p->getfd());
                        break;
                default :
                        break;
                }
                break;
        default :
                break;
        }

#ifdef __DEBUG_DUMP__
        EPMANAGER->dumpClassroom();
#endif

        SINGLE->bufpool.free(p);
        return;
}
