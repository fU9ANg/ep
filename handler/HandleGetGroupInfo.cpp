#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"

void CHandleMessage::handleGetGroupInfo (Buf* p) 
{
#ifdef __DEBUG_HANDLE_HEAD_
    cout << "CT_GetGroupInfo\n";
#endif

    const epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
    CHECK_P(pClassroom);

    group_node gn;
    UNPACKET(p, gn);

    const epGroup* p_group = gn.has_group_id()
            ? const_cast<epClassroom*>(pClassroom)->getGroupById(gn.group_id())
            : const_cast<epClassroom*>(pClassroom)->getGroupByFd(p->getfd());
    CHECK_P(p_group);

    group_node tmp;
    if (0 != p_group->id_)
            tmp.set_group_id(p_group->id_);
    printf("[DEBUG] CHandleMessage::handleGetGroupInfo : group_id = %d\n", p_group->id_);

    Buf* pBuf = packet_list(ST_GetGroupInfo, tmp, p->getfd());
    CHECK_P(pBuf);
    SINGLE->sendqueue.enqueue(pBuf);

    RETURN(p);
}

