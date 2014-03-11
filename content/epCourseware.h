#ifndef __EPCOURSEWARE_H__
#define __EPCOURSEWARE_H__

#include "../fun_obj.hpp"

class epCourseware {
public :
        typedef define_type<int, struct epCourseware_id_tag> id_type;
        typedef courseware_status status_type;

        id_type     id_;
        status_type status_;

        epCourseware(id_type& id
                   , status_type& status)
                : id_(id)
                  , status_(status) {}

        bool operator==(id_type& id) { return id_.value() == id.value(); }
};

#endif // __EPCOURSEWARE_H__
