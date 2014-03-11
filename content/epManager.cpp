#include "epManager.h"
#include "../Single.h"
#include "../netdef.h"
#include "../fun_obj.hpp"
#include <stdlib.h>

#include <iostream>

epManager* epManager::p_ = NULL;

epManager* epManager::instance(void) {
        if (NULL == p_) {
                p_ = new epManager();
                srand(time(NULL));
        }

        return p_;
}

epManager::epManager(void) {
}

epManager::~epManager(void) {
}

bool
epManager::insertClassroom(epClassroom* pObj) {
        EPCLASSROOM_MAP::iterator it = classroomMap_.begin();
        EPCLASSROOM_MAP::const_iterator cie = classroomMap_.end();

        for (; cie!=it; ++it) {
                if (pObj->id_ == it->first) {
                        return false;
                }
        }

        classroomMap_.insert(std::make_pair<int, epClassroom*>(pObj->id_, pObj));
        return true;
}

bool
epManager::deleteClassroomById(int id) {
        EPCLASSROOM_MAP::iterator it = classroomMap_.find(id);
        if (classroomMap_.end() != it) { // found
                delete it->second;
                it->second = NULL;
                classroomMap_.erase(it);
                return true;
        } else {
                return false;
        }
}

bool
epManager::removeClassroomById(int id) {
        /*
        EPCLASSROOM_MAP::iterator it = classroomMap_.begin();
        EPCLASSROOM_MAP::const_iterator cie = classroomMap_.end();

        for (; cie!=it; ++it) {
                if (id == it->first) {
                        classroomMap_.erase(it);
                        return true;
                }
        }

        return false;
        */

        EPCLASSROOM_MAP::iterator it = classroomMap_.find(id);
        if (classroomMap_.end() != it) { // found
                classroomMap_.erase(it);
                return true;
        } else {
                return false;
        }
}

bool
epManager::moveStudentToUser(const int student_id) {
        epStudent* p_student = const_cast<epStudent*>(getStudentByIdFromClassroom(student_id));
        if (NULL == p_student) {
                return false;
        }

        epClass* p_class = getClassById(p_student->classId_);
        if (NULL == p_class) {
                return false;
        }

        p_class->removeStudentByFd(p_student->fd_);
        insertUser(p_student->fd_, p_student);
        return true;
}

epClassroom*
epManager::getClassroomByFd(const int fd) {
        EPCLASSROOM_MAP::iterator it = classroomMap_.begin();
        EPCLASSROOM_MAP::const_iterator cie = classroomMap_.end();

        printf("[DEBUG] epManager::getClassroomByFd : map_size = %ld\n", classroomMap_.size());
        for (; cie!=it; ++it) {
        printf("[DEBUG] epManager::getClassroomByFd : whiteboard_     = %p\n", it->second->whiteboard_);
        printf("[DEBUG] epManager::getClassroomByFd : whiteboard_->fd = %d\n", it->second->whiteboard_->fd_);
                if (NULL!=it->second && NULL!=(it->second)->class_
                                && (it->second)->class_->getStudentByFd(fd)) {
                        DEBUG_INFO;
                        return it->second;
                }

                const epTeacher* pTeacher = (it->second)->teacher_;
                if (NULL!=pTeacher && fd==pTeacher->fd_) { // 教师
                        DEBUG_INFO;
                        return it->second;
                }

                const epWhiteBoard* pWhiteboard = (it->second)->whiteboard_;
                if (NULL!=pWhiteboard && fd==pWhiteboard->fd_) { // 白板。
                        DEBUG_INFO;
                        return it->second;
                }
        }

        DEBUG_INFO;
        return NULL;
}

epClassroom*
epManager::getClassroomByUserAccount(const std::string& account, enum LoginType type) {
        EPCLASSROOM_MAP::iterator it = classroomMap_.begin();
        EPCLASSROOM_MAP::const_iterator cie = classroomMap_.end();

        for (; cie!=it; ++it) {
                if (NULL!=it->second && NULL!=(it->second)->class_
                                && (it->second)->class_->getUserByAccount(account)) {
                        return it->second;
                }

                const epTeacher* pTeacher = (it->second)->teacher_;
                if (NULL!=pTeacher && type == pTeacher->getType()
                                && 0==strncmp(pTeacher->account_.c_str(), account.c_str(), account.size())
                                && pTeacher->account_.size(), account.size()
                                ) { // 教师
                        DEBUG_INFO;
                        return it->second;
                }

                const epWhiteBoard* pWhiteboard = (it->second)->whiteboard_;
                if (NULL!=pWhiteboard && type == pWhiteboard->getType()
                                && 0==strncmp(pWhiteboard->account_.c_str(), account.c_str(), account.size())
                                && pWhiteboard->account_.size(), account.size()
                   ) { // 白板。
                        DEBUG_INFO;
                        return it->second;
                }
        }

        DEBUG_INFO;
        return NULL;
}

