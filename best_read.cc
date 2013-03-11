#include "adaptors.h"

#include <iostream>
#include <string>

int main (int argc, char **argv) {

    if(argc != 2 or std::string("-h") == std::string(argv[1])) {
        std::cout << "usage: " << argv[0] << " <rel_contract>" << std::endl;
        if(argc == 2 and std::string("-h") == std::string(argv[1]))
            return 0;
        return 1;
    }

    std::string rel_contract(argv[1]);
    ush::BookReader reader(rel_contract);
    ush::BookData data(reader);
    ush::BestAdaptor best(data);

    while(true) {
        reader.read(data);
        best.apply();
        std::cout << best << std::endl;
        sleep(1);
    }

    return 0;
}
