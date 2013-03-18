#include "session.h"

#include <iostream>

int main ()
{
    ush::ShmSession session(ush::connect_only);

    std::cout << "::DoubleDataExchange\n";
    for (auto it = session.doubles().cbegin(); it != session.doubles().cend(); ++it)
        std::cout << it->first << " => " << it->second << '\n';
    std::cout << "::LongDataExchange\n";
    for (auto it = session.longs().cbegin(); it != session.longs().cend(); ++it)
        std::cout << it->first << " => " << it->second << '\n';

    return 0;
}
