#include <stdio.h>
#include <map>

class Tst {
public :
        Tst(void) { printf("[INFO] Tst::Tst()\n"); }
        ~Tst(void) { printf("[INFO] Tst::~Tst()\n"); }
};

int
main(void) {
        std::map<int, Tst*> mt;

        Tst* t;
        for (int i=0; i<3; ++i) {
                t = new Tst();
                mt.insert(std::make_pair<int, Tst*>(i, t));
        }

        std::map<int, Tst*>::iterator it = mt.find(1);
        if (mt.end() != it) {
                mt.erase(it);
        }

        // for (;;) { }

        return 0;
}