epClassroom*
epManager::getClassroomByClassId(const int class_id) {
        EPCLASSROOM_MAP::iterator it = classroomMap_.begin();
        EPCLASSROOM_MAP::const_iterator cie = classroomMap_.end();

        for (; cie!=it; ++it) {
                if (NULL != (it->second)->getClassById(class_id)) {
                        return it->second;
                }
        }

        return NULL;
}

epClassroom*
epManager::getClassroomById(const int classroom_id) {
        /*
        EPCLASSROOM_MAP::iterator it = classroomMap_.begin();
        EPCLASSROOM_MAP::const_iterator cie = classroomMap_.end();

        for (; cie!=it; ++it) {
                if (id == it->first) {
                        return &(it->second);
                }
        }

        return NULL;
        */

        EPCLASSROOM_MAP::iterator it = classroomMap_.find(classroom_id);
        if (classroomMap_.end() != it) { // found
                return it->second;
        } else {
                return NULL;
        }
}

const epGroup*
epManager::getGroupByFd(const int fd) {
        epClassroom* pClassroom = getClassroomByFd(fd);
        if (NULL == pClassroom) {
                return NULL;
        } else {
                const epGroup* pGroup = pClassroom->getGroupByFd(fd);
                return pGroup;
        }
}

epClass*
epManager::getClassById(const int class_id) {
        EPCLASSROOM_MAP::iterator it = classroomMap_.begin();
        EPCLASSROOM_MAP::const_iterator cie = classroomMap_.end();

        epClass* pClass = NULL;
        for (; cie!=it; ++it) {
                pClass = (it->second)->getClassById(class_id);
                if (NULL != pClass) {
                        return pClass;
                }
        }

        return NULL;
}

bool
epManager::insertUser(const int fd, epUser* user) {
        /*
           EPUSER_MAP::iterator it = userMap_.begin();
           EPUSER_MAP::const_iterator cie = userMap_.end();

           for (; cie!=it; ++it) {
           if (fd == it->first) {
           return false;
           }
           }
           */

        EPUSER_MAP::iterator it = userMap_.find(fd);
        if (userMap_.end() != it) { // found
                return false;
        } else {
                userMap_.insert(std::make_pair<int, epUser*>(fd, user));
                return true;
        }
}

epUser*
epManager::removeUserByFd(const int fd) {
        EPUSER_MAP::iterator it = userMap_.find(fd);
        if (userMap_.end() != it) { // found
                userMap_.erase(it);
                return it->second;
        } else {
                return NULL;
        }
}

const epUser*
epManager::getUserByAccount(const std::string& account, enum LoginType logintype) {
        EPUSER_MAP::iterator it = userMap_.begin();
        EPUSER_MAP::const_iterator cie = userMap_.end();

        for (; cie!=it; ++it) {
                if (0 == strcmp(account.c_str(), ((it->second)->account_).c_str())
                                && logintype == (it->second)->getType()) {
                        return it->second;
                }
        }

        return NULL;
}

const epUser*
epManager::getUserByAccountFromClassroom(const std::string& account, enum LoginType logintype) {
        epUser* pUser = const_cast<epUser*>(getUserByAccount(account, logintype));
        if (NULL == pUser) {
                EPCLASSROOM_MAP::iterator it = classroomMap_.begin();
                EPCLASSROOM_MAP::const_iterator cie = classroomMap_.end();
                for (; cie!=it; ++it) {
                        pUser = const_cast<epUser*>((it->second)->getUserByAccount(account, logintype));
                        if (NULL != pUser) {
                                return pUser;
                        }
                }
        }

        return pUser;
}

const epUser*
epManager::getUserByFd(const int fd) {
        EPUSER_MAP::iterator it = userMap_.find(fd);
        return (userMap_.end()!=it) ? it->second : NULL;
}

const epUser*
epManager::getUserByFdFromClassroom(const int fd) {
        EPCLASSROOM_MAP::iterator it  = classroomMap_.begin();
        EPCLASSROOM_MAP::const_iterator cie  = classroomMap_.end();
        epUser* pUser = NULL;
        for (; cie!=it; ++it) {
                pUser = const_cast<epUser*>((it->second)->getUserByFd(fd));
                if (NULL != pUser) {
                        return pUser;
                }
        }

        return NULL;
}

