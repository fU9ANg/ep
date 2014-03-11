
#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"
#include "Single.h"
#include "../netdef.h"

void CHandleMessage::handleTest (Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
    cout << "CT_Test\n";
#endif
    RETURN(p);
}


void CHandleMessage::handleAuth (Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
    cout << "CT_Auth\n";
#endif
    RETURN(p);
}

void CHandleMessage::handleHeart (Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
    // cout << "CT_Heart : fd = " << p->getfd() << endl;
#endif
    RETURN(p);
}
