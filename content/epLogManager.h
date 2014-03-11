#ifndef __EPLOGMANAGER_H__
#define __EPLOGMANAGER_H__

#include "epLogBase.h"
#include "Uncopyable.h"
#include "epUser.h"
#include <map>

class epLogManager : public Uncopyable {
public :
        static epLogManager* getInstance(void);
private :
        static epLogManager* p_;

public :
        typedef std::map<epLogBase::log_type, epLogBase*> USER_LOG_MAP;
        typedef std::map<epObject*, USER_LOG_MAP> LOG_MAP;

        bool add_log(epUser*, epLogBase::log_type, epLogBase*);
        bool rm_log(epUser*, epLogBase::log_type);
        epLogBase* get_log(epUser*, epLogBase::log_type);

private :
        MutexLock pthread_lock_;
        LOG_MAP log_map_;

        bool add_user_log(USER_LOG_MAP&, epLogBase::log_type, epLogBase*);
        bool rm_user_log(USER_LOG_MAP&, epLogBase::log_type);
        epLogBase* get_user_log(USER_LOG_MAP&, epLogBase::log_type);
};