const epStudent*
epManager::getStudentByIdFromUser(const int student_id) {
        EPUSER_MAP::iterator it = userMap_.begin();
        EPUSER_MAP::const_iterator cie = userMap_.end();

        for (; cie!=it; ++it) {
                if (LT_STUDENT == (it->second)->getType()
                                && student_id == (it->second)->id_) {
                        return dynamic_cast<epStudent*>(it->second);
                }
        }

        return NULL;
}

const epStudent*
epManager::getStudentByIdFromClassroom(const int id) {
        EPCLASSROOM_MAP::iterator it = classroomMap_.begin();
        EPCLASSROOM_MAP::const_iterator cie = classroomMap_.end();

        epStudent* pStudent = NULL;
        for (; cie!=it; ++it) {
                if (NULL!=it->second && NULL!=(it->second)->class_
                                && (pStudent=const_cast<epStudent*>((it->second)->class_->getStudentById(id))))
                        break;
        }

        return pStudent;
}

const epStudent*
epManager::getStudentById(const int student_id) {
        epStudent* pStudent = NULL;
        return (pStudent = const_cast<epStudent*>(getStudentByIdFromUser(student_id)))
                ? pStudent // found
                : const_cast<epStudent*>(getStudentByIdFromClassroom(student_id));
}

const epTeacher*
epManager::getTeacherByFd(const int fd) {
        EPCLASSROOM_MAP::iterator it = classroomMap_.begin();
        EPCLASSROOM_MAP::const_iterator cie = classroomMap_.end();

        for (; cie!=it; ++it) {
                if (NULL != (it->second)->teacher_) {
                        if (fd == (it->second)->teacher_->fd_) {
                                return (it->second)->teacher_;
                        }
                }
        }

        return NULL;
}

const epTeacher*
epManager::getTeacherByIdFromUser(const int id) {
        EPUSER_MAP::iterator it = userMap_.begin();
        EPUSER_MAP::const_iterator cie = userMap_.end();

        for (; cie!=it; ++it) {
                if (LT_TEACHER == (it->second)->getType()
                                && id == (it->second)->id_) {
                        return dynamic_cast<epTeacher*>(it->second);
                }
        }

        return NULL;
}

const epTeacher*
epManager::getTeacherByIdFromClassroom(const int teacher_id) {
        EPCLASSROOM_MAP::iterator it = classroomMap_.begin();
        EPCLASSROOM_MAP::const_iterator cie = classroomMap_.end();

        for (; cie!=it; ++it) {
                if (NULL != (it->second)->teacher_) {
                        if (teacher_id == (it->second)->teacher_->id_) {
                                return (it->second)->teacher_;
                        }
                }
        }

        return NULL;
}

std::vector<int>
epManager::getActiveStudentListFromClass(const int class_id) {
        epClass* pClass = getClassById(class_id);
        if (NULL != pClass) {
                return pClass->getActiveStudent();
        } else {
                std::vector<int> vi;
                return vi;
        }
}

bool
epManager::insertStudentFromUserIntoClass(epClass* p_class) {
        if (NULL == p_class) return false;

        EPUSER_MAP::iterator it = userMap_.begin();
        EPUSER_MAP::const_iterator cie = userMap_.end();

        epStudent* pStudent = NULL;
        for (; cie!=it; ++it) {
                if (FT_SCHOOL == (it->second)->funcType_ && LT_STUDENT == (it->second)->getType()) {
                        pStudent = dynamic_cast<epStudent*>(it->second);
                        if (NULL!=pStudent && p_class->id_==pStudent->classId_) {
                                p_class->insertStudent(pStudent);
                                userMap_.erase(it);
                        }
                }
        }

        return true;
}

bool
epManager::insertStudentFromUserIntoClassroom(epClassroom* p_classroom) {
        return (NULL!=p_classroom) ? insertStudentFromUserIntoClass(p_classroom->class_) : false;
}

bool
epManager::insertWhiteboardFromUserIntoClassroom(epClassroom* p_classroom) {
        if (NULL == p_classroom)
                return false;

        EPUSER_MAP::iterator it = find_if(userMap_.begin(), userMap_.end(), is_whiteboard(p_classroom->id_));
        if (userMap_.end()!=it && NULL!=it->second) {
                p_classroom->whiteboard_ = dynamic_cast<epWhiteBoard*>(it->second);
                userMap_.erase(it);
                return true;
        } else {
                return false;
        }
}

