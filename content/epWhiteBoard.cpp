#include <stdio.h> // for printf
#include "epWhiteBoard.h"

epWhiteBoard::epWhiteBoard (void) {
}

epWhiteBoard::~epWhiteBoard(void) {
}

bool
epWhiteBoard::setWhiteboardNum(const std::string& whiteboardNum) {
        whiteboardNum_ = whiteboardNum;
	return true;
}

bool
epWhiteBoard::init(const std::string& account, const std::string& passwd) {
        epUser::init(account, passwd);
        // TODO :
        return true;
}

std::string
epWhiteBoard::getWhiteboardNum(void) const {
		return whiteboardNum_;
}

enum LoginType
epWhiteBoard::getType(void) const {
        return LT_WHITEBOARD;
}

void
epWhiteBoard::dump(void) {
        epUser::dump();
        printf("whileboard number = %s\n", whiteboardNum_.c_str());
}
