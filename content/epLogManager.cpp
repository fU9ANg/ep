#include "epLogManager.h"

epLogManager* epLogManager::p_ = NULL;

epLogManager*
epLogManager::getInstance(void) {
        if (NULL == p_) {
                p_ = new epLogManager();
        }
        return p_;
}

bool
epLogManager::add_log(epUser* p_user, epLogBase::log_type log_type, epLogBase* rhs) {
                MutexLockGuard guard(pthread_lock_);
        LOG_MAP::iterator it = log_map_.find(p_user);
        if (log_map_.end() != it) {
                return add_user_log(it->second, it->second, rhs);
        } else {
                USER_LOG_MAP tmp;
                return add_user_log(it->second, log_type, rhs) && (log_map_.insert(std::make_pair(p_user, tmp))).second;
        }
}

bool
epLogManager::rm_log(epUser* p_user, epLogBase::log_type log_type) {
                MutexLockGuard guard(pthread_lock_);
        LOG_MAP::iterator it = log_map_.find(p_user);
        bool result = false;
        if (log_map_.end() != it) {
                result = rm_user_log(it->second, log_type);
                if (it->second.empty())
                        log_map_.erase(it);
        }

        return result;
}

epLogBase*
epLogManager::get_log(epUser* p_user, epLogBase::log_type log_type) {
                MutexLockGuard guard(pthread_lock_);
        LOG_MAP::iterator it = log_map_.find(p_user);
        return (log_map_.end() != it) ? get_user_log(it->second, log_type) : NULL;
}

bool
epLogManager::add_user_log(USER_LOG_MAP& user_map, epLogBase::log_type log_type, epLogBase* rhs) {
                MutexLockGuard guard(pthread_lock_);
        USER_LOG_MAP::iterator it = user_map.find(log_type);
        if (user_map.end() != it) {
                return false;
        } else {
                return (user_map.insert(std::make_pair(log_type, rhs)).second;
        }

}

bool
epLogManager::rm_user_log(USER_LOG_MAP& user_map, epLogBase::log_type log_type) {
                MutexLockGuard guard(pthread_lock_);
        USER_LOG_MAP::iterator it = user_map.find(log_type);
        bool result = false;
        if (user_map.end() != it) {
                delete it->second;
                it->second = NULL;
                user_map.erase(it);
                result = true;
        }
        return result;
}

epLogBase*
epLogManager::get_user_log(USER_LOG_MAP& user_map, epLogBase::log_type log_type) {
                MutexLockGuard guard(pthread_lock_);
        USER_LOG_MAP::iterator it = user_map.find(log_type);
        return (user_map.end() != it) ? it->second : NULL;
}
