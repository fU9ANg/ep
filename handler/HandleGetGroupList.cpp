#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"

void CHandleMessage::handleGetGroupList (Buf* p) 
{
#ifdef __DEBUG_HANDLE_HEAD_
    cout << "CT_GetGroupList\n";
#endif

    epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
    CHECK_P(pClassroom);

    std::vector<int> vi = pClassroom->getGroupList();
    sGetGroupList tmp;
    for(int i=0; i<(signed)vi.size(); ++i) {
        tmp.add_group_list(vi[i]);
    }

    Buf* pBuf = packet_list(ST_GetGroupList, tmp, p->getfd());
    CHECK_P(pBuf);
    SINGLE->sendqueue.enqueue(pBuf);

    RETURN(p);
}

