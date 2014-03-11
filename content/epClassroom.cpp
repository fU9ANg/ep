#include "epClassroom.h"
#include "../Single.h"
#include "../netdef.h" // for EPCLASSROOM_INVALID_CLASSROOM_ID
#include "epManager.h"

epClassroom::epClassroom(void) : teacher_(), whiteboard_(), undisplay_(false), class_() {
        for (int i=0; i<EPCLASSROOM_MAX_COURSEWARE_IDX; ++i) {
                coursewareVector_.push_back(CS_INVALID);
        }
}

epClassroom::~epClassroom(void) {
        delete class_;
        class_ = NULL;

        EPGROUP_MAP::iterator itg = groupMap_.begin();
        EPGROUP_MAP::const_iterator cieg = groupMap_.end();
        for (; cieg!=itg; ++itg) {
                delete itg->second;
                itg->second = NULL;
                groupMap_.erase(itg);
        }

        MOVE_USER_TO_USER_MAP(teacher_);
        MOVE_USER_TO_USER_MAP(whiteboard_);
}

const epStudent*
epClassroom::getStudentById(const int student_id) {
        return (NULL!=class_) ? class_->getStudentById(student_id) : NULL;
}

const epUser*
epClassroom::getUserByFd(const int fd) {
        if (NULL!=teacher_ && fd==teacher_->fd_) {
                return teacher_;
        }

        if (NULL!=whiteboard_ && fd==whiteboard_->fd_) {
                return whiteboard_;
        }

        return class_->getStudentByFd(fd);
}

const epUser*
epClassroom::getUserByAccount(const std::string& account, enum LoginType logintype) {
        if (NULL!=teacher_ && account==teacher_->account_ && logintype == teacher_->getType()) {
                return teacher_;
        }

        if (NULL!=whiteboard_ && account==whiteboard_->account_ && logintype == whiteboard_->getType()) {
                DEBUG_INFO;
                return whiteboard_;
        }

        return (NULL!=class_) ? const_cast<epUser*>(class_->getUserByAccount(account)) : NULL;
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

        return class_->deleteStudentByFd(fd);
}

bool
epClassroom::sendtoAllClass(Buf* pBuf, const bool toSelf) {
        return (NULL!=class_) ? class_->sendtoAllStudent(pBuf, toSelf) : false;
}

void
epClassroom::sendtoAll(Buf* pBuf, const bool toSelf) {
        Buf* p1 = NULL;
        Buf* p2 = NULL;

        CLONE_BUF(p1, pBuf);
        sendtoAllClass(p1, toSelf);
        CLONE_BUF(p2, pBuf);
        sendtoTeacher(p2, toSelf); 
        sendtoWhiteBoard(pBuf, toSelf);
        return;
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

        // std::cout << std::hex << std::showbase << "teacher_ = " << teacher_ << ", userStatus_ = " << teacher_->userStatus_ << std::endl;
        if (NULL!=teacher_ && US_ONLINE==teacher_->userStatus_) {
                printf("[DEBUG] epClassroom::sendtoTeacher : NULL != teacher_, msg_type = %d\n", ((MSG_HEAD*)pBuf->ptr())->cType);
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

        // std::cout << std::hex << std::showbase << "whiteboard_ = " << whiteboard_ << ", userStatus_ = " << whiteboard_->userStatus_ << std::endl;
        if (NULL!=whiteboard_ && US_ONLINE==whiteboard_->userStatus_) {
                printf("[DEBUG] epClassroom::sendtoTeacher : NULL != whiteboard_, msg_type = %d\n", ((MSG_HEAD*)pBuf->ptr())->cType);
                pBuf->setfd(whiteboard_->fd_);
                SINGLE->sendqueue.enqueue(pBuf);
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
        if (NULL == class_) {
                class_ = obj;
                return true;
        } else
                return false;
}

bool
epClassroom::insertStudent(const epStudent* obj) {
        return NULL!=class_ && class_->insertStudent(obj);
}

bool
epClassroom::removeClassById(const int class_id) {
        if (NULL!=class_ && class_id==class_->id_) {
                delete class_;
                class_ = NULL;
                return true;
        } else
                return false;
}

bool
epClassroom::moveAllStudentToUser(void) {
        return class_->moveAllStudentToUser();
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
        return class_;
}

std::vector<epClass*>
epClassroom::getClassList(void) {
        std::vector<epClass*> ve;
        if (NULL != class_)
                ve.push_back(class_);
        return ve;
}

const epGroup*
epClassroom::getGroupByFd(const int fd) {
        EPGROUP_MAP::iterator it = groupMap_.begin();
        EPGROUP_MAP::const_iterator cie = groupMap_.end();
        // printf("[DEBUG] epClassroom::getGroupByFd : group_size = %ld\n", groupMap_.size());
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
                } else {
                        delete it->second;
                }
        }

        groupMap_.clear();

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
        if (NULL != class_)
                class_->dump();

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

int
epClassroom::getClassId(void) {
        if (NULL != class_)
                return class_->id_;
        return EPCLASS_INVALID_CLASS_ID;
}

void
epClassroom::changeCoursewareStatus(int idx, courseware_status cs) {

        /*
        COURSEWARE_STATUS_MAP::iterator it = coursewareStatusMap_.find(courseware_id);
        if (coursewareStatusMap_.end() != it) {
                (it->second) = cs;
        } else {
                coursewareStatusMap_.insert(std::make_pair<int, courseware_status>(courseware_id, cs));
        }
        */

        if (idx > EPCLASSROOM_MAX_COURSEWARE_IDX)
                printf("[DEBUG] file=%s : func=%s : line=%d\n", __FILE__, __func__, __LINE__);
        else
                coursewareVector_[idx] = cs;

}

/*
void
epClassroom::removeCourseware(const int courseware_id) {
        COURSEWARE_STATUS_MAP::iterator it = coursewareStatusMap_.find(courseware_id);
        if (coursewareStatusMap_.end() != it) {
                coursewareStatusMap_.erase(it);
        }
}
*/

std::vector<int> epClassroom::getGroupList(void) {
    EPGROUP_MAP::iterator it = groupMap_.begin();
    EPGROUP_MAP::const_iterator cie = groupMap_.end();
    std::vector<int> vi;
    for(; it!=cie; ++it){
        if(it->second != NULL) {
            vi.push_back(it->first);
        }
    }
    return vi;
}

bool
epClassroom::initWhiteboard(void) {
        // whiteboard_->whiteboardNum_ = ???
        whiteboard_->id_            = class_->id_;
        whiteboard_->classroomId_   = id_;
        whiteboard_->classroomName_ = name_;
        return whiteboard_->init(teacher_->account_
                        , teacher_->passwd_
                        , EPUSER_INVALID_USER_ID
                        , US_OFFLINE);
}
