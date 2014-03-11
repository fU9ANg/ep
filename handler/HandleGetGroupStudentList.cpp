#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"

void CHandleMessage::handleGetGroupStudentList(Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetGroupStudentList\n";
#endif
        epClassroom* p_classroom = EPMANAGER->getClassroomByFd(p->getfd());
        CHECK_P(p_classroom);

        cGetGroupStudentList cggsl;
        UNPACKET(p, cggsl);

        epGroup* p_group = NULL;
        if (cggsl.has_group_id())
                p_group = const_cast<epGroup*>(p_classroom->getGroupById(cggsl.group_id()));
        else
                p_group = const_cast<epGroup*>(p_classroom->getGroupByFd(p->getfd()));
        CHECK_P(p_group);

        p_group->cleanLock();
        std::cout << "[DEBUG] CHandleMessage::handleGetGroupStudentList : group_id = " << p_group->id_ << std::endl;

        sGetGroupStudentList tmp;
        if (0 != p_group->id_)
                tmp.set_group_id(p_group->id_);
        std::cout << "[DEBUG] CHandleMessage::handleGetGroupStudentList ：group_id = " << tmp.group_id() << std::endl;
        std::vector<int> vi = p_group->getStudentList();
        for (int i=0; i<(signed int)vi.size(); ++i) {
                printf("[DEBUG] CHandleMessage::handleGetGroupStudentList : student_id = %d\n", vi[i]);
                tmp.add_student_list(vi[i]);
        }

        Buf* pBuf = packet_list(ST_GetGroupStudentList, tmp, p->getfd());
        CHECK_P(pBuf);
        SINGLE->sendqueue.enqueue(pBuf);

        RETURN(p);
}
