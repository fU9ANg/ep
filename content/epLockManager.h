#ifndef __EPLOCKMANAGER_H__
#define __EPLOCKMANAGER_H__

#include "Uncopyable.h"
#include <map>
#include "epLockBase.h"

class epLockManager : public Uncopyable {
public :
        static epLockManager* getInstance(void) {
                if (NULL == p_) {
                        p_ = new epLockManager();
                }
                return p_;
        }
private :
        static epLockManager* p_;

public :
        typedef std::map<const enum LockType, epLockBase*> USER_LOCK_MAP;
        typedef std::map<epObject*, USER_LOCK_MAP> LOCK_MAP;

        bool add(epObject* obj, epLockBase::LOCK_TYPE type, epLockBase* rhs) {
                MutexLockGuard guard(pthread_lock_);
                LOCK_MAP::iterator it = lock_map_.find(obj);
                if (lock_map_.end() != it) {
                        return user_add(it->second, type, rhs);
                } else {
                        USER_LOCK_MAP tmp;
                        return user_add(tmp, type, rhs) && (lock_map_.insert(std::make_pair(obj, tmp))).second;
                }
        }

        bool rm (epObject* obj, epLockBase::LOCK_TYPE type) {
                MutexLockGuard guard(pthread_lock_);
                LOCK_MAP::iterator it = lock_map_.find(obj);
                if (lock_map_.end() != it) {
                        return user_rm(it->second, type);
                }
                return false;
        }

        epLockBase* get(epObject* obj, epLockBase::LOCK_TYPE type) {
                MutexLockGuard guard(pthread_lock_);
                LOCK_MAP::iterator it = lock_map_.find(obj);
                if (lock_map_.end() != it) {
                        return user_get(it->second, type);
                }
                return NULL;
        }

protected :
        MutexLock pthread_lock_;
        LOCK_MAP lock_map_;

        bool user_add(USER_LOCK_MAP& user_map, epLockBase::LOCK_TYPE type, epLockBase* rhs) {
                MutexLockGuard guard(pthread_lock_);
                USER_LOCK_MAP::iterator it = user_map.find(type);
                if (user_map.end() != it) {
                        return false;
                } else {
                        return (user_map.insert(std::make_pair(type, rhs))).second;
                }
        }

        bool user_rm (USER_LOCK_MAP& user_map, epLockBase::LOCK_TYPE type) {
                MutexLockGuard guard(pthread_lock_);
                USER_LOCK_MAP::iterator it = user_map.find(type);
                if (user_map.end() != it) {
                        delete it->second;
                        it->second = NULL;
                        user_map.erase(it);
                        return true;
                }
                return false;
        }

        epLockBase* user_get(USER_LOCK_MAP& user_map, epLockBase::LOCK_TYPE type) {
                MutexLockGuard guard(pthread_lock_);
                USER_LOCK_MAP::iterator it = user_map.find(type);
                if (user_map.end() != it) {
                        return it->second;
                }
                return NULL;
        }
};

#define LOCK_MANAGER (epLockManager::getInstance())

#endif // __EPLOCKMANAGER_H__
