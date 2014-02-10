#include "epClass.h"
#include "../Single.h"
#include "../netdef.h" // EPCLASSROOM_INVALID_CLASSROOM_ID
#include <stdio.h> // for printf
#include "epManager.h"

epClass::epClass(void) {
}

epClass::~epClass(void) {
        EPSTUDENT_MAP::iterator it = studentMap_.begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.end();
        for (; cie!=it; ++it) {
                /*
                delete it->second;
                it->second = NULL;
                */
                (it->second)->funcType_ = FT_INVALID;
                EPMANAGER->insertUser(it->first, it->second);
                studentMap_.erase(it);
        }
}

const epStudent*
epClass::getStudentById(const int student_id) {
        EPSTUDENT_MAP::iterator it = studentMap_.begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.end();

        for (; cie!=it; ++it) {
                if (student_id == (it->second)->id_) {
                        return it->second;
                }
        }

        return NULL;
}

bool
epClass::insertStudent(const epStudent* obj) {
        if (NULL == obj) {
                printf("[DEBUG] epClass::insertStudent : NULL == obj\n");
                return false;
        }

        EPSTUDENT_MAP::iterator it = studentMap_.find(obj->fd_);
        if (studentMap_.end() != it) { // found
                return false;
        } else {
                studentMap_.insert(std::make_pair<int, epStudent*>(obj->fd_, const_cast<epStudent*>(obj)));
                return true;
        }
}

bool
epClass::removeStudentByFd(const int fd) {
        EPSTUDENT_MAP::iterator it = studentMap_.find(fd);
        if (studentMap_.end() != it) { // found
                studentMap_.erase(it);
                return true;
        } else {
                return false;
        }
}

bool
epClass::moveAllStudentToUser(void) {
        EPSTUDENT_MAP::iterator it = studentMap_.begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.end();
        for (; cie!=it; ++it) {
                // TODO : 在移动学生过程中失败怎么办。
                if (EPMANAGER->insertUser(it->first, it->second)) {
                        studentMap_.erase(it);
                } else {
                        return false;
                }
        }
        return true;
}

bool
epClass::deleteStudentByFd(const int fd) {
        EPSTUDENT_MAP::iterator it = studentMap_.find(fd);
        if (studentMap_.end() != it) {
                delete it->second;
                it->second = NULL;
                studentMap_.erase(it);
                return true;
        } else {
                return false;
        }
}

const epStudent*
epClass::getStudentByFd(const int fd) {
        EPSTUDENT_MAP::iterator it = studentMap_.find(fd);
        if (studentMap_.end() != it) { // found
                return it->second;
        } else {
                return NULL;
        }
}

const epUser*
epClass::getUserByAccount(const std::string& account) {
        EPSTUDENT_MAP::iterator it = studentMap_.begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.end();
        for (; cie!=it; ++it) {
                if (account == (it->second)->account_) {
                        return it->second;
                }
        }

        return NULL;
}

bool
epClass::sendtoAllStudent(Buf* pBuf, const bool toSelf) {
        EPSTUDENT_MAP::iterator it = studentMap_.begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.end();
        Buf* p = NULL;
        for (; cie!=it; ++it) {
                if (US_ONLINE == (it->second)->userStatus_) {
                        if (pBuf->getfd()==it->first && !toSelf) {
                                continue;
                        }
                        CLONE_BUF(p, pBuf);
                        p->setfd(it->first);
                        SINGLE->sendqueue.enqueue(p);
                }
        }

        SINGLE->bufpool.free(pBuf);
        return true;
}

bool
epClass::sendtoStudentByFd(const int fd, Buf* pBuf) {
        EPSTUDENT_MAP::iterator it = studentMap_.find(fd);
        if (studentMap_.end()!=it && US_ONLINE==(it->second)->userStatus_) { // found
                pBuf->setfd(it->first);
                SINGLE->sendqueue.enqueue(pBuf);
                return true;
        } else {
                SINGLE->bufpool.free(pBuf);
                return false;
        }
}

void
epClass::dump(void) {
        epBase::dump();
        printf("for student : \n");
        EPSTUDENT_MAP::iterator it = studentMap_.begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.end();
        for (; cie!=it; ++it) {
                (it->second)->dump();
        }
}

const std::vector<int>
epClass::getActiveStudent(void) {
        EPSTUDENT_MAP::iterator it = studentMap_.begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.end();
        std::vector<int> vi;
        for (; cie!=it; ++it) {
                if (US_ONLINE == (it->second)->userStatus_) {
                        vi.push_back((it->second)->id_);
                }
        }
        return vi;
}
