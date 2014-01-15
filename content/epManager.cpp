#include "epManager.h"
#include "../Single.h"
#include <iostream>

epManager* epManager::p_ = NULL;

epManager* epManager::instance(void) {
        if (NULL == p_) {
                p_ = new epManager();
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
                if (pObj->getId() == it->first) {
                        return false;
                }
        }

        classroomMap_.insert(std::make_pair<int, epClassroom*>(pObj->getId(), pObj));
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

epClassroom*
epManager::getClassroomByFd(const int fd) {
        EPCLASSROOM_MAP::iterator it = classroomMap_.begin();
        EPCLASSROOM_MAP::const_iterator cie = classroomMap_.end();
        for (; cie!=it; ++it) {
                if (NULL != (it->second)->getStudentByFd(fd)) { // 学生
                        return it->second;
                }

                const epTeacher* pTeacher = (it->second)->getTeacher();
                if (NULL!=pTeacher && fd==pTeacher->getFd()) { // 教室。
                        return it->second;
                }

                const epWhiteBoard* pWhiteboard = (it->second)->getWhiteBoard();
                if (NULL!=pWhiteboard && fd==pWhiteboard->getFd()) { // 白板。
                        return it->second;
                }
        }

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

bool
epManager::removeUserByFd(const int fd) {
        /*
        EPUSER_MAP::iterator it = userMap_.begin();
        EPUSER_MAP::const_iterator cie = userMap_.end();

        for (; cie!=it; ++it) {
                if (fd == it->first) {
                        userMap_.erase(it);
                        return true;
                }
        }

        return false;
        */

        EPUSER_MAP::iterator it = userMap_.find(fd);
        if (userMap_.end() != it) { // found
                userMap_.erase(it);
                return true;
        } else {
                return false;
        }
}

const epUser*
epManager::getUserByAccount(const std::string& account) {
        EPUSER_MAP::iterator it = userMap_.begin();
        EPUSER_MAP::const_iterator cie = userMap_.end();

        for (; cie!=it; ++it) {
                if (account == (it->second)->getAccount()) {
                        return it->second;
                }
        }

        return NULL;
}

const epUser*
epManager::getUserByFd(const int fd) {
        /*
        EPUSER_MAP::iterator it = userMap_.begin();
        EPUSER_MAP::const_iterator cie = userMap_.end();
        for (; cie!=it; ++it) {
                if (fd == it->first) {
                        return it->second;
                }
        }

        return NULL;
        */
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
                                && student_id == (it->second)->getId()) {
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
                pStudent = const_cast<epStudent*>((it->second)->getStudentById(id));
                if (NULL != pStudent) {
                        return pStudent;
                }
        }

        return NULL;
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
                if (fd == (it->second)->getTeacher()->getFd()) {
                        return (it->second)->getTeacher();
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
                                && id == (it->second)->getId()) {
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
                if (teacher_id == (it->second)->getTeacher()->getId()) {
                        return (it->second)->getTeacher();
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
epManager::insertStudentFromUserIntoClass(const int class_id) {
        epClass* pClass = getClassById(class_id);

        EPUSER_MAP::iterator it = userMap_.begin();
        EPUSER_MAP::const_iterator cie = userMap_.end();

        epStudent* pStudent = NULL;
        for (; cie!=it; ++it) {
                if (FT_SCHOOL == (it->second)->getFuncType() && LT_STUDENT == (it->second)->getType()) {
                        pStudent = dynamic_cast<epStudent*>(it->second);
                        if (class_id == pStudent->getClassId()) {
                                pClass->insertStudent(it->first, pStudent);
                                userMap_.erase(it);
                        }
                }
        }

        return true;
}

bool
epManager::insertStudentFromUserIntoClassroom(const int classroom_id) {
        epClassroom* pClassroom = getClassroomById(classroom_id);
        if (NULL != pClassroom) {
                std::vector<epClass*> vc = pClassroom->getClassList();
                for(int i=0; i<(signed)vc.size(); ++i) {
                        insertStudentFromUserIntoClass(vc[i]->getId());
                }
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
epManager::sendtoAllClassroom(Buf* pBuf) {
        EPCLASSROOM_MAP::iterator it = classroomMap_.begin();
        EPCLASSROOM_MAP::const_iterator cie = classroomMap_.end();
        Buf* p = NULL;
        for (; cie!=it; ++it) {
                p = SINGLE->bufpool.malloc();
                p = pBuf;
                (it->second)->sendtoAllClass(p);
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
                return false;
        }
}

bool
epManager::sendtoAllUser(Buf* pBuf, const bool toSelf=false) {
        EPUSER_MAP::iterator it = userMap_.begin();
        EPUSER_MAP::const_iterator cie = userMap_.end();
        Buf* p = NULL;
        for (; cie!=it; ++it) {
                if (!toSelf && pBuf->getfd()==it->first) { // sendto self ?
                        continue;
                }
                p = SINGLE->bufpool.malloc();
                p = pBuf;
                p->setfd(it->first);
                SINGLE->sendqueue.enqueue(p);
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
                return false;
        }
}

void
epManager::dumpClassroom(void) {
        EPCLASSROOM_MAP::iterator it = classroomMap_.begin();
        EPCLASSROOM_MAP::const_iterator cie = classroomMap_.end();
        for (; cie!=it; ++it) {
                (it->second)->dump();
        }
}

void
epManager::dumpUser(void) {
        EPUSER_MAP::iterator it = userMap_.begin();
        EPUSER_MAP::const_iterator cie = userMap_.end();
        for (; cie!=it; ++it) {
                (it->second)->dump();
        }
}
