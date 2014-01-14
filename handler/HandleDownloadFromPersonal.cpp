#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"

void CHandleMessage::handleDownloadFromPersonal (Buf* p)
{
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_DownloadFromPersonal\n";
#endif

        cDownloadFromPublic cdfp;
        if (!unpacket(p, cdfp)) {
                printf("[DEBUG] CHandleMessage::handleDownloadFromPersonal : unpacket fail!\n");
                SINGLE->bufpool.free(p);
                return;
        }

        const epUser* pUser = EPMANAGER->getUserByFd(p->getfd());
        if (NULL == pUser) {
                printf("[DEBUG] CHandleMessage::handleDownloadFromPersonal : NULL == pUser\n");
                SINGLE->bufpool.free(p);
                return;
        }

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
                printf("[DEBUG] CHandleMessage::handleDownloadFromPersonal : SQLException = %s\n", e.what());
                SINGLE->bufpool.free(p);
                return;
        }

        /*
        tmp.set_down_addr("http://" + CONFIG->download_upload_server_ip
                        + ":" + CONFIG->download_upload_server_port + "/" + res_path);

        Buf* pBuf = packet(ST_DownloadFromPersonal, tmp, p->getfd());
        if (NULL != pBuf) {
                SINGLE->sendqueue.enqueue(pBuf);
        }
        SINGLE->bufpool.free(p);
        */
}
