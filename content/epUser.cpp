#include "epUser.h"
#include <stdio.h> // for printf
#include "../netdef.h"

epUser::epUser(void)
        : funcType_(FT_INVALID)
          , id_(EPUSER_INVALID_USER_ID)
          , fd_(EPUSER_INVALID_USER_FD)
{ }

epUser::~epUser(void) { }

bool
epUser::init(const std::string& account
           , const std::string& passwd) {
        account_ = account;
        passwd_  = passwd;
        return true;
}

const enum LoginType
epUser::getType(void) const {
        return LT_USER;
}

void
epUser::dump(void) {
        printf("=======================================\n");
        printf("id       = %d\n", id_);
        printf("fd       = %d\n", fd_);
        printf("funcType = %d\n", funcType_);
        printf("account  = %s\n", account_.c_str());
        printf("passwd   = %s\n", passwd_.c_str());
}
