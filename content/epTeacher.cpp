#include "epTeacher.h"
#include <stdio.h> // for printf
#include "database.h"
#include "SQL.h"
#include "../netdef.h" // for EPTEACHER_INVALID_TEACHER_ID

epTeacher::epTeacher(void) {
}

epTeacher::~epTeacher(void) {
}

bool
epTeacher::setTeacherNum(const std::string& teacherNum) {
        teacherNum_ = teacherNum;
        return true;
}

bool
epTeacher::setSchoolName (const std::string& schoolName) {
        schoolName_ = schoolName;
        return true;
}

bool
epTeacher::setSchoolId(const int schoolId) {
        schoolId_ = schoolId;
        return true;
}

bool
epTeacher::setFuncType(const enum FuncType funcType) {
        switch (funcType) {
        case FT_OFFICE :
        case FT_SCHOOL :
                funcType_ = funcType;
                return true;
                break;
        default :
                break;
        }

        funcType_ = FT_INVALID;
        return false;
}

std::string
epTeacher::getTeacherNum(void) const {
        return teacherNum_;
}

const std::string&
epTeacher::getSchoolName (void) const {
        return schoolName_;
}

int
epTeacher::getSchoolId(void) const {
        return schoolId_;
}

const enum LoginType
epTeacher::getType(void) const {
        return LT_TEACHER;
}

const std::string
epTeacher::getNum(void) {
        return teacherNum_;
}

void
epTeacher::dump(void) {
        epUser::dump();
        epPerson::dump();
        printf("teacher number  = %s\n", teacherNum_.c_str());
        printf("school name     = %s\n", schoolName_.c_str());
        printf("school id       = %d\n", schoolId_);
}

bool
epTeacher::init(const std::string& account, const std::string& passwd) {
        epUser::init(account, passwd);
        bool result = false;
        std::string strpwd;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_TEA_INFO_BY_ACCOUNT);
                pstmt->setString (1, account);
                ResultSet* prst = pstmt->executeQuery ();
                while (prst->next ()) {
                        strpwd = prst->getString ("password");
                        if (0 == strncmp(passwd.c_str(), strpwd.c_str(), strpwd.size()) && passwd.size() > 0) {

                                printf("teacher: login success!\n"); // ADD student_name to CLASS
                                setName (prst->getString("last_name"), prst->getString ("first_name"));
                                setSex (prst->getString ("sex"));
                                setAge (prst->getInt ("age"));
                                setRace (prst->getString ("race_name"));
                                //setBirthday (prst->getString ("birthday"));
                                setNative (prst->getString ("native_name"));

                                setId         (prst->getInt    ("teacher_id"));
                                setTeacherNum (prst->getString ("teacher_id"));
                                setSchoolId (prst->getInt ("school_id"));
                                setSchoolName (prst->getString ("school_name"));
                                result = true;
                        } else {
                                result = false;
                        }
                }
                delete prst;
                delete pstmt;
        }catch (SQLException e) {
                printf ("SQLException: %s\n", e.what());    
                result = false;
        }

        return result;
}
