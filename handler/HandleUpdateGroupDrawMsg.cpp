#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"
#include "../netdef.h"

#include "../content/epManager.h"

void CHandleMessage::handleUpdateGroupDrawMsg(Buf* p) {

        epUser* pUser = NULL;
        pUser = const_cast<epUser*>(EPMANAGER->getUserByFdFromClassroom(p->getfd()));
        //epUser* pUser = const_cast<epUser*>(EPMANAGER->getUserByFd(p->getfd()));
        CHECK_P(pUser);

        enum CommandType type = ((MSG_HEAD*)p->ptr())->cType;
        cUpdateCopyIntoPaint cucip;
        cUpdatePenAnderaser cupa;
        UpdatePuzzleResult upr;

        switch (type) {
        case CT_UpdateBackground :
#ifdef __DEBUG_HANDLE_HEAD_
                cout << "CT_UpdateBackground\n";
#endif
                type = ST_UpdateBackground;
                break;
        case CT_UpdateCopyIntoPaint :
#ifdef __DEBUG_HANDLE_HEAD_
                cout << "CT_UpdateCopyIntoPaint\n";
#endif
#ifdef __DEBUG__
                /*
                unpacket(p, cucip);
                printf("[DEBUG] CHandleMessage::handleUpdateGroupDrawMsg : CT_UpdateCopyIntoPaint pt_x = %f\n", atof(cucip.pt_x().c_str()));
                printf("[DEBUG] CHandleMessage::handleUpdateGroupDrawMsg : CT_UpdateCopyIntoPaint pt_y = %f\n", atof(cucip.pt_y().c_str()));
                printf("[DEBUG] CHandleMessage::handleUpdateGroupDrawMsg : CT_UpdateCopyIntoPaint pt_y = %s\n", cucip.pic_res().c_str());
                */
#endif
                type = ST_UpdateCopyIntoPaint;
                break;
        case CT_UpdatePenAnderaser :
#ifdef __DEBUG_HANDLE_HEAD_
                cout << "CT_UpdatePenAnderaser\n";
#endif
#ifdef __DEBUG__
                /*
                unpacket(p, cupa);
                printf("[DEBUG] CHandleMessage::handleUpdateGroupDrawMsg : CT_UpdatePenAnderaser pt_x    = %f\n", atof(cupa.pt_x().c_str()));
                printf("[DEBUG] CHandleMessage::handleUpdateGroupDrawMsg : CT_UpdatePenAnderaser pt_y	 = %f\n", atof(cupa.pt_y().c_str()));
                printf("[DEBUG] CHandleMessage::handleUpdateGroupDrawMsg : CT_UpdatePenAnderaser prept_x = %f\n", atof(cupa.prept_x().c_str()));
                printf("[DEBUG] CHandleMessage::handleUpdateGroupDrawMsg : CT_UpdatePenAnderaser prept_y = %f\n", atof(cupa.prept_y().c_str()));
                */
#endif
                type = ST_UpdatePenAnderaser;
                break;
        case CT_UpdateFilling :
#ifdef __DEBUG_HANDLE_HEAD_
                cout << "CT_UpdateFilling\n";
#endif
#ifdef __DEBUG__
                // printf("[DEBUG] CHandleMessage::handleUpdateGroupDrawMsg : CT_UpdateFilling pt_x    = %f\n", atof(cupa.pt_x().c_str()));
#endif
                type = ST_UpdateFilling;
                break;
        case CT_UpdateStamp :
#ifdef __DEBUG_HANDLE_HEAD_
                cout << "CT_UpdateStamp\n";
#endif
                type = ST_UpdateStamp;
                break;
        case CT_UpdateFillPic :
#ifdef __DEBUG_HANDLE_HEAD_
                cout << "CT_UpdateFillPic\n";
#endif
                type = ST_UpdateFillPic;
                break;
        case CT_UpdateCollage :
#ifdef __DEBUG_HANDLE_HEAD_
                cout << "CT_UpdateCollage\n";
#endif
                type = ST_UpdateCollage;
                break;
        case CT_UpdateWord :
#ifdef __DEBUG_HANDLE_HEAD_
                cout << "CT_UpdateWord\n";
#endif
                type = ST_UpdateWord;
                break;
        case CT_UpdateFrame :
#ifdef __DEBUG_HANDLE_HEAD_
                cout << "CT_UpdateFrame\n";
#endif
                type = ST_UpdateFrame;
                break;
        case CT_UpdatePuzzleResult :
#ifdef __DEBUG_HANDLE_HEAD_
                std::cout << "CT_UpdatePuzzleResult" << std::endl;
#endif
                type = ST_UpdatePuzzleResult;
                UNPACKET(p, upr);
                // printf("[DEBUG] CHandleMessage::handleUpdateGroupDrawMsg : group = %d\n", (upr.group_info()).group_id());
                /*
                printf("[DEBUG] CHandleMessage::handleUpdateGroupDrawMsg : x = %f\n", atof((upr.point()).pt_x().c_str()));
                printf("[DEBUG] CHandleMessage::handleUpdateGroupDrawMsg : y = %f\n", atof((upr.point()).pt_y().c_str()));
                */
                break;
        default :
#ifdef __DEBUG_HANDLE_HEAD_
                cout << "CT_UpdateGroupMsg : defualt\n";
#endif
                break;
        }

        ((MSG_HEAD*)p->ptr())->cType = type;

        epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
        CHECK_P(pClassroom);

        if (LT_TEACHER == pUser->getType()) {
                Buf* p_buf = NULL;
                CLONE_BUF(p_buf, p);
                pClassroom->sendtoWhiteBoard(p_buf);

                pClassroom->sendtoTeacher(p);
                return;
        }

        /*
        if (LT_WHITEBOARD == pUser->getType()) {
                epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
                CHECK_P(pClassroom);
                pClassroom->sendtoWhiteBoard(p);
                return;
        }
        */

        epGroup* pGroup = const_cast<epGroup*>(EPMANAGER->getGroupByFd(p->getfd()));
        if (NULL == pGroup) { // 该学生不在上课。
                printf("[DEBUG] CHandleMessage::handleUpdateGroupDrawMsg : NULL == pGroup\n");
                SINGLE->bufpool.free(p);
                return;
        } else {
                Buf* p_buf = NULL;
                CLONE_BUF(p_buf, p);
                pClassroom->sendtoWhiteBoard(p_buf);

                Buf* p_buf_1 = NULL;
                CLONE_BUF(p_buf_1, p);
                // if (pClassroom->undisplay_)
                pClassroom->sendtoTeacher(p_buf_1);

                pGroup->sendtoAllStudent(p, true);
        }

        /*
           epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
           if (NULL == pClassroom) {
           printf("[DEBUG] CHandleMessage::handleUpdateGroupDrawMsg : NULL == pClassroom\n");
           SINGLE->bufpool.free(p);
           return;
           }

           Buf* pBuf_1 = NULL;
           CLONE_BUF(pBuf_1, p);
           pClassroom->sendtoAllClass(pBuf_1, true);

           Buf* pBuf_2 = NULL;
           CLONE_BUF(pBuf_2, p);
           pClassroom->sendtoWhiteBoard(pBuf_2);

           SINGLE->bufpool.free(p);
           */

        return;
}
