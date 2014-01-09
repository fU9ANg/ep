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

bool
epClassroom::sendtoAllClass(Buf* pBuf) {
        EPCLASS_MAP::iterator it = classMap_.begin();
        EPCLASS_MAP::const_iterator cie = classMap_.end();
        Buf* p = NULL;
        for (; cie!=it; ++it) {
                p = pBuf;
                (it->second).sendtoAllStudent(p);
        }

        SINGLE->bufpool.free(pBuf);
        return true;
}

bool
epClassroom::sendtoClass(const int id, Buf* pBuf) {
        EPCLASS_MAP::iterator it = classMap_.begin();
        EPCLASS_MAP::const_iterator cie = classMap_.end();
        for (; cie!=it; ++it) {
                if (id == it->first) {
                        (it->second).sendtoAllStudent(pBuf);
                        return true;
                }
        }

        return false;
}

bool
epClassroom::sendtoAllGroup(Buf* pBuf) {
        EPGROUP_MAP::iterator it = groupMap_.begin();
        EPGROUP_MAP::const_iterator cie = groupMap_.end();
        Buf* p = NULL;
        for (; cie!=it; ++it) {
                p = pBuf;
                (it->second).sendtoAllStudent(pBuf);
        }

        SINGLE->bufpool.free(pBuf);
        return true;
}

bool
epClassroom::sendtoGroup(const int id, Buf* pBuf) {
        EPGROUP_MAP::iterator it = groupMap_.begin();
        EPGROUP_MAP::const_iterator cie = groupMap_.end();
        for (; cie!=it; ++it) {
                if (id == it->first) {
                        (it->second).sendtoAllStudent(pBuf);
                        return true;
                }
        }

        return false;
}

bool
epClassroom::sendtoTeacher(Buf* pBuf) {
}

bool
epClassroom::sendtoWhiteBoard(Buf* pBuf) {
}

void
epClassroom::dump(void) const {
        printf("class room id = %d\n", id_);
        printf("for class :\n");
        class_.dump();

        printf("for group :\n");
        GROUP_MAP::iterator it = groupMap_.begin();
        GROUP_MAP::const_iterator cie = groupMap_.end();
        for (; cie!=it; ++it) {
                (it->second).dump();
        }

        printf("for teacher :\n");
        teacher_.dump();

        printf("for whiteboard_ :\n");
        whiteboard_.dump();
}
