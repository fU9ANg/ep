#include "epBase.h"
#include <stdio.h> // for printf

epBase::epBase(void) { }
epBase::~epBase(void) { }

void epBase::dump(void) {
        printf("id      = %d\n", id_);
        printf("name_   = %s\n", name_.c_str());
        printf("isUsed_ = %s\n", isUsed_ ? "true" : "false");
}
