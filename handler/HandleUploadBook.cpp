#include "HandleMessage.h"

#include "../protocol.h"
#include "../Buf.h"
#include "../Single.h"
#include "../database.h"
#include "../global_functions.h"
#include "../message/proto/protocol.pb.h"

#include "../content/epManager.h"
#include "../netdef.h"

void
CHandleMessage::handleUploadBook(Buf* p) {
#ifdef __DEBUG_HANDLE_HEAD_
        cout << "CT_UploadBook\n";
#endif

        cUploadBook cub;
        UNPACKET(p, cub);

        const epUser* pUser = EPMANAGER->getUserByFd(p->getfd());
        CHECK_P(pUser);

        //int result = 0;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement \
                                           ("CALL epdb.proc2(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
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
                pstmt->setInt   (11,pUser->id_);
                pstmt->setInt   (12,pUser->getType());

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
                printf ("[DEBUG] %s : SQLException: %s\n", __func__, e.what());
                SINGLE->bufpool.free(p);
                return;
        }

        sUploadBook tmp;
        tmp.set_result(TRUE);
        tmp.set_msg("插入成功！\n");

        Buf* pBuf = packet(ST_UploadBook, tmp, p->getfd());
        CHECK_P(pBuf);
        SINGLE->sendqueue.enqueue(pBuf);

        RETURN(p);
}
