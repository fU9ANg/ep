#include "epStudent.h"
#include <stdio.h> // for printf

epStudent::epStudent(void) {
}

epStudent::~epStudent(void) {
}

bool
epStudent::setStudentNum(const std::string& studentNum) {
        studentNum_ = studentNum;
        return true;
}

bool
epStudent::setClassName (const std::string& className) {
        className_ = className;
        return true;
}

bool
epStudent::init(const std::string& account, const std::string& passwd) {
        epUser::init(account, passwd);
        // TODO :
        return true;
}

std::string
epStudent::getStudentNum(void) const {
        return studentNum_;
}

const std::string
epStudent::getClassName (void) const {
        return className_;
}

int
epStudent::getClassId(void) const {
        return classId_;
}

enum LoginType
epStudent::getType(void) const {
        return LT_STUDENT;
}

void
epStudent::dump(void) {
        printf("for user :\n");
        epUser::dump();
        printf("for person :\n");
        epPerson::dump();
        printf("for student :\n");
        printf("student number = %s\n", studentNum_.c_str());
        printf("class id       = %d\n", classId_);
        printf("class name     = %s\n", className_.c_str());
}
