#include "session.h"

#include <iostream>

int main ()
{
    ush::ShmSession session(true);
    std::cout << "created /dev/shm/" << ush::SHM_NAME << " of size " << ush::SHM_SIZE << std::endl;
    return 0;
}

