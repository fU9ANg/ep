#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"

void CHandleMessage::handleGetActiveStudentList (Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetActiveStudentList fd = " << p->getfd() << endl;
#endif

        cGetActiveStudentList casl;
        UNPACKET(p, casl);

        // const epUser* pUser = EPMANAGER->getUserByFdFromClassroom(p->getfd());
        std::vector<int> vi = EPMANAGER->getActiveStudentListFromClass(casl.class_id());
        sGetActiveStudentList tmp;
        printf("[DEBUG] CHandleMessage::handleGetActiveStudentList ：vi.size() = %ld\n", vi.size());
        for (int i=0; i<(signed)vi.size(); ++i) {
                printf("[DEBUG] CHandleMessage::handleGetActiveStudentList fd[%d]: idx = %d\n", p->getfd(), vi[i]);
                tmp.add_student_list(vi[i]);
        }

        Buf* pBuf = packet_list(ST_GetActiveStudentList, tmp, p->getfd());
        CHECK_P(pBuf);
        SINGLE->sendqueue.enqueue(pBuf);

        RETURN(p);
}
