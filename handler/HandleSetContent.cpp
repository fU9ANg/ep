#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"
#include "Single.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epClassroom.h"
#include "../content/epManager.h"
#include "../netdef.h"

void CHandleMessage::handleSetContent (Buf* p)
{
        /**
         * @brief 教师设置上课信息。
         *        1. 检查该用户类型是否为教师。
         *        2. 查看该教师所选教室是否已经被使用。
         *        3. 查看该教师所选班级是否已经在上课。
         *        4. 查看该教师是否不处理游离状态，上课或者没有登录。
         *        5. 将班级加入到教室。
         *        6. 将属于该班的学生加入到上课的班级中。
         *        7. 将教室加入到已经使用教室的列表中。
         *        8. 将教师加入到教室中。
         */

#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_SetContent\n";
#endif

        const epUser* user = EPMANAGER->getUserByFd(p->getfd());
        if (NULL == user) {
                printf("[DEBUG] %s : NULL == pUser\n", __func__);
                SINGLE->bufpool.free(p);
                return;
        }

        const epTeacher* pTeacher = dynamic_cast<const epTeacher*>(user);
        if (NULL == pTeacher) {
                printf("[DEBUG] %s : NULL == pTeacher\n", __func__);
                SINGLE->bufpool.free(p);
                return;
        }

        cSetContent sc;
        if (!unpacket(p, sc)) { // 解包失败。
#ifdef __DEBUG__
                printf("[DEBUG] %s : unpacket fail!\n", __func__);
#endif
                SINGLE->bufpool.free(p);
                return;
        }
        int classroom_id = sc.classroom_id();

        sSetContent tmp;
        epClassroom* pClassroom = EPMANAGER->getClassroomById(sc.classroom_id());
        if (NULL != pClassroom) { // 该教室已经占用。
                printf("[DEBUG] CHandleMessage::handleSetContent : NULL != pClassroom\n");
                tmp.set_result(FALSE);
                tmp.set_msg("该教室已经被使用！");
                Buf* pBuf = packet(ST_SetContent, tmp, p->getfd());
                if (NULL != pBuf) {
                        SINGLE->sendqueue.enqueue(pBuf);
                }
                SINGLE->bufpool.free(p);
                return;
        }

        epClass* pClass = EPMANAGER->getClassById(sc.class_id());
        if (NULL != pClass) { // 该班已经在上课了。
                printf("[DEBUG] CHandleMessage::handleSetContent : NULL != pClass\n");
                tmp.set_result(FALSE);
                tmp.set_msg("该班已经在上课了！");
                Buf* pBuf = packet(ST_SetContent, tmp, p->getfd());
                if (NULL != pBuf) {
                        SINGLE->sendqueue.enqueue(pBuf);
                }
                SINGLE->bufpool.free(p);
                return;
        }


        // TODO : add whiteboard into classroom

        pClassroom = new epClassroom();
        pClassroom->teacher_ = const_cast<epTeacher*>(pTeacher);
        EPMANAGER->removeUserByFd(p->getfd());

        pClass     = new epClass();
        pClass->id_= sc.class_id();
        printf("[DEBUG] CHandleMessage::handleSetContent : class id = %d\n", pClass->id_);
        pClassroom->insertClass(pClass);

        // insert classroom
        pClassroom->id_ = sc.classroom_id();
        EPMANAGER->insertClassroom(pClassroom);

        EPMANAGER->insertStudentFromUserIntoClassroom(classroom_id);

        /*
        Buf* pBuf_startClass = packet(ST_StartClass, p->getfd());
        if (NULL != pBuf_startClass) {
                pClass->sendtoAllStudent(pBuf_startClass);
        }
        */

        // 点亮学生端的功能选项的教室。
        Buf* pBuf_EnableClassroom = packet(ST_EnableClassroom, p->getfd());
        CHECK_BUF(pBuf_EnableClassroom, p);
        EPMANAGER->sendtoClassFromUser(pBuf_EnableClassroom, sc.class_id());

        tmp.set_result(true);
        tmp.set_msg("课程设置成功。！");
#ifdef __DEBUG_DUMP__
        EPMANAGER->dumpClassroom();
#endif

        // 保存课程列表。
        pClassroom->courseList_ = ((char*)p->ptr() + MSG_HEAD_LEN + sizeof(int));

        Buf* pBuf = packet(ST_SetContent, tmp, p->getfd());
        CHECK_BUF(pBuf, p);
        SINGLE->sendqueue.enqueue(pBuf);

        SINGLE->bufpool.free(p);
        return;
}
