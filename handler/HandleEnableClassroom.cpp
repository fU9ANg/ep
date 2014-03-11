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
                epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
                CHECK_P(pClassroom);
        } else {
                epStudent* pStudent = dynamic_cast<epStudent*>(pUser);
                CHECK_P(pStudent);

                epClassroom* pClassroom = EPMANAGER->getClassroomByClassId(pStudent->classId_);
                CHECK_P(pClassroom);
        }

        // 如果该学生所在班级正在上课，则将功能选择的教室按钮点亮。
        Buf* pBuf_EnableClassroom = packet(ST_EnableClassroom, p->getfd());
        CHECK_BUF(pBuf_EnableClassroom, p);
        SINGLE->sendqueue.enqueue(pBuf_EnableClassroom);

        RETURN(p);
}
