/*
 * HandleClassRoom.cpp
 */

#include "HandleMessage.h"

#include "protocol.h"
#include "Buf.h"
#include "LoginCheck.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

void CHandleMessage::handleLogin (Buf* p) {
        sLogin i;
        packet(ST_Test, i, p->getfd());
        packet(ST_Test, p->getfd());
        unpacket(p, i);
        Buf* pBuf = NULL;
        combine_buf(pBuf, &p);
        cout << "CT_Login\n";
        LoginCheck::check(p);

        cLogin clogin;
        unpacket(p, clogin);
        sLogin slogin;
        // slogin.result = checkLogin(clogin.account, clogin.passwd);
        // bool checkLogin(const std::string&, const std::string&);
        SINGLE->sendqueue.enqueue(packet(ST_Login, slogin, p->getfd()));
}
