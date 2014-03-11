#include "epGroup.h"
#include "../netdef.h" // for EPGROUP_INVALID_GROUP_ID
#include "../Single.h"
#include <stdlib.h>

epGroup::epGroup(void) : random_(rand()) {
}

epGroup::~epGroup(void) {
        studentMap_.clear();
}

bool
epGroup::insertStudent(int fd, epStudent* student) {
        // printf("[DEBUG] epGroup::insertStudent : before studentMap_.size() = %ld, fd = %d\n", studentMap_.size(), fd);
                bool result = (studentMap_.insert(std::make_pair<int, epStudent*>(fd, student))).second;
        // printf("[DEBUG] epGroup::insertStudent : after  studentMap_.size() = %ld, fd = %d\n", studentMap_.size(), fd);
        return result;
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

const epStudent*
epGroup::getNextStudentByFd(const int fd) {
        STUDENT_MAP::iterator it = studentMap_.begin();
        STUDENT_MAP::const_iterator cie = studentMap_.end();

        for (; cie!=it; ++it) {
                if (fd == (it->second)->fd_) {
                        ++it;
                        if (cie == it) {
                                return (studentMap_.begin())->second; // 从头开始。
                        } else {
                                return it->second; // 下一个。
                        }
                }
        }

        return NULL;
}

std::vector<int>
epGroup::getStudentList(void) {
        STUDENT_MAP::iterator it = studentMap_.begin();
        STUDENT_MAP::const_iterator cie = studentMap_.end();
        printf("[DEBUG] epGroup::getStudentList : map_size = %ld\n", studentMap_.size());
        std::vector<int> vi;
        for (; cie!=it; ++it) {
                vi.push_back((it->second)->id_);
        }
        return vi;
}

bool
epGroup::sendtoAllStudent(Buf* pBuf, const bool toSelf) { // true
        STUDENT_MAP::iterator it = studentMap_.begin();
        STUDENT_MAP::const_iterator cie = studentMap_.end();
        Buf* p = NULL;
        for (; cie!=it; ++it) {
                if (US_ONLINE == (it->second)->userStatus_) {
                        if (pBuf->getfd()==(it->second)->fd_ && !toSelf) { // 不发送给自己。
                                DEBUG_INFO;
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
epGroup::lock(const int lock_id) {
        /*
           printf ("lock_&(1<<lock_id) = %d, 1<<lock_id = %d\n", lock_&(1<<lock_id), (1<<lock_id));
           if ((lock_&(1<<lock_id)) != 0) { // 已经被占用。
           return false;
           } else {
           printf ("lock:  1 lock_=%d\n", lock_);
           lock_ |= (1<<lock_id);
           printf ("lock:  2 lock_=%d\n", lock_);
           return true;
           }
           */

        LOCK_MAP::iterator it = lockMap_.find(lock_id);
        if (lockMap_.end() != it) { // found
                if (it->second == MS_LOCK) {
                        return false;
                } else {
                        it->second = MS_LOCK;
                        return true;
                }
        } else {
                return (lockMap_.insert(std::make_pair<int, mutex_status>(lock_id, MS_LOCK))).second;
        }
}

bool
epGroup::unlock(const int lock_id) {
        LOCK_MAP::iterator it = lockMap_.find(lock_id);
        if (lockMap_.end() != it) { // found
                if (it->second == MS_LOCK) {
                        it->second = MS_UNLOCK;
                        return true;
                }
        }
        return false;

        /*
           if ((lock_&(1<<lock_id)) != 0) {
           printf ("unlock:  1 lock_=%d\n", lock_);
           lock_ ^= (1<<lock_id);
           printf ("unlock:  2 lock_=%d\n", lock_);
           return true;
           } else {
           return false;
           }
           */
}

bool
epGroup::isLock(const int lock_id) {
        LOCK_MAP::iterator it = lockMap_.find(lock_id);
        return (lockMap_.end() != it && it->second == MS_LOCK);
}

void
epGroup::cleanLock(void) {
        lockMap_.clear();
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
