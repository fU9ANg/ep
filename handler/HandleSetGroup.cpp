#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"
#include "../netdef.h"

#include "../content/epManager.h"
#include "../content/epGroup.h"

void CHandleMessage::handleSetGroup (Buf* p) {
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_SetGroup\n";
#endif

        // 该FD必须处于上课状态。
        epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
        CHECK_P(pClassroom);

        int result = TRUE;
        std::string msg = "success!";

        epGroup* pGroup = new epGroup();
        if (NULL == pGroup) {
                printf("[DEBUG] %s : NULL == pGroup\n", __func__);
                result = FALSE;
                msg = "fail!";
                RETURN(p);
        }

        /*
        pGroup->id_ = *(int*)((char*)p->ptr() + MSG_HEAD_LEN);
        pGroup->name_ = (char*)p->ptr() + MSG_HEAD_LEN + sizeof(int);

        int student_id = 0;
        epStudent* pStudent = NULL;
        int cnt = *(int*)((char*)p->ptr() + MSG_HEAD_LEN + sizeof(int) + GROUP_NAME_LEN);
        printf("[DEBUG] CHandleMessage::handleSetGroup : cnt = %d\n", cnt);
        char* pc = (char*)p->ptr() + MSG_HEAD_LEN + sizeof(int) + GROUP_NAME_LEN + sizeof(int);
        for (int i=0; i<cnt; ++i) {
                student_id = *(int*)(pc+sizeof(int)*i);
                printf("[DEBUG] CHandleMessage::handleSetGroup : student_id  = %d\n", student_id);
                pStudent = const_cast<epStudent*>(pClassroom->getStudentById(student_id));
                if (NULL != pStudent){
                        printf("[DEBUG] CHandleMessage::handleSetGroup : NULL != pStudent\n");
                        pGroup->insertStudent(pStudent->fd_, pStudent);
                } else {
                        printf("[DEBUG] CHandleMessage::handleSetGroup : NULL == pStudent\n");
                        result = FALSE;
                        // msg = pGroup->getId() + "组的" + *(int*)(pc+i*sizeof(int)) + std::string("号学生不在该上课的教室中。");
                        msg = "====fail!";
                        break;
                }

        }
        */

        cSetGroup csg;
        unpacket(p, csg);
        pGroup->id_ = csg.group_id();
        printf("[DEBUG] CHandleMessage::handleSetGroup : group id = %d\n", pGroup->id_);
        pGroup->name_ = csg.group_name();

        PeerGroup pg;
        epStudent* pStudent = NULL;
        printf("[DEBUG] CHandleMessage::handleSetGroup : student_list_size = %d\n", csg.student_list_size());
        for (int i=0; i<csg.student_list_size(); ++i) {
                pg = csg.student_list(i);
                int student_id = pg.student_id();
                printf("[DEBUG] CHandleMessage::handleSetGroup : student_id  = %d\n", student_id);
                pStudent = const_cast<epStudent*>(pClassroom->getStudentById(student_id));
                if (NULL != pStudent){
                        printf("[DEBUG] CHandleMessage::handleSetGroup : NULL != pStudent\n");
                        if (pGroup->insertStudent(pStudent->fd_, pStudent)) {
                                printf("[DEBUG] CHandleMessage::handleSetGroup : SetGroup success!\n");
                        } else {
                                printf("[DEBUG] CHandleMessage::handleSetGroup : SetGroup fail!\n");
                        }
                } else {
                        printf("[DEBUG] CHandleMessage::handleSetGroup : NULL == pStudent\n");
                        result = FALSE;
                        // msg = pGroup->getId() + "组的" + *(int*)(pc+i*sizeof(int)) + std::string("号学生不在该上课的教室中。");
                        msg = "====fail!";
                        break;
                }
        }

        pClassroom->insertGroup(pGroup);

        if (!result) { // 分组失败，将之前可能的分组清除。
                printf("[DEBUG] CHandleMessage::handleSetGroup : result = %s\n", result ? "true" : "false");
                pClassroom->deleteAllGroup();
        }

        sSetGroup tmp;
        tmp.set_result(result);
        tmp.set_msg(msg);

        Buf* pBuf = packet(ST_SetGroup, tmp, p->getfd());
        CHECK_P(pBuf);
        SINGLE->sendqueue.enqueue(pBuf);

        RETURN(p);
}
