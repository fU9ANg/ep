#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"

void CHandleMessage::handleDownloadFromPublic(Buf* p) {
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_DownloadFromPublic\n";
#endif

        /*
        cDownloadFromPublic cdfp;
        UNPACKET(p, cdfp);

        const epUser* pUser = EPMANAGER->getUserByFd(p->getfd());
        CHECK_P(pUser);

        sDownloadFromPublic tmp;
        std::string res_path;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (XXX);
                pstmt->setInt (1, cdfp.book_id());
                ResultSet* prst = pstmt->executeQuery ();
                while (prst->next ()) {
                        res_path = prst->getString("res_path");
                }
                delete prst;
                delete pstmt;
        }catch (SQLException e) {
                PRINT_CATCH(e);
                RETURN(p);
        }

        tmp.set_result((0==res_path.size()) ? true : false);
        char buf[128] = {0};
        snprintf(buf, sizeof(CONFIG->download_upload_server_port), "%d", CONFIG->download_upload_server_port);
        tmp.set_down_addr("http://" + CONFIG->download_upload_server_ip + ":" + buf + "/" + res_path);

        Buf* pBuf = packet(ST_GetClassList, tmp, p->getfd());
        CHECK_P(pBuf);
        SINGLE->sendqueue.enqueue(pBuf);
        RETURN(p);
        */
}
