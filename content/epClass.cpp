#include "epClass.h"
#include "../Single.h"
#include "../netdef.h" // EPCLASSROOM_INVALID_CLASSROOM_ID
#include <stdio.h> // for printf
#include "epManager.h"
#include "../fun_obj.hpp"

epClass::epClass(void) { }

epClass::~epClass(void) {
        EPSTUDENT_MAP::iterator it = studentMap_.get_obj().begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.get_obj().end();
        for (; cie!=it; ++it) {
                if (NULL == it->second)
                        continue;
                (it->second)->funcType_ = FT_INVALID;
                EPMANAGER->insertUser(it->first, it->second);
                studentMap_.get_obj().erase(it);
        }

        studentMap_.my_for_each(fun_obj<delete_operation, void>(delete_operation()));
}

const epStudent*
epClass::getStudentById(const int student_id) {
        return studentMap_.get_obj_by_other(fun_obj<compare_operation<int>, bool>(compare_operation<int>(student_id)));
}

bool
epClass::insertStudent(const epStudent* obj) {
        bool flag = (NULL!=obj && studentMap_.insert(obj->fd_, const_cast<epStudent*>(obj)));
        return flag;
}

bool
epClass::removeStudentByFd(const int fd) {
        return NULL != studentMap_.erase_by_key(fd);
}

bool
epClass::moveAllStudentToUser(void) {
        EPSTUDENT_MAP::iterator it = studentMap_.get_obj().begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.get_obj().end();
        for (; cie!=it; ++it) {
                // TODO : 在移动学生过程中失败怎么办。
                if (EPMANAGER->insertUser(it->first, it->second)) {
                        studentMap_.get_obj().erase(it);
                } else {
                        return false;
                }
        }
        return true;
}

bool
epClass::deleteStudentByFd(const int fd) {
        delete studentMap_.erase_by_key(fd);
        return true;
}

const epStudent*
epClass::getStudentByFd(const int fd) {
        epStudent* p_student = studentMap_.get_obj_by_key(fd);
        if (NULL == p_student) {
                EPSTUDENT_LIST::iterator it = offlineStudentList_.value().begin();
                EPSTUDENT_LIST::const_iterator cie = offlineStudentList_.value().end();
                for (; cie!=it; ++it) {
                        if (fd == (*it)->fd_) {
                                return *it;
                        }
                }
        }

        return p_student;

        /*
        EPSTUDENT_MAP::iterator it = studentMap_.get_obj().find(fd);
        if (studentMap_.get_obj().end() != it) { // found
                return it->second;
        } else {
                EPSTUDENT_LIST::iterator it = offlineStudentList_.get_obj().begin();
                EPSTUDENT_LIST::const_iterator cie = offlineStudentList_.get_obj().end();
                for (; cie!=it; ++it) {
                        if (fd == (*it)->fd_) {
                                return *it;
                        }
                }
                return NULL;
        }
        */
}

const epUser*
epClass::getUserByAccount(const std::string& account) {
        epStudent* p_student = studentMap_.get_obj_by_other(fun_obj<compare_operation<std::string>, bool>(compare_operation<std::string>(account)));
        if (NULL == p_student) {
                p_student = offlineStudentList_.get_obj_by(fun_obj<compare_operation<std::string>, bool>(compare_operation<std::string>(account)));
        }

        return p_student;
}

bool
epClass::sendtoAllStudent(Buf* pBuf, const bool toSelf) {
        // studentMap_.my_for_each(fun_obj<send_to_user, void>(send_to_user(pBuf, toSelf)));

        EPSTUDENT_MAP::iterator it = studentMap_.get_obj().begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.get_obj().end();
        Buf* p = NULL;
        for (; cie!=it; ++it) {
                if (US_ONLINE == (it->second)->userStatus_) {
                        if (pBuf->getfd()==it->first && !toSelf) {
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
epClass::sendtoStudentByFd(const int fd, Buf* pBuf) {
        fun_obj<send_to_user, void>(send_to_user(pBuf, true))(studentMap_.get_obj_by_key(fd));
        return true;
}

void
epClass::dump(void) {
        epBase::dump();
        printf("for student : \n");
        studentMap_.my_for_each(fun_obj<dump_operation, void>(dump_operation()));
}

const std::vector<int>
epClass::getActiveStudent(void) {
        EPSTUDENT_MAP::iterator it = studentMap_.get_obj().begin();
        EPSTUDENT_MAP::const_iterator cie = studentMap_.get_obj().end();
        std::vector<int> vi;
        for (; cie!=it; ++it) {
                if (US_ONLINE == (it->second)->userStatus_) {
                        vi.push_back((it->second)->id_);
                }
        }
        return vi;
}

bool
epClass::moveStudentFromMapToListByFd(const int fd) {
        EPSTUDENT_MAP::iterator it = studentMap_.get_obj().find(fd);
        if (studentMap_.get_obj().end() != it) {
                offlineStudentList_.value().push_back(it->second);
                studentMap_.get_obj().erase(it);
                return true;
        } else {
                return false;
        }
}


bool
epClass::moveStudentFromListToMapByAccount(const int fd, const std::string& account) {
        // TODO :
        epStudent* p_student = offlineStudentList_.get_obj_by(fun_obj<compare_operation<std::string>, bool>(compare_operation<std::string>(account)));
        if (NULL != p_student) {
                p_student->fd_ = fd;
                insertStudent(p_student);
                offlineStudentList_.erase_by(fun_obj<compare_operation<std::string>, bool>(compare_operation<std::string>(account)));
                return true;
        } else {
                return false;
        }
}
