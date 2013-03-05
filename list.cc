#include "session.h"

#include <iostream>

int main ()
{
    ush::ShmSession session(ush::connect_only);

    std::cout << "::DoubleDataExchange\n";
    for (auto it = session.doubles().begin(); it != session.doubles().end(); ++it)
        std::cout << it->first << " => " << it->second << '\n';
    std::cout << "::LongDataExchange\n";
    for (auto it = session.longs().begin(); it != session.longs().end(); ++it)
        std::cout << it->first << " => " << it->second << '\n';

    return 0;
}


