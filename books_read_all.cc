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

    std::vector<ush::BookReader> readers;
    for(int i = 1; i != argc; ++i) {
        std::string rel_contract(argv[i]);
        readers.push_back(ush::BookReader(rel_contract));
    }

    ush::MultiReader<ush::BookReader, ush::BookData> multireader(readers);

    while(true) {
        std::cout << multireader.read_next() << ',' << multireader << std::endl;
    }

    return 0;
}
