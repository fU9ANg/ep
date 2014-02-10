#include <stdio.h> // for printf
#include "epWhiteBoard.h"

epWhiteBoard::epWhiteBoard (void) { }

epWhiteBoard::~epWhiteBoard(void) { }

bool
epWhiteBoard::init(const std::string& account
                 , const std::string& passwd
                 ) {
        epUser::init(account, passwd);
        // TODO :
        return true;
}

const enum LoginType
epWhiteBoard::getType(void) const {
        return LT_WHITEBOARD;
}

void
epWhiteBoard::dump(void) {
        epUser::dump();
        printf("whileboard number         = %s\n", whiteboardNum_.c_str());
        printf("whiteboard classroom id   = %d\n", classroomId_);
        printf("whiteboard classroom name = %s\n", classroomName_.c_str());
}
