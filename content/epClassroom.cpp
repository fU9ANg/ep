#include "epClassroom.h"
#include "../Single.h"
#include "../netdef.h" // for EPCLASSROOM_INVALID_CLASSROOM_ID

epClassroom::epClassroom(const int classroom_id=EPCLASSROOM_INVALID_CLASSROOM_ID) : id_(classroom_id) {
}

epClassroom::~epClassroom(void) {
}

const int
epClassroom::getId(void) const {
        return id_;
}

const epStudent*
epClassroom::getStudentById(const int student_id) {
        EPCLASS_MAP::iterator it = classMap_.begin();
        EPCLASS_MAP::const_iterator cie = classMap_.end();

        epStudent* pStudent = NULL;
        for (; cie!=it; ++it) {
                pStudent = const_cast<epStudent*>((it->second).getStudentById(student_id));
                if (NULL != pStudent) {
                        return pStudent;
                }
        }

        return NULL;
}

const epTeacher*
epClassroom::getTeacher(void) {
        return teacher_;
}

const epWhiteBoard*
epClassroom::getWhiteBoard(void) const {
        return whiteboard_;
}

const std::string&
epClassroom::getCourseList(void) const {
        return courseList_;
}

bool
epClassroom::setId(const int id) {
        id_ = id;
        return true;
}

bool
epClassroom::setTeacher(const epTeacher* teacher) {
        teacher_ = const_cast<epTeacher*>(teacher);
        return true;
}

bool
epClassroom::setWhiteBoard(const epWhiteBoard* whiteboard) {
        whiteboard_ = const_cast<epWhiteBoard*>(whiteboard);
        return true;
}

bool
epClassroom::setCourseList(const std::string& courseList) {
        courseList_ = courseList;
        return true;
}

bool
epClassroom::sendtoAllClass(Buf* pBuf) {
        EPCLASS_MAP::iterator it = classMap_.begin();
        EPCLASS_MAP::const_iterator cie = classMap_.end();
        Buf* p = NULL;
        for (; cie!=it; ++it) {
                p = SINGLE->bufpool.malloc();
                p = pBuf;
                (it->second).sendtoAllStudent(p);
        }

        SINGLE->bufpool.free(pBuf);
        return true;
}

bool
epClassroom::sendtoClassById(const int class_id, Buf* pBuf) {
        EPCLASS_MAP::iterator it = classMap_.find(class_id);
        if (classMap_.end() != it) { // found
                (it->second).sendtoAllStudent(pBuf);
                return true;
        } else {
                return false;
        }
}

bool
epClassroom::sendtoAllGroup(Buf* pBuf) {
        EPGROUP_MAP::iterator it = groupMap_.begin();
        EPGROUP_MAP::const_iterator cie = groupMap_.end();
        Buf* p = NULL;
        for (; cie!=it; ++it) {
                p = SINGLE->bufpool.malloc();
                p = pBuf;
                (it->second).sendtoAllStudent(p);
        }

        SINGLE->bufpool.free(pBuf);
        return true;
}

bool
epClassroom::sendtoGroupById(const int group_id, Buf* pBuf) {
        EPGROUP_MAP::iterator it = groupMap_.find(group_id);
        if (groupMap_.end() != it) { // found
                (it->second).sendtoAllStudent(pBuf);
                return true;
        } else {
                return false;
        }
}

bool
epClassroom::sendtoTeacher(Buf* pBuf) {
        pBuf->setfd(teacher_->getFd());
        SINGLE->sendqueue.enqueue(pBuf);
        return true;
}

bool
epClassroom::sendtoWhiteBoard(Buf* pBuf) {
        pBuf->setfd(whiteboard_->getFd());
        SINGLE->sendqueue.enqueue(pBuf);
        return true;
}

bool
epClassroom::insertClass(epClass& obj) {
        EPCLASS_MAP::iterator it = classMap_.find(obj.getId());
        if (classMap_.end() != it) { // found
                return false;
        } else {
                classMap_.insert(std::make_pair<int, epClass>(obj.getId(), obj));
                return true;
        }
}

bool
epClassroom::removeClassById(const int class_id) {
        EPCLASS_MAP::iterator it = classMap_.find(class_id);
        if (classMap_.end() != it) { // found
                classMap_.erase(it);
                return true;
        } else {
                return false;
        }
}

epClass*
epClassroom::getClassById(const int class_id) {
        EPCLASS_MAP::iterator it = classMap_.find(class_id);
        if (classMap_.end() != it) { // found
                return &(it->second);
        } else {
                return NULL;
        }
}

std::vector<epClass*>
epClassroom::getClassList(void) {
        std::vector<epClass*> vc;
        EPCLASS_MAP::iterator it = classMap_.begin();
        EPCLASS_MAP::const_iterator cie = classMap_.end();
        for (; cie!=it; ++it) {
                vc.push_back(&(it->second));
        }
        return vc;
}

bool
epClassroom::insertGroup(const epGroup& obj) {
        EPGROUP_MAP::iterator it = groupMap_.find(obj.getId());
        if (groupMap_.end() != it) { // found
                return false;
        } else {
                groupMap_.insert(std::make_pair<int, epGroup>(obj.getId(), obj));
                return true;
        }
}

bool
epClassroom::removeGroupById(const int group_id) {
        EPGROUP_MAP::iterator it = groupMap_.find(group_id);
        if (groupMap_.end() != it) { // found
                groupMap_.erase(it);
                return true;
        } else {
                return false;
        }
}

epGroup*
epClassroom::getGroupById(const int group_id) {
        EPGROUP_MAP::iterator it = groupMap_.find(group_id);
        if (groupMap_.end() != it) { // found
                return &(it->second);
        } else {
                return NULL;
        }
}

void
epClassroom::dump(void) {
        printf("==================================================\n");
        printf("class room id = %d\n", id_);
        printf("for class :\n");
        EPCLASS_MAP::iterator it = classMap_.begin();
        EPCLASS_MAP::const_iterator cie = classMap_.end();
        for (; cie!=it; ++it) {
                (it->second).dump();
        }

        printf("for group :\n");
        EPGROUP_MAP::iterator it_g = groupMap_.begin();
        EPGROUP_MAP::const_iterator cie_g = groupMap_.end();
        for (; cie_g!=it_g; ++it_g) {
                (it_g->second).dump();
        }

        printf("for teacher :\n");
        teacher_->dump();

        printf("for whiteboard_ :\n");
        whiteboard_->dump();
}
