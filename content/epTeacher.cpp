#include "epTeacher.h"
#include <stdio.h> // for printf
#include "database.h"
#include "SQL.h"
#include "../netdef.h" // for EPTEACHER_INVALID_TEACHER_ID

epTeacher::epTeacher(void) { }

epTeacher::~epTeacher(void) { }

const enum LoginType
epTeacher::getType(void) const {
        return LT_TEACHER;
}

bool
epTeacher::init(const std::string& account
              , const std::string& passwd
              , const int fd
              , const enum user_status us) {
        epUser::init(account, passwd, fd, us);
        bool result = false;
        std::string strpwd;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_TEA_INFO_BY_ACCOUNT);
                pstmt->setString (1, account);
                ResultSet* prst = pstmt->executeQuery ();
                while (prst->next ()) {
                        strpwd = prst->getString ("password");
                        if (0 == strncmp(passwd.c_str(), strpwd.c_str(), strpwd.size())
                                        && passwd.size() > 0
                                        && strpwd.size() == passwd.size()) {

                                epPerson::init(prst->getString("first_name")
                                             , prst->getString("last_name")
                                             , prst->getString("sex")
                                             , prst->getInt   ("age")
                                             , prst->getString("race_name")
                                             , prst->getString("birthday")
                                             , prst->getString("native_name")
                                             , std::string() // , prst->getString("address")
                                              );
                                /*
                                   lastName_  = prst->getString("last_name");
                                   firstName_ = prst->getString("first_name");
                                   sex_       = prst->getString("sex");
                                   age_       = prst->getInt   ("age");
                                   race_      = prst->getString("race_name");
                                // birthday_  = prst->getString("birthday");
                                native_    = prst->getString("native_name");
                                */

                                id_        = prst->getInt   ("teacher_id");
                                teacherNum_= prst->getString("teacher_id");
                                schoolId_  = prst->getInt   ("school_id");
                                schoolName_= prst->getString("school_name");
                                result = true;
                        } else {
                                result = false;
                        }
                }
                delete prst;
                delete pstmt;
        } catch (SQLException e) {
                printf ("[DEBUG] %s : SQLException: %s\n", __func__, e.what());    
                result = false;
        }

        return result;
}

void
epTeacher::dump(void) {
        epUser::dump();
        epPerson::dump();
        printf("teacher number  = %s\n", teacherNum_.c_str());
        printf("school name     = %s\n", schoolName_.c_str());
        printf("school id       = %d\n", schoolId_);
}

