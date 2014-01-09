#include "epStudent.h"
#include "../netdef.h" // for EPSTUDENT_INVALIED_ID

epStudent::epStudent(void) : id_(EPSTUDENT_INVALIED_STUDENT_ID) {
}

epStudent::~epStudent(void) {
}

bool
epStudent::setId(const int id) {
		  id_ = id;
		  return true;
}

bool
epStudent::setClassName (const std::string& className) {
		  className_ = className;
		  return true;
}

int
epStudent::getId(void) const {
		  return id_;
}

const std::string
epStudent::getClassName (void) const {
		  return className_;
}

void
epStudent::dump(void) {
        epUser::dump();
        epPerson::dump();
        printf("student id = %d\n", id_);
        printf("class id   = %d\n", classId_);
        printf("class name = %s\n", className_.c_str());
}
