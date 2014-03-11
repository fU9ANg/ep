#include <stdio.h> // for printf
#include "epWhiteBoard.h"

epWhiteBoard::epWhiteBoard (void) { }

epWhiteBoard::~epWhiteBoard(void) { }

bool
epWhiteBoard::init(const std::string& account
                 , const std::string& passwd
                 , const int fd
                 , const enum user_status us
                 ) {
        return epUser::init(account, passwd, fd, us);

        /*
        id_;
        fd_;
        whiteboardNum_;
        classroomName_;
        classroomId_;
        */
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
