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
epTeacher::setschoolName (const std::string& schoolName) {
        schoolName_ = schoolName;
        return true;
}

int
epTeacher::getId(void) const {
        return id_;
}

const std::string&
epTeacher::getschoolName (void) const {
        return schoolName_;
}

void
epTeacher::dump(void) {
        epUser::dump();
        epPerson::dump();
        printf("teacher id  = %d\n", id_);
        printf("school name = %s\n", schoolName_.c_str());
}
