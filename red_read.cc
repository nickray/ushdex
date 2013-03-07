#include "red_rw.h"

#include <iostream>
#include <string>

int main (int argc, char **argv) {

    if(argc != 2 or std::string("-h") == std::string(argv[1])) {
        std::cout << "usage: " << argv[0] << " <rel_contract>" << std::endl;
        if(argc == 2 and std::string("-h") == std::string(argv[1]))
            return 0;
        return 1;
    }

    ush::RedData data;
    std::string rel_contract(argv[1]);
    ush::RedReader reader(rel_contract);

    while(true) {
        reader.read(data);
        std::cout << data << std::endl;
        sleep(1);
    }

    return 0;
}
