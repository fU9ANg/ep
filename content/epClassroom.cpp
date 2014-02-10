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

        /*
        delete teacher_;
        teacher_ = NULL;
        delete whiteboard_;
        whiteboard_ = NULL;
        */

        if (NULL != teacher_) {
                teacher_->funcType_ = FT_INVALID;
                EPMANAGER->insertUser(teacher_->fd_, teacher_);
        }

        if (NULL != whiteboard_) {
                whiteboard_->funcType_ = FT_INVALID;
                EPMANAGER->insertUser(whiteboard_->fd_, whiteboard_);
        }
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

const epUser*
epClassroom::getUserByFd(const int fd) {
        if (NULL!=teacher_ && fd==teacher_->fd_) {
                return teacher_;
        }

        if (NULL!=whiteboard_ && fd==whiteboard_->fd_) {
                return whiteboard_;
        }

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

const epUser*
epClassroom::getUserByAccount(const std::string& account) {
        if (NULL!=teacher_ && account==teacher_->account_) {
                return teacher_;
        }

        if (NULL!=whiteboard_ && account==whiteboard_->account_) {
                return whiteboard_;
        }

        EPCLASS_MAP::iterator it = classMap_.begin();
        EPCLASS_MAP::const_iterator cie = classMap_.end();
        epUser* pUser = NULL;
        for (; cie!=it; ++it) {
                pUser = const_cast<epUser*>((it->second)->getUserByAccount(account));
                if (NULL != pUser) {
                        return pUser;
                }
        }

        return NULL;
}

bool
epClassroom::deleteUserByFd(const int fd) {
        if (NULL!=teacher_ && fd==teacher_->fd_) {
                delete teacher_;
                teacher_ = NULL;
                return true;
        }

        if (NULL!=whiteboard_ && fd==whiteboard_->fd_) {
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

bool
epClassroom::sendtoAllClass(Buf* pBuf, const bool toSelf) {
        EPCLASS_MAP::iterator it = classMap_.begin();
        EPCLASS_MAP::const_iterator cie = classMap_.end();
        Buf* p = NULL;
        for (; cie!=it; ++it) {
                CLONE_BUF(p, pBuf);
                (it->second)->sendtoAllStudent(p, toSelf);
        }

        SINGLE->bufpool.free(pBuf);
        return true;
}

bool
epClassroom::sendtoAll(Buf* pBuf, const bool toSelf) {
        Buf* p1 = NULL;
        Buf* p2 = NULL;

        CLONE_BUF(p1, pBuf);
        CLONE_BUF(p2, pBuf);
        return sendtoAllClass(p1, toSelf) && sendtoTeacher(p2, toSelf) && sendtoWhiteBoard(pBuf, toSelf);
}

bool
epClassroom::sendtoTeacher(Buf* pBuf, const bool toSelf) {
        /*
        if (NULL!=pBuf && NULL!=teacher_ && (toSelf || pBuf->getfd()!=teacher_->fd_)) {
                pBuf->setfd(teacher_->fd_);
                SINGLE->sendqueue.enqueue(pBuf);
                return true;
        } else {
                SINGLE->bufpool.free(pBuf);
                return false;
        }
        */

        if (NULL!=teacher_ && US_ONLINE==teacher_->userStatus_) {
                pBuf->setfd(teacher_->fd_);
                SINGLE->sendqueue.enqueue(pBuf);
                return true;
        } else {
                SINGLE->bufpool.free(pBuf);
                return false;
        }
}

bool
epClassroom::sendtoWhiteBoard(Buf* pBuf, const bool toSelf) {
        /*
        if (NULL!=pBuf && NULL!=teacher_ && (toSelf || pBuf->getfd()!=teacher_->fd_)) {
                pBuf->setfd(teacher_->fd_);
                SINGLE->sendqueue.enqueue(pBuf);
                return true;
        } else {
                SINGLE->bufpool.free(pBuf);
                return false;
        }
        */

        if (NULL!=whiteboard_ && US_ONLINE==whiteboard_->userStatus_) {
                pBuf->setfd(whiteboard_->fd_);
                SINGLE->sendqueue.enqueue(pBuf);
                return true;
        } else {
                SINGLE->bufpool.free(pBuf);
                return false;
        }
}

bool
epClassroom::sendtoClassById(const int class_id, Buf* pBuf, const bool toSelf) {
        EPCLASS_MAP::iterator it = classMap_.find(class_id);
        if (classMap_.end() != it) { // found
                (it->second)->sendtoAllStudent(pBuf, toSelf);
                return true;
        } else {
                SINGLE->bufpool.free(pBuf);
                return false;
        }
}

bool
epClassroom::sendtoAllGroup(Buf* pBuf, const bool toSelf) {
        EPGROUP_MAP::iterator it = groupMap_.begin();
        EPGROUP_MAP::const_iterator cie = groupMap_.end();
        Buf* p = NULL;
        for (; cie!=it; ++it) {
                CLONE_BUF(p, pBuf);
                (it->second)->sendtoAllStudent(p, toSelf);
        }

        SINGLE->bufpool.free(pBuf);
        return true;
}

bool
epClassroom::sendtoGroupById(const int group_id, Buf* pBuf, const bool toSelf) {
        EPGROUP_MAP::iterator it = groupMap_.find(group_id);
        if (groupMap_.end() != it) { // found
                (it->second)->sendtoAllStudent(pBuf, toSelf);
                return true;
        } else {
                SINGLE->bufpool.free(pBuf);
                return false;
        }
}

bool
epClassroom::insertClass(epClass* obj) {
        EPCLASS_MAP::iterator it = classMap_.find(obj->id_);
        if (classMap_.end() != it) { // found
                return false;
        } else {
                classMap_.insert(std::make_pair<int, epClass*>(obj->id_, obj));
                return true;
        }
}

bool
epClassroom::insertStudent(const epStudent* obj) {
        EPCLASS_MAP::iterator it = classMap_.find(obj->classId_);
        if (classMap_.end() != it) {
                (it->second)->insertStudent(obj);
                return true;
        } else {
                return false;
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

bool
epClassroom::moveAllStudentToUser(void) {
        EPCLASS_MAP::iterator it = classMap_.begin();
        EPCLASS_MAP::const_iterator cie = classMap_.end();
        for (; cie!=it; ++it) {
                (it->second)->moveAllStudentToUser();
        }

        return true;
}

bool
epClassroom::moveWhiteboardToUser(void) {
        if (NULL != whiteboard_) {
                if (EPMANAGER->insertUser(whiteboard_->fd_, whiteboard_)) {
                        whiteboard_ = NULL;
                        return true;
                } else {
                        return false;
                }
        }
        return true;
}

bool
epClassroom::moveTeacherToUser(void) {
        if (NULL != teacher_) {
                if (EPMANAGER->insertUser(teacher_->fd_, teacher_)) {
                        teacher_ = NULL;
                        return true;
                } else {
                        return false;
                }
        }
        return true;
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

const epGroup*
epClassroom::getGroupByFd(const int fd) {
        EPGROUP_MAP::iterator it = groupMap_.begin();
        EPGROUP_MAP::const_iterator cie = groupMap_.end();
        // printf("[DEBUG] epClassroom::getGroupByFd : groupMap_.size() = %ld\n", groupMap_.size());
        for (; cie!=it; ++it) {
                if (NULL != (it->second)->getStudentByFd(fd)) {
                        return it->second;
                }
        }

        return NULL;
}

bool
epClassroom::insertGroup(epGroup* obj) {
        EPGROUP_MAP::iterator it = groupMap_.find(obj->id_);
        if (groupMap_.end() != it) { // found
                return false;
        } else {
                groupMap_.insert(std::make_pair<int, epGroup*>(obj->id_, obj));
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
epClassroom::removeAllGroup(void) {
        EPGROUP_MAP::iterator it = groupMap_.begin();
        EPGROUP_MAP::const_iterator cie = groupMap_.end();
        for (; cie!=it; ++it) {
                if (!(it->second)->removeAllStudent()) {
                        return false;
                }
        }

        return true;
}

bool
epClassroom::deleteGroupById(const int group_id) {
        EPGROUP_MAP::iterator it = groupMap_.find(group_id);
        if (groupMap_.end() != it) {
                delete it->second;
                it->second = NULL;
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
        // printf("[DEBUG] epClassroom::getGroupById : groupMap_.size() = %ld\n", groupMap_.size());
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
