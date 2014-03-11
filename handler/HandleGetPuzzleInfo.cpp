#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"

void CHandleMessage::handleGetPuzzleInfo(Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetPuzzleInfo\n";
#endif

        epClassroom* p_classroom = EPMANAGER->getClassroomByFd(p->getfd());
        CHECK_P(p_classroom);

        PuzzleInfo pi;
        UNPACKET(p, pi);

        const epGroup* p_group = pi.has_group_id()
                ? p_classroom->getGroupById(pi.group_id())
                : p_classroom->getGroupByFd(p->getfd());
        CHECK_P(p_group);

        std::cout << "CHandleMessage::handleGetPuzzleInfo : " << "group_id  = " << pi.group_id()  << std::endl;
        std::cout << "CHandleMessage::handleGetPuzzleInfo : " << "puzzle_id = " << pi.puzzle_id() << std::endl;

        PuzzleInfo tmp;
        int random = p_group->random_ + pi.puzzle_id();
        if (0 != random)
                tmp.set_random(random);
        if (0 != p_group->id_)
                tmp.set_group_id(p_group->id_);

        tmp.set_puzzle_id(pi.puzzle_id());

        Buf* pBuf = packet(ST_GetPuzzleInfo, tmp, p->getfd());
        CHECK_P(pBuf);
        SINGLE->sendqueue.enqueue(pBuf);

        RETURN(p);
}
