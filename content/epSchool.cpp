#include <stdio.h> // for printf
#include "epSchool.h"

epSchool::epSchool(void) {
}

epSchool::~epSchool(void) {
}

const std::string
epSchool::getNative(void) const {
        return native_;
}

bool
epSchool::setSchoolNative(const std::string& native) {
        native_ = native;
        return true;
}

void
epSchool::dump(void) {
        epBase::dump();
        printf("for school :\n");
        printf("school native = %s\n", native_.c_str());
}
