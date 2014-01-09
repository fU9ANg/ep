#include "epSchool.h"
#include "../netdef.h" // for EPSCHOOL_INVALIED_SCHOOL_ID

epSchool::epSchool(void) : id_(EPSCHOOL_INVALIED_SCHOOL_ID) {
}

epSchool::~epSchool(void) {
}

int
epSchool::getSchoolId(void) const {
		  return id_;
}

const std::string
epSchool::getSchoolName(void) const {
		  return schoolName_;
}

const std::string
epSchool::getNative(void) const {
		  return native_;
}

bool
epSchool::setSchoolId(const int id) {
		  id_ = id;
		  return true;
}

bool
epSchool::setSchoolName(const std::string& schoolname) {
		  schoolName_ = schoolname;
		  return true;
}

bool
epSchool::setSchoolNative(const std::string& native) {
		  native_ = native;
		  return true;
}

