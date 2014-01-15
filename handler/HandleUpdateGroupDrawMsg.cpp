#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"

void CHandleMessage::handleUpdateGroupDrawMsg(Buf* p) {
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_UpdateGroupDrawMsg\n";
#endif

        epGroup* pGroup = const_cast<epGroup*>(EPMANAGER->getGroupByFd(p->getfd()));
        if (NULL == pGroup) { // 该学生不在上课。
                printf("[DEBUG] CHandleMessage::handleUpdateGroupDrawMsg : NULL == pGroup\n");
                SINGLE->bufpool.free(p);
                return;
        }

        enum CommandType type = ((MSG_HEAD*)p->ptr())->cType;

        switch (type) {
        case CT_UpdateBackground :
                type = ST_UpdateBackground;
                break;
        case CT_UpdateCopyIntoPaint :
                type = ST_UpdateCopyIntoPaint;
                break;
        case CT_UpdatePenAnderaser :
                type = ST_UpdatePenAnderaser;
                break;
        case CT_UpdateFilling :
                type = ST_UpdateFilling;
                break;
        case CT_UpdateStamp :
                type = ST_UpdateStamp;
                break;
        case CT_UpdateFillPic :
                type = ST_UpdateFillPic;
                break;
        case CT_UpdateCollage :
                type = ST_UpdateCollage;
                break;
        case CT_UpdateWord :
                type = ST_UpdateWord;
                break;
        default :
                break;
        }

        ((MSG_HEAD*)p->ptr())->cType = type;
        pGroup->sendtoAllStudent(p);
}
