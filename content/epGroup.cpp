#include "epGroup.h"
#include "../netdef.h" // for EPGROUP_INVALID_GROUP_ID
#include "../Single.h"

epGroup::epGroup(void) {
}

epGroup::~epGroup(void) {
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

const epStudent*
epGroup::getStudentByFd(const int fd) {
        STUDENT_MAP::iterator it = studentMap_.find(fd);
        if (studentMap_.end() != it) { // found
                return it->second;
        } else {
                return NULL;
        }
}

bool
epGroup::sendtoAllStudent(Buf* pBuf) {
        STUDENT_MAP::iterator it = studentMap_.begin();
        STUDENT_MAP::const_iterator cie = studentMap_.end();
        Buf* p = NULL;
        for (; cie!=it; ++it) {
                p = SINGLE->bufpool.malloc();
                p = pBuf;
                p->setfd(it->first);
                SINGLE->sendqueue.enqueue(p);
        }

        SINGLE->bufpool.free(pBuf);
        return true;
}

bool
epGroup::sendtoStudentByFd(const int fd, Buf* pBuf) {
        STUDENT_MAP::iterator it = studentMap_.find(fd);
        if (studentMap_.end() != it) { // found
                SINGLE->sendqueue.enqueue(pBuf);
                return true;
        } else {
                return false;
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
