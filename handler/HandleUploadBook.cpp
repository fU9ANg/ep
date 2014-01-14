#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"

void
CHandleMessage::handleUploadBook(Buf* p) {
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_UploadBook\n";
#endif

        cUploadBook cub;
        if (!unpacket(p, cub)) {
                printf("[DEBUG] CHandleMessage::handleUploadBook : unpacket fail!\n");
                SINGLE->bufpool.free(p);
                return;
        }

        const epUser* pUser = EPMANAGER->getUserByFd(p->getfd());
        if (NULL == pUser) { // 该用户不在线或在上课状态。
                printf("[DEBUG] CHandleMessage::handleUploadBook : NULL == pUser\n");
                SINGLE->bufpool.free(p);
                return;
        }

        //int result = 0;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement \
                                           ("CALL epdb.proc2(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
                pstmt->setInt   (1, cub.book_type());
                pstmt->setString(2, cub.book_name());
                pstmt->setString(3, cub.time());
                pstmt->setInt   (4, cub.belongs());
                pstmt->setInt   (5, cub.art());
                pstmt->setInt   (6, cub.language());
                pstmt->setInt   (7, cub.community());
                pstmt->setInt   (8, cub.health());
                pstmt->setInt   (9, cub.science());
                pstmt->setString(10,cub.res_path());

                pstmt->execute ();
#if 0 
                pstmt = DATABASE->preStatement ("SELECT @_out AS out");
                ResultSet* prst = pstmt->executeQuery ();
                while (prst->next ()) {
                        result = prst->getInt ("out");
                        printf ("result============%d\n", result);
                }
#endif
                //delete prst;
                delete pstmt;
        }catch (SQLException e) {
                printf ("SQLException: %s\n", e.what());
                SINGLE->bufpool.free(p);
                return;
        }

        sUploadBook tmp;
        tmp.set_result(true);
        tmp.set_msg("插入成功！\n");

        Buf* pBuf = packet(ST_UploadBook, tmp, p->getfd());
        if (NULL != pBuf) {
                SINGLE->sendqueue.enqueue(pBuf);
        }
        SINGLE->bufpool.free(p);
        return;
}
