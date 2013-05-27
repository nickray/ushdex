#include "multi_r.h"
#include "book_rw.h"

#include <iostream>
#include <string>

int main (int argc, char **argv) {

    if(argc < 2 or std::string("-h") == std::string(argv[1])) {
        std::cout << "usage: " << argv[0] << " <rel_contract> [<rel_contract> [<rel_contract> ...]]" << std::endl;
        if(argc == 2 and std::string("-h") == std::string(argv[1]))
            return 0;
        return 1;
    }

    /*
    std::vector<ush::BookReader> readers;
    for(int i = 1; i != argc; ++i) {
        std::string rel_contract(argv[i]);
        readers.emplace_back(rel_contract);
    }

    ush::MultiReader<ush::BookReader, ush::BookData> multireader(readers);
    */

    std::vector<std::string> rel_contracts;
    for(int i = 1; i != argc; ++i) {
        rel_contracts.emplace_back(argv[i]);
    }

    ush::MultiReader<ush::BookReader, ush::BookData> multireader(rel_contracts);

    while(true) {
        std::cout << multireader.read_next() << ',' << multireader << std::endl;
    }

    return 0;
}
