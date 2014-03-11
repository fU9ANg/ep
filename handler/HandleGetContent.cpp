#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epUser.h"
#include "../content/epManager.h"
#include "../netdef.h"

void CHandleMessage::handleGetContent (Buf* p)
{
        /**
         * @brief 用户端请求课程列表。
         *        1. 根据用户ID，查找用户所在教室。
         *        2. 将该教室的课程列表返回。
         */

#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_GetContent\n";
#endif

        sGetContent tmp;
        epClassroom* pClassroom = EPMANAGER->getClassroomByFd(p->getfd());
        CHECK_P(pClassroom);

        Buf* pBuf = SINGLE->bufpool.malloc();
        CHECK_P(pBuf);
        ((MSG_HEAD*)pBuf->ptr())->cType = ST_GetContent;
        ((MSG_HEAD*)pBuf->ptr())->cLen  = MSG_HEAD_LEN + sizeof(int) + pClassroom->courseList_.size();
        *(int*)((char*)pBuf->ptr()+MSG_HEAD_LEN) = 1;

        memcpy((char*)pBuf->ptr() + MSG_HEAD_LEN + sizeof(int), pClassroom->courseList_.c_str(), pClassroom->courseList_.size());
        pBuf->setfd(p->getfd());
        pBuf->setsize(((MSG_HEAD*)pBuf->ptr())->cLen);

        sGetContent sgc;
        ContentNode cn;
        unpacket(pBuf, sgc);
        for (int i=0; i<sgc.course_list_size(); ++i) {
                cn = sgc.course_list(i);
                printf("[DEBUG] CHandleMessage::handleGetContent : course id       = %d\n", cn.course_id());
                printf("[DEBUG] CHandleMessage::handleGetContent : course xml path = %s\n", cn.course_xml().c_str());
        }

        SINGLE->sendqueue.enqueue(pBuf);

        RETURN(p);
}
