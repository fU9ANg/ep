#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"

void CHandleMessage::handleEnableClassroom(Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_EnableClassroom\n";
#endif
        epUser* pUser = const_cast<epUser*>(EPMANAGER->getUserByFd(p->getfd()));
        if (NULL == pUser) {
                printf("[DEBUG] CHandleMessage::handleEnableClassroom : NULL == pUser\n");
                SINGLE->bufpool.free(p);
                return;
        }

        /*
        pUser = const_cast<epUser*>(EPMANAGER->getUserByFdFromClassroom(p->getfd()));
        if (NULL == pUser) { // not in classroom
                printf("[DEBUG] CHandleMessage::handleEnableClassroom : NULL == pUser\n");
                SINGLE->bufpool.free(p);
                return;
        }
        */

        epStudent* pStudent = dynamic_cast<epStudent*>(pUser);
        if (NULL == pStudent) {
                printf("[DEBUG] CHandleMessage::handleEnableClassroom : NULL == pStudent\n");
                SINGLE->bufpool.free(p);
                return;
        }

        epClassroom* pClassroom = EPMANAGER->getClassroomByClassId(pStudent->classId_);
        if (NULL == pClassroom) {
                printf("[DEBUG] CHandleMessage::handleEnableClassroom : NULL == pClassroom\n");
                SINGLE->bufpool.free(p);
                return;
        }

        // 如果该学生所在班级正在上课，则将功能选择的教室按钮点亮。
        Buf* pBuf_EnableClassroom = packet(ST_EnableClassroom, p->getfd());
        CHECK_BUF(pBuf_EnableClassroom, p);
        SINGLE->sendqueue.enqueue(pBuf_EnableClassroom);

        SINGLE->bufpool.free(p);
        return;
}
