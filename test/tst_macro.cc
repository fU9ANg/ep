#include <stdio.h>

#define ABC(obj, i) \
        printf(#obj); \
        obj->set_##obj();

#define CHECK_USER_FD(obj, from) \
        ep##obj* obj = EPMANAGER->get##obj##From##from##ByFd(p->getfd()); \
        if (NULL == obj) { \
                printf("[DEBUG] %s : NULL == "#obj, __func__); \
                SINGLE->bufpool.free(p); \
                return; \
        }

#define DEBUG_MSG_TYPE(p) \
        #ifdef __DEBUG_MSG_TYPE__ \
        printf("[DEBUG] %s : %s(%d) fd = [%d]\n", __FILE__, __func__, __LINE__, p->getfd()); \
        #endif

int
main(void) {
//         ABC(dksfjkd, 7)

        CHECK_USER_FD(User, User)
        CHECK_USER_FD(Teacher, Classroom)

        return 0;
}
