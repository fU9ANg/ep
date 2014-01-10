#include "epUser.h"
#include <stdio.h> // for printf

epUser::epUser(void) {
}

epUser::~epUser(void) {
}

bool
epUser::setId(const int id) {
        id_ = id;
        return true;
}

bool
epUser::setFd(const int fd) {
        fd_ = fd;
        return true;
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

bool
epUser::init(const std::string& account, const std::string& passwd) {
        // TODO :
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

enum LoginType
epUser::getType(void) const {
        return LT_USER;
}

int
epUser::getId(void) const {
        return id_;
}

int
epUser::getFd(void) const {
        return fd_;
}

void
epUser::dump(void) {
        printf("id      = %d\n", id_);
        printf("account = %s\n", account_.c_str());
        printf("passwd  = %s\n", passwd_.c_str());
}
