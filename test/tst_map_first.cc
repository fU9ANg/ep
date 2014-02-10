#include <map>
#include <iostream>

int
main(void) {
        std::map<int, std::string> mis;

        mis.insert(std::make_pair<int, std::string>(1, "abc"));

        std::map<int, std::string>::iterator it = mis.begin();
        (it->first) = 3;

        return 0;
}
