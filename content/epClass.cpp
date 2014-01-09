#include "epClass.h"
#include "../netdef.h" // for EPCLASS_INVALID_CLASS_ID
#include <stdio.h> // for printf

epClass::epClass(void) : id_(EPCLASS_INVALID_CLASS_ID) {
}

epClass::~epClass(void) {
}

int
epClass::getClassId(void) {
        return id_;
}

bool
epClass::insertStudent(int fd, const epStudent& student) {
        EPSTUDENT_MAP::iterator it = studentMap_.begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.end();

        for (; cie!=it; ++it) {
                if (fd == it->first) {
                        return false;
                }
        }

        studentMap_.insert(std::make_pair<int, epStudent>(fd, student));
        return true;
}

bool
epClass::removeStudentByFd(const int fd) {
        EPSTUDENT_MAP::iterator it = studentMap_.begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.end();

        for (; cie!=it; ++it) {
                if (fd == it->first) {
                        studentMap_.erase(it);
                        return true;
                }
        }

        return false;
}

const epStudent*
epClass::getStudentByFd(const int fd) {
        EPSTUDENT_MAP::iterator it = studentMap_.begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.end();

        for (; cie!=it; ++it) {
                if (fd == it->first) {
                        return &(it->second);
                }
        }

        return NULL;
}

bool
epClass::sendtoAllStudent(Buf* pBuf) {
        EPSTUDENT_MAP::iterator it = studentMap_.begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.end();
        Buf* p = NULL;
        for (; cie!=it; ++it) {
                if (pBuf->getfd() == it->first) { // not send to self.
                        continue;
                }
                p = pBuf;
                p->setfd(it->first);
                SINGLE->sendqueue.enqueue(p);
        }

        SINGLE->bufpool.free(pBuf);
        return true;
}

bool
epClass::sendtoStudent(const int fd, Buf* pBuf) {
        EPSTUDENT_MAP::iterator it = studentMap_.begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.end();
        for (; cie!=it; ++it) {
                if (fd == it->first) {
                        pBuf->setfd(it->first);
                        SINGLE->sendqueue.enqueue(pBuf);
                        return true;
                }
        }

        // SINGLE->bufpool.free(pBuf); // ??? TODO
        return false;
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
