#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"
#include "Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"

void CHandleMessage::handleGetWhiteboardInfo(Buf* p) 
{
#ifdef __DEBUG_HANDLE_HEAD_
	cout << "CT_GetWhiteboardInfo\n";
#endif

        epWhiteBoard* p_whiteboard = dynamic_cast<epWhiteBoard*>(const_cast<epUser*>(EPMANAGER->getUserByFd(p->getfd())));
        if (NULL == p_whiteboard) {
                epClassroom* p_classroom = EPMANAGER->getClassroomByFd(p->getfd());
                CHECK_P(p_classroom);
                p_whiteboard = p_classroom->whiteboard_;
                CHECK_P(p_whiteboard);
        }

#if 0
        epWhiteBoard* p_whiteboard = NULL;
        epClassroom* p_classroom = EPMANAGER->getClassroomByFd(p->getfd());
        if(NULL != p_classroom) { // in classroom
                /*
                   if((NULL != p_classroom->whiteboard_) && (p_classroom->whiteboard_->id_ != EPWHITEBOARD_INVALID_WHITEBOARD_ID)) {
                   DEBUG_INFO;
                   tmp.set_whiteboard_num_	(p_classroom->whiteboard_->whiteboardNum_);
                   tmp.set_classroom_name_	(p_classroom->whiteboard_->classroomName_);
                   tmp.set_classroom_id_	(p_classroom->whiteboard_->classroomId_);
                   tmp.set_functype_	(p_classroom->whiteboard_->funcType_);
                   tmp.set_id_		(p_classroom->whiteboard_->id_);
                   tmp.set_fd_		(p_classroom->whiteboard_->fd_);
                   tmp.set_userstatus_	(p_classroom->whiteboard_->userStatus_);
                   tmp.set_account_	(p_classroom->whiteboard_->account_);
                   tmp.set_passwd_		(p_classroom->whiteboard_->passwd_);
                   }
                   */
                CHECK_P(p_classroom->whiteboard_);
                p_whiteboard = p_classroom->whiteboard_;
        } else { // in user
        }
#endif

        sGetWhiteboardInfo tmp;
        tmp.set_whiteboard_num_	(p_whiteboard->whiteboardNum_);
        tmp.set_classroom_name_	(p_whiteboard->classroomName_);
        tmp.set_classroom_id_	(p_whiteboard->classroomId_);
        printf("[DEBUG] CHandleMessage::handleGetWhiteboardInfo : room_id = %d\n", p_whiteboard->classroomId_);
        tmp.set_functype_	(p_whiteboard->funcType_);
        tmp.set_id_		(p_whiteboard->id_);
        tmp.set_fd_		(p_whiteboard->fd_);
        tmp.set_userstatus_	(p_whiteboard->userStatus_);
        tmp.set_account_	(p_whiteboard->account_);
        tmp.set_passwd_		(p_whiteboard->passwd_);

        Buf* pBuf = packet(ST_GetWhiteboardInfo, tmp, p->getfd());
        CHECK_P(pBuf);
        SINGLE->sendqueue.enqueue(pBuf);

        RETURN(p);
}
