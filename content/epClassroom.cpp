#include "epClassroom.h"
#include "../Single.h"
#include "../netdef.h" // for EPCLASSROOM_INVALID_CLASSROOM_ID
#include "epManager.h"

epClassroom::epClassroom(void)
          : teacher_()
          , whiteboard_()
{
}

epClassroom::~epClassroom(void) {
        EPCLASS_MAP::iterator it = classMap_.begin();
        EPCLASS_MAP::const_iterator cie = classMap_.end();
        for (; cie!=it; ++it) {
                delete it->second;
                it->second = NULL;
                classMap_.erase(it);
        }

        EPGROUP_MAP::iterator itg = groupMap_.begin();
        EPGROUP_MAP::const_iterator cieg = groupMap_.end();
        for (; cieg!=itg; ++itg) {
                delete itg->second;
                itg->second = NULL;
                groupMap_.erase(itg);
        }

        delete teacher_;
        teacher_ = NULL;
        delete whiteboard_;
        whiteboard_ = NULL;
}

const epStudent*
epClassroom::getStudentById(const int student_id) {
        EPCLASS_MAP::iterator it = classMap_.begin();
        EPCLASS_MAP::const_iterator cie = classMap_.end();

        epStudent* pStudent = NULL;
        for (; cie!=it; ++it) {
                pStudent = const_cast<epStudent*>((it->second)->getStudentById(student_id));
                if (NULL != pStudent) {
                        return pStudent;
                }
        }

        return NULL;
}

