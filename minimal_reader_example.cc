#include "book_rw.h"

#include <iostream>

int main () {
    std::string rel_contract("CL.F.GLOB.0");

    ush::BookReader reader(rel_contract);
    ush::BookData data(reader.depth);
    reader.read(data);
    std::cout << "BookData for " << rel_contract << " has depth " <<
        reader.depth << ":\n" << data << std::endl;

    ush::BookReader restricted_reader(1, rel_contract);
    ush::BookData restricted_data(1);
    restricted_reader.read(restricted_data);

    std::cout << "But we can also read restricted data: " << restricted_data << std::endl;

    std::cout << "returning" << std::endl;
    return 0;
}
