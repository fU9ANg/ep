#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

void CHandleMessage::handleSetFunc (Buf* p) {
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_SetFunc\n";
#endif
        // TODO:

        /*
        cSetFunc setFunc;
        unpacket(p, setFunc);
        switch (setFunc.funcId()) {
        case PERSONAL :
                break;
        case SCHOOL :
                break;
        case FAMILY :
                break;
        case FRIEND :
                break;
        case OFFICE :
                break;
        default :
                break;
        }
        */
}
