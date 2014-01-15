/**
 * @file epBase.h
 * @brief 管理基类。
 * @author nvngliu@gmail.com
 * @version 1.0
 * @date 2014-01-14
 */
#ifndef __EPBASE_H__
#define __EPBASE_H__

#include <iostream>
#include <stdio.h> // for printf

/**
 * @brief 管理基类。
 */
class epBase {
public :
        epBase(void) {}
        virtual ~epBase(void) {}

        inline int getId(void) const {
                return id_;
        }

        inline bool setId(const int id) {
                id_ = id;
                return true;
        }

        inline const std::string getName(void) const {
                return name_;
        }

        inline bool setName(const std::string& name) {
                name_ = name;
                return true;
        }

        inline bool setIsUsed(const bool isUsed) {
                isUsed_ = isUsed;
                return true;
        }

        inline bool getIsUsed(void) const {
                return isUsed_;
        }

        virtual inline void dump(void) {
                printf("id      = %d\n", id_);
                printf("name_   = %s\n", name_.c_str());
                printf("isUsed_ = %s\n", isUsed_ ? "true" : "false");
        }

        // virtual const enum ManagerType getType(void) = 0;
protected :
        /**
         * @brief 同类标识ID。
         */
        int id_;
        /**
         * @brief 管理者名称。
         */
        std::string name_;
        /**
         * @brief 是否已经使用。
         */
        bool isUsed_;
};

#endif // __EPBASE_H__
