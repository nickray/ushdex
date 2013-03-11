#include "book_rw.h"

#include <iostream>

int main (int argc, char **argv) {

    if(argc != 2 or std::string("-h") == std::string(argv[1])) {
        std::cout << "usage: " << argv[0] << " <rel_contract>" << std::endl;
        if(argc == 2 and std::string("-h") == std::string(argv[1]))
            return 0;
        return 1;
    }

    std::string rel_contract(argv[1]);
    ush::BookReader reader(rel_contract);
    std::cout << "BookData for " << rel_contract << " has depth " << reader.depth() << 
        " and implied depth " << reader.implied_depth() << std::endl;

    return 0;
}