const epTeacher*
epManager::getTeacherById(const int teacher_id) {
        /*
           epTeacher* pTeacher = getTeacherByIdFromUser(teacher_id);
           if (NULL != pTeacher) {
           return pTeacher;
           }

           pTeacher = getTeacherByIdFromClassroom(teacher_id);
           return pTeacher;
           */

        epTeacher* pTeacher = NULL;
        return (NULL != (pTeacher=const_cast<epTeacher*>(getTeacherByIdFromUser(teacher_id))))
                ? pTeacher                                 // found from userMap_
                : const_cast<epTeacher*>(getTeacherByIdFromClassroom(teacher_id)); // found from Classroom
}

bool
epManager::sendtoAllClassroom(Buf* pBuf, const bool toSelf) {
        EPCLASSROOM_MAP::iterator it = classroomMap_.begin();
        EPCLASSROOM_MAP::const_iterator cie = classroomMap_.end();
        Buf* p = NULL;
        for (; cie!=it; ++it) {
                CLONE_BUF(p, pBuf);
                (it->second)->sendtoAll(p, toSelf);
        }

        SINGLE->bufpool.free(pBuf);
        return true;
}

bool
epManager::sendtoClassroomById(const int classroom_id, Buf* pBuf) {
        EPCLASSROOM_MAP::iterator it = classroomMap_.find(classroom_id);
        if (classroomMap_.end() != it) { // found
                (it->second)->sendtoAllClass(pBuf);
                return true;
        } else {
                SINGLE->bufpool.free(pBuf);
                return false;
        }
}

bool
epManager::sendtoClassFromUser(Buf* pBuf, const int class_id) {
        EPUSER_MAP::iterator it = userMap_.begin();
        EPUSER_MAP::const_iterator cie = userMap_.end();
        Buf* p = NULL;
        epStudent* pStudent = NULL;
        for (; cie!=it; ++it) {
                pStudent = dynamic_cast<epStudent*>(it->second);
                if (NULL!=pStudent && class_id==pStudent->classId_) {
                        CLONE_BUF(p, pBuf);
                        p->setfd(it->first);
                        SINGLE->sendqueue.enqueue(p);
                }
        }

        SINGLE->bufpool.free(pBuf);
        return true;
}

void
epManager::sendtoWhiteboardFromUser(Buf* p_buf, const int classroom_id) {
        EPUSER_MAP::iterator it = userMap_.begin();
        EPUSER_MAP::const_iterator cie = userMap_.end();
        epWhiteBoard* p_whiteboard = NULL;
        for (; cie!=it; ++it) {
                p_whiteboard = dynamic_cast<epWhiteBoard*>(it->second);
                if (NULL!=p_whiteboard && classroom_id==p_whiteboard->classroomId_) {
                        p_buf->setfd(it->first);
                        SINGLE->sendqueue.enqueue(p_buf);
                        return;
                }
        }

        SINGLE->bufpool.free(p_buf);
        return;
}

bool
epManager::sendtoAllUser(Buf* pBuf, const bool toSelf) {
        EPUSER_MAP::iterator it = userMap_.begin();
        EPUSER_MAP::const_iterator cie = userMap_.end();
        Buf* p = NULL;
        for (; cie!=it; ++it) {
                if (US_ONLINE == (it->second)->userStatus_) {
                        if (pBuf->getfd()==it->first && !toSelf) { // sendto self ?
                                continue;
                        }
                        CLONE_BUF(p, pBuf);
                        p->setfd(it->first);
                        SINGLE->sendqueue.enqueue(p);
                }
        }

        SINGLE->bufpool.free(pBuf);
        return true;
}

bool
epManager::sendtoUserByFd(const int fd, Buf* pBuf) {
        EPUSER_MAP::iterator it = userMap_.find(fd);
        if (userMap_.end() != it) { // found
                SINGLE->sendqueue.enqueue(pBuf);
                return true;
        } else {
                SINGLE->bufpool.free(pBuf);
                return false;
        }
}

void
epManager::dumpClassroom(void) {
        /*
           EPCLASSROOM_MAP::iterator it = classroomMap_.begin();
           EPCLASSROOM_MAP::const_iterator cie = classroomMap_.end();
           for (; cie!=it; ++it) {
           (it->second)->dump();
           }
           */

        // for_each(classroomMap_.begin(), classroomMap_.end(), dump_obj());
        for_each(classroomMap_.begin(), classroomMap_.end(), fun_obj<dump_operation, void>(dump_operation()));
}

void
epManager::dumpUser(void) {
        /*
           EPUSER_MAP::iterator it = userMap_.begin();
           EPUSER_MAP::const_iterator cie = userMap_.end();
           for (; cie!=it; ++it) {
           (it->second)->dump();
           }
           */
        // for_each(userMap_.begin(), userMap_.end(), dump_obj());
        for_each(userMap_.begin(), userMap_.end(), fun_obj<dump_operation, void>(dump_operation()));
}
