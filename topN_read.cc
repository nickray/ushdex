#include "topN_rw.h"

#include <cstdlib>
#include <iostream>
#include <string>

int main (int argc, char **argv) {

    if(argc != 3 or std::string("-h") == std::string(argv[1])) {
        std::cout << "usage: " << argv[0] << " <level> <rel_contract>" << std::endl;
        if(argc == 3 and std::string("-h") == std::string(argv[1]))
            return 0;
        return 1;
    }

    long N(std::atol(argv[1]));
    ush::TopData data(N);
    std::string rel_contract(argv[2]);
    ush::TopReader reader(N, rel_contract);

    while(true) {
        reader.read(data);
        std::cout << data << std::endl;
        sleep(1);
    }

    return 0;
}
