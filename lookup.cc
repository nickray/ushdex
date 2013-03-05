#include "session.h"

#include <iostream>

int main (int argc, char **argv)
{
    if (argc != 3) {
        std::cout << "Usage: lookup <rel_contract> <data_item>" << std::endl;
        return 1;
    }

    ush::ShmSession session(ush::connect_only);

    ush::SessionKey key(argv[1], argv[2], session);

    if (session.doubles().count(key))
        std::cout << key << " in session.doubles: " << session.doubles().at(key) << std::endl;
    if (session.longs().count(key))
        std::cout << key << " in session.longs: " << session.longs().at(key) << std::endl;

    return 0;
}

