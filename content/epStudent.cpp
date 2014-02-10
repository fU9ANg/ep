#include "epStudent.h"
#include "database.h"
#include "SQL.h"
#include <stdio.h> // for printf

epStudent::epStudent(void) {
}

epStudent::~epStudent(void) {
}

bool
epStudent::init(const std::string& account, const std::string& passwd) {
        epUser::init(account, passwd);
        bool result = false;
        std::string strpwd;
        try {
                MutexLockGuard guard(DATABASE->m_mutex);
                PreparedStatement* pstmt = DATABASE->preStatement (SQL_GET_STU_INFO_BY_ACCOUT);
                pstmt->setString (1, account);
                ResultSet* prst = pstmt->executeQuery ();
                while (prst->next ()) {
                        strpwd = prst->getString ("password");
                        if (0 == strncmp(passwd.c_str(), strpwd.c_str(), strpwd.size())
                                        && passwd.size() > 0
                                        && strpwd.size() == passwd.size()) {

                                lastName_  = prst->getString("last_name");
                                firstName_ = prst->getString("first_name");
                                sex_       = prst->getString("sex");
                                age_       = prst->getInt   ("age");
                                race_      = prst->getString("race_name");
                                // birthday_  = prst->getString("birthday");
                                native_    = prst->getString("native_name");

                                id_        = prst->getInt   ("student_id");
                                studentNum_= prst->getString("number");
                                classId_   = prst->getInt   ("class_id");
                                className_ = prst->getString("class_name");
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

const enum LoginType
epStudent::getType(void) const {
        return LT_STUDENT;
}

void
epStudent::dump(void) {
        epUser::dump();
        epPerson::dump();
        printf("student number = %s\n", studentNum_.c_str());
        printf("class id       = %d\n", classId_);
        printf("class name     = %s\n", className_.c_str());
}
