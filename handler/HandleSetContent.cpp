#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"
#include "Single.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epClassroom.h"
#include "../content/epManager.h"

void CHandleMessage::handleSetContent (Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_SetContent\n";
#endif
        // TODO:

        cSetContent sc;
        unpacket(p, sc);
        int class_id = sc.class_id();
        int classroom_id = sc.classroom_id();

        sSetContent tmp;
        epClassroom* pClassroom = EPMANAGER->getClassroomById(sc.classroom_id());
        if (NULL != pClassroom) { // 该教室已经占用。
                tmp.set_result(false);
                tmp.set_msg("该教室已经被使用！");
                SINGLE->sendqueue.enqueue(packet(ST_SetContent, tmp, p->getfd()));
                SINGLE->bufpool.free(p);
        }

        epClass* pClass = EPMANAGER->getClassById(sc.class_id());
        if (NULL != pClass) { // 该班已经在上课了。
                tmp.set_result(false);
                tmp.set_msg("该班已经在上课了！");
                SINGLE->sendqueue.enqueue(packet(ST_SetContent, tmp, p->getfd()));
                SINGLE->bufpool.free(p);
        }

        epUser* pUser = EPMANAGER->getUserByFd(p->getfd());
        epTeacher* pTeacher = NULL;
        if (NULL == pUser || NULL == (pTeacher=dynamic_cast<epTeacher*>(pUser))) { // 该教师已经上课或没有登录。
                tmp.set_result(false);
                tmp.set_msg("该教师已经上课或没有登录！");
                SINGLE->sendqueue.enqueue(packet(ST_SetContent, tmp, p->getfd()));
                SINGLE->bufpool.free(p);
        }

        pClassroom = new epClassroom(sc.classroom_id());
        pClass     = new epClass(sc.class_id());

        pClassroom->setTeacher(*pTeacher);
        pClassroom->insertClass(*pClass);
        EPMANAGER->insertClassroom(*pClassroom);
}
