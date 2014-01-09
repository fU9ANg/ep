#include "epUser.h"

epUser::epUser(void) {
}

epUser::~epUser(void) {
}

bool
epUser::setAccount(const std::string& account) {
        account_ = account;
        return true;
}

bool
epUser::setPasswd(const std::string& passwd) {
        passwd_ = passwd;

        return true;
}

const std::string&
epUser::getAccount(void) const {
        return account_;
}

const std::string&
epUser::getPasswd(void) const {
        return passwd_;
}

void
epUser::dump(void) {
        printf("account = %d\n", account_);
        printf("passwd  = %s\n", passwd_.c_str());
}
