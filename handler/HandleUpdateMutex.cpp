#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"

void CHandleMessage::handleUpdateMutex (Buf* p) {
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_UpdateMutex\n";
#endif

        epGroup* pGroup = const_cast<epGroup*>(EPMANAGER->getGroupByFd(p->getfd()));
        CHECK_P(pGroup);

        cUpdateMutex cum;
        UNPACKET(p, cum);

        bool result = false;
        sUpdateMutex tmp;
        Buf* pBuf = NULL;
        epStudent* p_student = NULL;
        switch (cum.ms()) {
        case MS_LOCK :
                result = pGroup->lock(cum.lock_id());
                /*
                if (result) {
                        tmp.set_result(result);
                }
                tmp.set_lock_id(cum.lock_id());
                printf("[DEBUG] CHandleMessage::handleUpdateMutex : tmp.result() = %d\n", tmp.result());
                pBuf = packet(ST_UpdateMutex, tmp, p->getfd());
                CHECK_P(pBuf);
                SINGLE->sendqueue.enqueue(pBuf);
                */
                break;
        case MS_UNLOCK :
                result = pGroup->unlock(cum.lock_id());
                break;
        case MS_INVALID :
                result = !(pGroup->isLock(cum.lock_id()));
                if (result) {
                        tmp.set_result(result);
                }
                tmp.set_lock_id(cum.lock_id());
                p_student = const_cast<epStudent*>(pGroup->getStudentByFd(p->getfd()));
                CHECK_P(p_student);
                tmp.set_student_id(p_student->id_);
                pBuf = packet(ST_UpdateMutex, tmp, p->getfd());
                CHECK_P(pBuf);
                SINGLE->sendqueue.enqueue(pBuf);
                break;
        default :
                break;
        }

        RETURN(p);
}
