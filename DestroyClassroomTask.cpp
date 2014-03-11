#include "DestroyClassroomTask.h"
#include "content/epManager.h"
#include "global_functions.h"
#include "netdef.h"
#include "content/epClassroom.h"

#define INTERVAL       (20) // 秒
#define INTERVAL_SLEEP (2)  // 秒

DES_MAP DestroyClassroomTask::desMap_;

int DestroyClassroomTask::work(void) {
        while (true) {
                DES_MAP::iterator it = desMap_.begin();
                DES_MAP::const_iterator cie = desMap_.end();
                time_t now = time(NULL);
                for (; cie!=it; ++it) {
                        if (INTERVAL <= now - it->second) {
                                Buf* pBuf = packet(ST_ClassOver, 0);
                                CHECK_BUF(pBuf, NULL);
                                epClassroom* pClassroom = EPMANAGER->getClassroomById(it->first);
                                if (NULL != pClassroom) {
                                        printf("[DEBUG] DestroyClassroomTask::work : NULL!=pClassroom\n");
                                        pClassroom->sendtoAll(pBuf);
                                        EPMANAGER->deleteClassroomById(it->first);
                                        desMap_.erase(it);
                                }
                        }
                }
                sleep(INTERVAL_SLEEP);
        }

        return -1;
}
