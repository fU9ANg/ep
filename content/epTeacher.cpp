#include "epTeacher.h"
#include "../netdef.h" // for EPTEACHER_INVALID_TEACHER_ID

epTeacher::epTeacher(void)
        : id_(EPTEACHER_INVALID_TEACHER_ID) {
}

epTeacher::~epTeacher(void) {
}

bool
epTeacher::setId(const int id) {
        id_ = id;
        return true;
}

bool
epTeacher::setSchoolName (const std::string& schoolName) {
        schoolName_ = schoolName;
        return true;
}

bool
epTeacher::setSchoolId(const int schoolId) {
        schoolId_ = schoolId;
        return true;
}

int
epTeacher::getId(void) const {
        return id_;
}

const std::string&
epTeacher::getSchoolName (void) const {
        return schoolName_;
}

int
epTeacher::getSchoolId(void) const {
        return schoolId_;
}

void
epTeacher::dump(void) {
        printf("for user :\n");
        epUser::dump();
        printf("for person :\n");
        epPerson::dump();
        printf("teacher id  = %d\n", id_);
        printf("school name = %s\n", schoolName_.c_str());
}
