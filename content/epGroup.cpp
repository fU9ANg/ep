#include "epGroup.h"
#include "../netdef.h" // for EPGROUP_INVALID_GROUP_ID
#include "../Single.h"

epGroup::epGroup(void) : id_(EPGROUP_INVALID_GROUP_ID) {
}

epGroup::~epGroup(void) {
}

bool
epGroup::setId(const int id) {
        id_ = id;
        return true;
}

bool
epGroup::insertStudent(const epStudent& student) {
        STUDENT_MAP::iterator it = studentMap_.begin();
        STUDENT_MAP::const_iterator cie = studentMap_.end();

        for (; cie!=it; ++it) {
                if (student.getId() == it->first) {
                        return false;
                }
        }

        studentMap_.insert(std::make_pair<int, epStudent>(student.getId(), student));
        return true;
}

bool
epGroup::deleteStudentById(const int id) {
        STUDENT_MAP::iterator it = studentMap_.begin();
        STUDENT_MAP::const_iterator cie = studentMap_.end();

        for (; cie!=it; ++it) {
                if (id == it->first) {
                        studentMap_.erase(it);
                        return true;
                }
        }
        return false;
}

int
epGroup::getId(void) const {
        return id_;
}

const epStudent*
epGroup::getStudentById(const int id) {
        STUDENT_MAP::iterator it = studentMap_.begin();
        STUDENT_MAP::const_iterator cie = studentMap_.end();

        for (; cie!=it; ++it) {
                if (id == it->first) {
                        return &(it->second);
                }
        }
        return false;
}

bool
epGroup::sendtoGroup(Buf* pBuf) {
        STUDENT_MAP::iterator it = studentMap_.begin();
        STUDENT_MAP::const_iterator cie = studentMap_.end();

        for (; cie!=it; ++it) {
                Buf* p = SINGLE->bufpool.malloc();
                p = pBuf;
                SINGLE->sendqueue.enqueue(p);
        }

        return true;
}
