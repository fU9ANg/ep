#include "epClass.h"
#include "../Single.h"
#include "../netdef.h" // EPCLASSROOM_INVALID_CLASSROOM_ID
#include <stdio.h> // for printf

epClass::epClass(const int class_id=EPCLASSROOM_INVALID_CLASSROOM_ID) : id_(class_id) {
}

epClass::~epClass(void) {
}

int
epClass::getId(void) {
        return id_;
}

const epStudent*
epClass::getStudentById(const int student_id) {
        EPSTUDENT_MAP::iterator it = studentMap_.begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.end();

        for (; cie!=it; ++it) {
                if (student_id == (it->second).getId()) {
                        return &(it->second);
                }
        }

        return NULL;
}

bool
epClass::insertStudent(const int fd, const epStudent& student) {
        EPSTUDENT_MAP::iterator it = studentMap_.find(fd);
        if (studentMap_.end() != it) { // found
                return false;
        } else {
                studentMap_.insert(std::make_pair<int, epStudent>(fd, student));
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

const epStudent*
epClass::getStudentByFd(const int fd) {
        EPSTUDENT_MAP::iterator it = studentMap_.find(fd);
        if (studentMap_.end() != it) { // found
                return &(it->second);
        } else {
                return NULL;
        }
}

bool
epClass::sendtoAllStudent(Buf* pBuf, const bool toSelf) {
        EPSTUDENT_MAP::iterator it = studentMap_.begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.end();
        Buf* p = NULL;
        for (; cie!=it; ++it) {
                if (!toSelf && pBuf->getfd()==it->first) { // send to self?
                        continue;
                }
                p = SINGLE->bufpool.malloc();
                p = pBuf;
                p->setfd(it->first);
                SINGLE->sendqueue.enqueue(p);
        }

        SINGLE->bufpool.free(pBuf);
        return true;
}

bool
epClass::sendtoStudentByFd(const int fd, Buf* pBuf) {
        EPSTUDENT_MAP::iterator it = studentMap_.find(fd);
        if (studentMap_.end() != it) { // found
                pBuf->setfd(it->first);
                SINGLE->sendqueue.enqueue(pBuf);
                return true;
        } else {
                return false;
        }
}

void
epClass::dump(void) {
        printf("class id = %d\n", id_);
        printf("for student : \n");
        EPSTUDENT_MAP::iterator it = studentMap_.begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.end();
        for (; cie!=it; ++it) {
                (it->second).dump();
        }
}

const std::vector<int>
epClass::getActiveStudent(void) {
        EPSTUDENT_MAP::iterator it = studentMap_.begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.end();
        std::vector<int> vi;
        for (; cie!=it; ++it) {
                vi.push_back((it->second).getId());
        }
        return vi;
}
