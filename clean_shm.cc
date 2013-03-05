#include "session.h"

#include <iostream>
#include <sstream>

int main (int argc, char **argv)
{
    if(argc > 1 and std::string("-h") == std::string(argv[1])) {
        std::cout << "usage: " << argv[0] << " [<size> [<name>]]" << std::endl;
        std::cout << "default size: " << ush::default_shm_size << std::endl;
        std::cout << "standard name: " << ush::standard_shm_name << std::endl;
        return 0;
    }
    std::size_t size(ush::default_shm_size);
    if(argc > 1) {
        std::stringstream stream;
        stream << argv[1];
        stream >> size;
    }
    std::string name(ush::standard_shm_name);
    if(argc > 2) {
        name = argv[2];
    }
    ush::ShmSession session(ush::recreate, size, name);
    std::cout << "created ushdex /dev/shm/" << session.name << " of size " << 
        session.size << ", " << session.free_memory() << " remains" << std::endl;
    return 0;
}

