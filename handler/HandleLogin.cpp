/*
 * HandleClassRoom.cpp
 */

#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"
#include "../netdef.h" // for __DEBUG__

#include "../content/epManager.h"
#include "../content/epUser.h"
#include "../content/epStudent.h"
#include "../content/epTeacher.h"
#include "../content/epWhiteBoard.h"

#define CREATE_USER(type)

void CHandleMessage::handleLogin (Buf* p) {
        cLogin clogin;
        unpacket(p, clogin);
#ifdef __DEBUG__
        printf("clogin.type    = %d\n", clogin.type());
        printf("clogin.account = %s\n", clogin.account().c_str());
        printf("clogin.passwd  = %s\n", clogin.passwd().c_str());
#endif

        epUser* pUser = NULL;
        switch (clogin.type()) {
        case LT_STUDENT :
                pUser = new epStudent();
                break;
        case LT_HEADMASTER :
                // pUser = new epHeadmaster();
                break;
        case LT_TEACHER :
                pUser = new epTeacher();
                break;
        case LT_PARENTS :
                // pUser = new epParents();
                break;
        case LT_WHITEBOARD :
                pUser = new epWhiteBoard();
                break;
        default :
                break;
        }

        bool result = pUser->init(clogin.account(), clogin.passwd());
        if (!result) {
                printf("[INFO] handleLogin : epUser init fault!\n");
                delete pUser;
                pUser = NULL;
        } else {
                EPMANAGER->insertUser(p->getfd(), pUser);
        }

        sLogin slogin;
        slogin.set_result(result);
        SINGLE->sendqueue.enqueue(packet(ST_Login, slogin, p->getfd()));
}
