#include "HandleMessage.h"
#include "../netdef.h"
#include "../global_functions.h"
#include "../content/epUser.h"

#include "../content/epManager.h"
#include "../Config.h"

void
CHandleMessage::handleGetServerAddr(Buf* p) {
#ifdef __DEBUG_HANDLE_HEAD_
        std::cout << "CT_GetServerAddr" << std::endl;
#endif
        const epUser* p_user = EPMANAGER->getUserByFd(p->getfd());
        CHECK_P(p_user);

        std::string addr = CONFIG->download_upload_server_ip;
        int port = CONFIG->download_upload_server_port;

        serverInfoNode tmp;
        if (0 != addr.size())
                tmp.set_server_addr(addr);
        if (0 != port)
                tmp.set_server_port(port);

        char buf[32] = {0};
        snprintf(buf, sizeof(port)+1, "%d", port);
        tmp.set_full_addr("ftp://" + addr + ":" + buf + "/SOURCES/aaaaa/");

        Buf* p_buf = packet(ST_GetServerAddr, tmp, p->getfd());
        CHECK_P(p_buf);
        SINGLE->sendqueue.enqueue(p_buf);

        RETURN(p);
}
