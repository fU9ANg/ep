#include "epWhiteBoard.h"
#include "../netdef.h" // for EPWHITEBOARD_INVALID_WHITEBOARD_ID

epWhiteBoard::epWhiteBoard (void) 
		: id_(EPWHITEBOARD_INVALID_WHITEBOARD_ID) {
}
epWhiteBoard::~epWhiteBoard(void) {
}

bool epWhiteBoard::setId(const int id) {
	id_ = id;
	return true;
}

int epWhiteBoard::getId(void) const {
		return id_;
}

void
epWhiteBoard::dump(void) {
        epUser::dump();
        printf("whileboard id = %d\n", id_);
}
