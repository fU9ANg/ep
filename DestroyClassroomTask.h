#ifndef __DESTROYCLASSROOMTASK_H__
#define __DESTROYCLASSROOMTASK_H__

#include "task.h"
#include <map>

typedef std::map<int, time_t> DES_MAP;

class DestroyClassroomTask : public task {
public :
        virtual int work(void);
        static DES_MAP desMap_;
};

#endif // __DESTROYCLASSROOMTASK_H__
