#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"
/*
#include "../content/epLogDraw.h"
#include "../content/epLogManager.h"
*/

void CHandleMessage::handleGetStudentDrawLog(Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetStudentDrawLog\n";
#endif
        /*
        cGetStudentDrawLog cgsdl;
        UNPACKET(p, cgsdl);

        epStudent* p_student = const_cast<epStudent*>(EPMANAGER->getStudentById(cgsdl.student_id()));
        CHECK_P(p_student);

        epLogBase::buf_type str;
        Buf* p_buf = NULL;
        read_log log(LOG_MANAGER->get_log(p_student, LOG_DRAW)->get_log_file());

        while (true) {
                str = log();
                if (0 == str.size()) break;

                p_buf = SINGLE->bufpool.malloc();
                CHECK_P(p_buf);
                p_buf->setfd(p->getfd());
                p_buf->setsize(((MSG_HEAD*)str.c_str())->cLen);
                memcpy(p_buf->ptr(), str.c_str(), p_buf->size());

                SINGLE->sendqueue.enqueue(p_buf);
        }
        */

        /*
        while (true) {
                p_buf = log();
                CHECK_P(p_buf);
                SINGLE->sendqueue.enqueue(p_buf);
        }
        */

        RETURN(p);
}
