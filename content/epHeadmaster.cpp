#include "epHeadmaster.h"
#include "stdio.h" // for printf

bool
epHeadmaster::init(const std::string& account
                 , const std::string& passwd
                ) {
        return true;
}

void
epHeadmaster::dump(void) {
        epUser::dump();
        epPerson::dump();
        printf("for headmaster :\n");
        printf("school id   = %d\n", schoolId_);
        printf("school name = %s\n", schoolName_.c_str());
}
