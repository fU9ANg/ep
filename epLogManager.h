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
        typedef std::map<typename epLog<T>::log_type, typename epLog<T>*> USER_LOG_MAP;
        typedef std::map<epUser*, USER_LOG_MAP> LOG_MAP;

        bool add_log(epUser*, epLogBase::log_type, epLogBase::log_filename_type&);
        bool rm_log(epUser*, epLogBase::log_type);
        epLogBase* get_log(epUser*, epLogBase::log_type);

private :
        LOG_MAP log_map_;

        bool add_user_log(epLogBase::log_filename_type&, epLogBase::log_type, USER_LOG_MAP&);
        bool rm_user_log(USER_LOG_MAP&, epLogBase::log_type);
        epLogBase* get_user_log(USER_LOG_MAP&, epLogBase::log_type);
};

#define LOG_MANAGER (epLogManager::getInstance())

#endif // __EPLOGMANAGER_H__
