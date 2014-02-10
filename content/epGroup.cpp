#include "epGroup.h"
#include "../netdef.h" // for EPGROUP_INVALID_GROUP_ID
#include "../Single.h"

epGroup::epGroup(void) : lock_() {
}

epGroup::~epGroup(void) {
        studentMap_.clear();
}

bool
epGroup::insertStudent(int fd, epStudent* student) {
        STUDENT_MAP::iterator it = studentMap_.find(fd);
        if (studentMap_.end() != it) { // found
                return false;
        } else {
                studentMap_.insert(std::make_pair<int, epStudent*>(fd, student));
                return true;
        }
}

bool
epGroup::removeStudentByFd(const int fd) {
        STUDENT_MAP::iterator it = studentMap_.find(fd);
        if (studentMap_.end() != it) { // found
                studentMap_.erase(it);
                return true;
        } else {
                return false;
        }
}

bool
epGroup::removeAllStudent(void) {
        studentMap_.clear();
        return true;
}

const epStudent*
epGroup::getStudentByFd(const int fd) {
        STUDENT_MAP::iterator it = studentMap_.find(fd);
        // printf("[DEBUG] epGroup::getStudentByFd : studentMap_.size() = %ld\n", studentMap_.size());
        if (studentMap_.end() != it) { // found
                return it->second;
        } else {
                return NULL;
        }
}

int
epGroup::getNextIdByFd(const int fd) {
        STUDENT_MAP::iterator it = studentMap_.begin();
        STUDENT_MAP::const_iterator cie = studentMap_.end();

        for (; cie!=it; ++it) {
                if (fd == (it->second)->fd_) {
                        ++it;
                        if (cie == it) {
                                ((studentMap_.begin())->second)->id_;
                        } else {
                                return (it->second)->id_;
                        }
                }
        }

        return EPSTUDENT_INVALIED_STUDENT_ID;
}

bool
epGroup::sendtoAllStudent(Buf* pBuf, const bool toSelf) {
        STUDENT_MAP::iterator it = studentMap_.begin();
        STUDENT_MAP::const_iterator cie = studentMap_.end();
        Buf* p = NULL;
        for (; cie!=it; ++it) {
                if (US_ONLINE == (it->second)->userStatus_) {
                        if (pBuf->getfd()==(it->second)->fd_ && !toSelf) { // 不发送给自己。
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
epGroup::sendtoStudentByFd(const int fd, Buf* pBuf) {
        STUDENT_MAP::iterator it = studentMap_.find(fd);
        if (studentMap_.end()!=it && US_ONLINE==(it->second)->userStatus_) { // found and online
                pBuf->setfd(it->first);
                SINGLE->sendqueue.enqueue(pBuf);
                return true;
        } else {
                SINGLE->bufpool.free(pBuf);
                return false;
        }
}

bool
epGroup::setLock(const int lock) {
        int i = 0;
        for (; i<(signed)sizeof(lock); ++i) {
                if ((lock&(1<<i)) != 0) {
                        break;
                }
        }

        if ((lock_&(1<<i)) != 0) {
                return false;
        } else {
                lock_ = lock_ | (1<<i);
                return true;
        }
}

void
epGroup::dump(void) {
        epBase::dump();
        STUDENT_MAP::iterator it = studentMap_.begin();
        STUDENT_MAP::const_iterator cie = studentMap_.end();
        for (; cie!=it; ++it) {
                (it->second)->dump();
        }
}
