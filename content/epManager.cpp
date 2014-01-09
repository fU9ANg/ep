#include "epManager.h"
#include <iostream>

epManager* epManager::p_ = NULL;

epManager* epManager::instance(void) {
        if (NULL == p_) {
                p_ = new epManager();
        }

        return p_;
}

epManager::epManager(void) {}

epManager::~epManager(void) {}

bool
epManager::insertClassroom(const epClassroom& obj) {
        EPCLASSROOM_MAP::iterator it = classroomMap_.begin();
        EPCLASSROOM_MAP::const_iterator cie = classroomMap_.end();

        for (; cie!=it; ++it) {
                if (obj.getId() == it->first) {
                        return false;
                }
        }

        classroomMap_.insert(std::make_pair<int, epClassroom>(obj.getId(), obj));
        return true;
}

bool
epManager::removeClassroomById(int id) {
        EPCLASSROOM_MAP::iterator it = classroomMap_.begin();
        EPCLASSROOM_MAP::const_iterator cie = classroomMap_.end();

        for (; cie!=it; ++it) {
                if (id == it->first) {
                        classroomMap_.erase(it);
                        return true;
                }
        }

        return false;
}

const epClassroom*
epManager::getClassroomById(int id) {
        EPCLASSROOM_MAP::iterator it = classroomMap_.begin();
        EPCLASSROOM_MAP::const_iterator cie = classroomMap_.end();

        for (; cie!=it; ++it) {
                if (id == it->first) {
                        return &(it->second);
                }
        }

        return NULL;
}

bool
epManager::insertUser(const int fd, const pUser& user) {
        EPFREECLIENT_MAP::iterator it = freeClientMap_.begin();
        EPFREECLIENT_MAP::const_iterator cie = freeClientMap_.end();

        for (; cie!=it; ++it) {
                if (fd == it->first) {
                        return false;
                }
        }

        freeClientMap_.insert(std::make_pair<int, epUser*>(fd, &user));
        return true;
}

bool
epManager::removeUserByFd(const int fd) {
        EPFREECLIENT_MAP::iterator it = freeClientMap_.begin();
        EPFREECLIENT_MAP::const_iterator cie = freeClientMap_.end();

        for (; cie!=it; ++it) {
                if (fd == it->first) {
                        delete it->second;
                        freeClientMap_.erase(it);
                        return true;
                }
        }

        return false;
}

const epUser*
epManager::getUserByFd(const int fd) {
        EPFREECLIENT_MAP::iterator it = freeClientMap_.begin();
        EPFREECLIENT_MAP::const_iterator cie = freeClientMap_.end();

        for (; cie!=it; ++it) {
                if (fd == it->first) {
                        epUser* pu = new epUser(*(it->second));
                        return pu;
                }
        }

        return NULL;
}

void
epManager::dumpClassroom(void) const {
        EPCLASSROOM_MAP::iterator it = classroomMap_.begin();
        EPCLASSROOM_MAP::const_iterator cie = classroomMap_.end();

        for (; cie!=it; ++it) {
                (it->second).dump();
        }
}

void
epManager::dumpUser(void) const {
        EPUSER_MAP::iterator it = userMap_.begin();
        EPUSER_MAP::const_iterator cie = userMap_.end();

        for (; cie!=it; ++it) {
                (it->second).dump();
        }
}
