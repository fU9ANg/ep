#include <stdio.h>
#include <vector>
#include "../Buf.h"
#include "../Single.h"
#include "../protocol.h"
#include "../message/proto/protocol.pb.h"

int
main(void) {
        std::vector<sGetGradeList> vs;
        sGetGradeList ggl;

        ggl.set_grade_id(1);
        ggl.set_grade_name("abc");
        vs.push_back(ggl);

        ggl.set_grade_id(1);
        ggl.set_grade_name("abc");
        vs.push_back(ggl);

        ggl.set_grade_id(1);
        ggl.set_grade_name("abc");
        vs.push_back(ggl);

        Buf* pBuf = NULL;
        char* p = (char*)pBuf->ptr() + MSG_HEAD_LEN + sizeof(int);
        int size = 256;
        for (int i=0; i<3; ++i) {
                pBuf = SINGLE->bufpool.malloc();
                vs[i].SerializeToArray(p+i*size, size);
        }

        for (int i=0; i<3; ++i) {
                ggl.ParseFromArray(p+i*size, size); 
                printf("grade_id   = %d\n", ggl.grade_id());
                printf("grade_name = %s\n", ggl.grade_name().c_str());
        }

        return 0;
}
