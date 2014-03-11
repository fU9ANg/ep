#include "epUser.h"
#include <stdio.h> // for printf
#include "../netdef.h"
#include <stdlib.h>

epUser::epUser(void)
        : funcType_(FT_INVALID)
          , id_(EPUSER_INVALID_USER_ID)
          , fd_(EPUSER_INVALID_USER_FD)
{
}

epUser::~epUser(void) { }

bool
epUser::init(const std::string& account
           , const std::string& passwd
           , const int fd
           , const enum user_status us
           ) {
        MutexLockGuard guard(lock_);
        account_    = account;
        passwd_     = passwd;
        fd_         = fd;
        userStatus_ = us;
        /*
        char buf[BUF_MAX_SIZE] = {0};
        snprintf(buf, BUF_MAX_SIZE, "%s_%d.eplog", account.c_str(), rand());
        log_filename_ = buf;
        std::cout << "log_filename_ = " << log_filename_ << std::endl;
        */
        
        return true;
}

const enum LoginType
epUser::getType(void) const {
        return LT_USER;
}

void
epUser::dump(void) {
        MutexLockGuard guard(lock_);
        std::cout << "===============================" << std::endl;
        std::cout << "id       = " << id_              << std::endl;
        std::cout << "fd       = " << fd_              << std::endl;
        std::cout << "funcType = " << funcType_        << std::endl;
        std::cout << "account  = " << account_.c_str() << std::endl;
        std::cout << "passwd   = " << passwd_.c_str()  << std::endl;
}

bool
epUser::operator==(const std::string& account) {
        MutexLockGuard guard(lock_);

        return account_.size() == account.size()
                && 0 == strncmp(account_.c_str(), account.c_str(), account.size());
}

bool
epUser::check(const std::string& account, const std::string& passwd) {
        MutexLockGuard guard(lock_);

        return 0==strncmp(account_.c_str(), account.c_str(), account.size())
                && 0==strncmp(passwd_.c_str(), passwd.c_str(), passwd.size());
}
