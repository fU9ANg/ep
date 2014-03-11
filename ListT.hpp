#ifndef __LISTT_HPP__
#define __LISTT_HPP__

#include <iostream>
#include <list>

template <typename T>
class ListT {
public :
        typedef std::list<T>                          value_type;
        typedef typename std::list<T>::iterator       iterator;
        typedef typename std::list<T>::const_iterator const_iterator;

        template <typename _F>
        inline T
        get_obj_by(_F _pred) {
                iterator it = find_if(list_.begin(), list_.end(), _pred);
                return list_.end()!=it ? *it : NULL;
        }

        template <typename _F>
        inline _F
        for_each(_F _pred) {
                return for_each(list_.begin(), list_.end(), _pred);
        }

        template <typename _F>
        inline T
        erase_by(_F _pred) {
                iterator it = find_if(list_.begin(), list_.end(), _pred);
                if (list_.end() != it) {
                        list_.erase(it);
                        return *it;
                } else {
                        return NULL;
                }
        }

        void insert(T& obj) { list_.push_back(obj); }

        value_type& value(void) { return list_; }

private :
        value_type list_;
};

#endif // __LISTT_HPP__
