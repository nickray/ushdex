#include "ushdex.h"

#include <iostream>
using namespace std;

int main ()
{
    ShmSession session;

    cout << "::DoubleDataExchange\n";
    for (auto it = session.doubles().begin(); it != session.doubles().end(); ++it)
        cout << it->first << " => " << it->second << '\n';
    cout << "::LongDataExchange\n";
    for (auto it = session.longs().begin(); it != session.longs().end(); ++it)
        cout << it->first << " => " << it->second << '\n';

    return 0;
}

