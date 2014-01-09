#include "epClassroom.h"
#include "../netdef.h" // for EPCLASSROOM_INVALID_CLASSROOM_ID

epClassroom::epClassroom(void) : id_(EPCLASSROOM_INVALID_CLASSROOM_ID) {
}

epClassroom::~epClassroom(void) {
}

const int
epClassroom::getId(void) const {
        return id_;
}

const epTeacher
epClassroom::getTeacher(void) const {
        return teacher_;
}

const epWhiteBoard
epClassroom::getWhiteBoard(void) const {
        return whiteboard_;
}

bool
epClassroom::setId(const int id) {
        id_ = id;
        return true;
}

bool
epClassroom::setTeacher(const epTeacher& teacher) {
        teacher_ = teacher;
        return true;
}

bool
epClassroom::setWhiteBoard(const epWhiteBoard& whiteboard) {
        whiteboard_ = whiteboard;
        return true;
}
