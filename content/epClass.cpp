#include "epClass.h"
#include "../netdef.h" // for EPCLASS_INVALID_CLASS_ID

epClass::epClass(void) : id_(EPCLASS_INVALID_CLASS_ID) {
}

epClass::~epClass(void) {
}

int
epClass::getClassId(void) {
        return id_;
}

bool
epClass::insertStudent(const epStudent& student) {
        EPSTUDENT_MAP::iterator it = studentMap_.begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.end();

        for (; cie!=it; ++it) {
                if (student.getId() == it->first) {
                        return false;
                }
        }

        studentMap_.insert(std::make_pair<int, epStudent>(student.getId(), student));
        return true;
}

bool
epClass::deleteStudentById(const int id) {
        EPSTUDENT_MAP::iterator it = studentMap_.begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.end();

        for (; cie!=it; ++it) {
                if (id == it->first) {
                        studentMap_.erase(it);
                        return true;
                }
        }

        return false;
}

const epStudent*
epClass::getStudentById(const int id) {
        EPSTUDENT_MAP::iterator it = studentMap_.begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.end();

        for (; cie!=it; ++it) {
                if (id == it->first) {
                        return &(it->second);
                }
        }

        return NULL;
}
