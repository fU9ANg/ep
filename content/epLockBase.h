#ifndef __EPLOCKBASE_H__
#define __EPLOCKBASE_H__

enum LockType {
        LOCK_INVALID = 1,
        LOCK_DRAW,
        LOCK_CONN,
        LOCK_PUZZLE,
};

#include <map>

#include "../message/proto/protocol.pb.h"
#include "../Mutex.h"

class epLockBase {
public :
        typedef enum LockType LOCK_TYPE;
        typedef int LOCK_ID_TYPE;
        typedef std::map<LOCK_ID_TYPE, enum mutex_status> LOCK_MAP;

        virtual LOCK_TYPE getType(void) { return LOCK_INVALID; }

        bool lock(const LOCK_ID_TYPE lock_id) {
                MutexLockGuard guard(pthread_lock_);
                LOCK_MAP::iterator it = lock_map_.find(lock_id);
                if (lock_map_.end() != it) {
                        if (MS_LOCK == it->second) {
                                return false;
                        } else {
                                it->second = MS_LOCK;
                                return true;
                        }
                } else {
                        return (lock_map_.insert(std::make_pair(lock_id, MS_LOCK))).second;
                }
        }

        bool unlock(const LOCK_ID_TYPE lock_id) {
                MutexLockGuard guard(pthread_lock_);
                LOCK_MAP::iterator it = lock_map_.find(lock_id);
                if (lock_map_.end() != it) {
                        if (MS_LOCK == it->second) {
                                it->second = MS_UNLOCK;
                                return true;
                        }
                }
                return false;
        }

        bool is_lock(const LOCK_ID_TYPE lock_id) {
                MutexLockGuard guard(pthread_lock_);
                LOCK_MAP::iterator it = lock_map_.find(lock_id);
                return (lock_map_.end() != it && it->second == MS_LOCK);
        }

protected :
        MutexLock pthread_lock_;
        LOCK_MAP lock_map_;
};

template <epLockBase::LOCK_TYPE lock_type>
class epLock : public epLockBase {
public :
        virtual epLockBase::LOCK_TYPE getType(void) { return lock_type; }
};

#endif // __EPLOCKBASE_H__