const epStudent*
epClassroom::getStudentByFd(const int fd) {
        EPCLASS_MAP::iterator it = classMap_.begin();
        EPCLASS_MAP::const_iterator cie = classMap_.end();
        epStudent* pStudent = NULL;
        for (; cie!=it; ++it) {
                pStudent = const_cast<epStudent*>((it->second)->getStudentByFd(fd));
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

const epUser*
epClassroom::getUserByFd(const int fd) {
        if (NULL!=teacher_ && fd==teacher_->getFd()) {
                return teacher_;
        }

        if (NULL!=whiteboard_ && fd==whiteboard_->getFd()) {
                return whiteboard_;
        }

        /*
        epUser* pUser = const_cast<epUser*>(EPMANAGER->getUserByFdFromClassroom(fd));
        if (NULL != pUser) {
                epStudent* pStudent = dynamic_cast<epStudent*>(pUser);
                if (NULL != pStudent) {
                        EPCLASS_MAP::iterator it = classMap_.find(pStudent->getClassId());
                        if (classMap_.end() != it) { // found
                                printf("[DEBUG] %s : before return student point\n", __func__);
                                return (it->second)->getStudentByFd(fd);
                        }
                }
        }
        */
        

        EPCLASS_MAP::iterator it = classMap_.begin();
        EPCLASS_MAP::const_iterator cie = classMap_.end();
        epStudent* pStudent = NULL;
        for (; cie!=it; ++it) {
                pStudent = const_cast<epStudent*>((it->second)->getStudentByFd(fd));
                if (NULL != pStudent) {
                        return pStudent;
                }
        }

        return NULL;
}

bool
epClassroom::deleteUserByFd(const int fd) {
        if (NULL!=teacher_ && fd==teacher_->getFd()) {
                delete teacher_;
                teacher_ = NULL;
                return true;
        }

        if (NULL!=whiteboard_ && fd==whiteboard_->getFd()) {
                delete whiteboard_;
                whiteboard_ = NULL;
                return true;
        }

        EPCLASS_MAP::iterator it = classMap_.begin();
        EPCLASS_MAP::const_iterator cie = classMap_.end();
        for (; cie!=it; ++it) {
                if ((it->second)->deleteStudentByFd(fd)) {
                        return true;
                }
        }

        return false;
}

const std::string&
epClassroom::getCourseList(void) const {
        return courseList_;
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
epClassroom::sendtoAllStudent(Buf* pBuf, const bool toSelf) {
        EPCLASS_MAP::iterator it = classMap_.begin();
        EPCLASS_MAP::const_iterator cie = classMap_.end();
        for (; cie!=it; ++it) {
                (it->second)->sendtoAllStudent(pBuf, toSelf);
        }

        return true;
}

bool
epClassroom::sendtoTeacher(Buf* pBuf) {
        if (NULL != teacher_) {
                pBuf->setfd(teacher_->getFd());
        }
        SINGLE->sendqueue.enqueue(pBuf);
        return true;
}

bool
epClassroom::sendtoWhiteBoard(Buf* pBuf) {
        if (NULL != whiteboard_) {
                pBuf->setfd(whiteboard_->getFd());
        }
        SINGLE->sendqueue.enqueue(pBuf);
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
                (it->second)->sendtoAllStudent(p);
        }

        SINGLE->bufpool.free(pBuf);
        return true;
}

bool
epClassroom::sendtoClassById(const int class_id, Buf* pBuf) {
        EPCLASS_MAP::iterator it = classMap_.find(class_id);
        if (classMap_.end() != it) { // found
                (it->second)->sendtoAllStudent(pBuf);
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
                (it->second)->sendtoAllStudent(p);
        }

        SINGLE->bufpool.free(pBuf);
        return true;
}

bool
epClassroom::sendtoGroupById(const int group_id, Buf* pBuf) {
        EPGROUP_MAP::iterator it = groupMap_.find(group_id);
        if (groupMap_.end() != it) { // found
                (it->second)->sendtoAllStudent(pBuf);
                return true;
        } else {
                return false;
        }
}

bool
epClassroom::insertClass(epClass* obj) {
        EPCLASS_MAP::iterator it = classMap_.find(obj->getId());
        if (classMap_.end() != it) { // found
                return false;
        } else {
                classMap_.insert(std::make_pair<int, epClass*>(obj->getId(), obj));
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
                return it->second;
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
                vc.push_back(it->second);
        }
        return vc;
}

bool
epClassroom::insertGroup(epGroup* obj) {
        EPGROUP_MAP::iterator it = groupMap_.find(obj->getId());
        if (groupMap_.end() != it) { // found
                return false;
        } else {
                groupMap_.insert(std::make_pair<int, epGroup*>(obj->getId(), obj));
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

bool
epClassroom::deleteGroupById(const int group_id) {
        EPGROUP_MAP::iterator it = groupMap_.find(group_id);
        if (groupMap_.end() != it) {
                delete it->second;
                it->second = false;
                groupMap_.erase(it);
                return true;
        } else {
                return false;
        }
}

bool
epClassroom::deleteAllGroup(void) {
        EPGROUP_MAP::iterator it = groupMap_.begin();
        EPGROUP_MAP::const_iterator cie = groupMap_.end();
        for (; cie!=it; ++it) {
                delete it->second;
                it->second = NULL;
                groupMap_.erase(it);
        }

        return true;
}

epGroup*
epClassroom::getGroupById(const int group_id) {
        EPGROUP_MAP::iterator it = groupMap_.find(group_id);
        if (groupMap_.end() != it) { // found
                return it->second;
        } else {
                return NULL;
        }
}

void
epClassroom::dump(void) {
        epBase::dump();
        printf("for class :\n");
        EPCLASS_MAP::iterator it = classMap_.begin();
        EPCLASS_MAP::const_iterator cie = classMap_.end();
        for (; cie!=it; ++it) {
                (it->second)->dump();
        }

        printf("for group :\n");
        EPGROUP_MAP::iterator it_g = groupMap_.begin();
        EPGROUP_MAP::const_iterator cie_g = groupMap_.end();
        for (; cie_g!=it_g; ++it_g) {
                (it_g->second)->dump();
        }

        printf("for teacher :\n");
        if (NULL != teacher_) {
                teacher_->dump();
        }

        printf("for whiteboard_ :\n");
        if (NULL != whiteboard_) {
                whiteboard_->dump();
        }
}
