#include "epPerson.h"
#include <stdio.h> // for printf
#include "../netdef.h"

epPerson::epPerson (void) : age_(EPPERSON_INVALID_PERSON_AGE) { }
epPerson::~epPerson(void) { }

void
epPerson::dump(void) {
        printf("name     = %s\n", (lastName_ + firstName_).c_str());
        printf("sex      = %s\n", sex_.c_str());
        printf("age      = %d\n", age_);
        printf("race     = %s\n", race_.c_str());
        printf("birthday = %s\n", birthday_.c_str());
        printf("native   = %s\n", native_.c_str());
}
