#include "DestroyClassroomTask.h"
#include "content/epManager.h"
#include "global_functions.h"
#include "netdef.h"
#include "content/epClassroom.h"

#define INTERVAL       (5 * 60) // 秒
#define INTERVAL_SLEEP (1)     // 秒

DES_MAP DestroyClassroomTask::desMap_;

int DestroyClassroomTask::work(void) {
        while (true) {
                DES_MAP::iterator it = desMap_.begin();
                DES_MAP::const_iterator cie = desMap_.end();
                time_t now = time(NULL);
                for (; cie!=it; ++it) {
                        if (INTERVAL <= it->second - now) {
                                EPMANAGER->deleteClassroomById(it->first);
                                desMap_.erase(it);

                                Buf* pBuf = packet(ST_ClassOver, 0);
                                CHECK_BUF(pBuf, NULL);
                                epClassroom* pClassroom = EPMANAGER->getClassroomById(it->first);
                                if (NULL != pClassroom) {
                                        pClassroom->sendtoAll(pBuf);
                                }
                        }
                }
                sleep(INTERVAL_SLEEP);
        }

        return -1;
}
