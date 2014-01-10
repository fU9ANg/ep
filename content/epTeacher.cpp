#include "epTeacher.h"
#include <stdio.h> // for printf
#include "../netdef.h" // for EPTEACHER_INVALID_TEACHER_ID

epTeacher::epTeacher(void) {
}

epTeacher::~epTeacher(void) {
}

bool
epTeacher::setTeacherNum(const std::string& teacherNum) {
        teacherNum_ = teacherNum;
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

bool
epTeacher::init(const std::string& account, const std::string& passwd) {
        epUser::init(account, passwd);
        // TODO :
        return true;
}

std::string
epTeacher::getTeacherNum(void) const {
        return teacherNum_;
}

const std::string&
epTeacher::getSchoolName (void) const {
        return schoolName_;
}

int
epTeacher::getSchoolId(void) const {
        return schoolId_;
}

enum LoginType
epTeacher::getType(void) const {
        return LT_TEACHER;
}

void
epTeacher::dump(void) {
        printf("for user :\n");
        epUser::dump();
        printf("for person :\n");
        epPerson::dump();
        printf("teacher number  = %s\n", teacherNum_.c_str());
        printf("school name     = %s\n", schoolName_.c_str());
}
