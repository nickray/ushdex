#include "ushdex.h"

#include <iostream>
using namespace std;

int main (int argc, char **argv)
{
    if (argc != 3) {
        cout << "Usage: lookup <rel_contract> <data_item>" << endl;
        return 1;
    }

    ShmSession session;

    SessionKey key(argv[1], argv[2], session);

    /*
    cout << key << " in session.doubles: " << session.doubles()[key] << endl;
    cout << key << " in session.longs:   " << session.longs()[key] << endl;
    */

    cout << key << " in session.doubles: " << session.doubles().at(key) << endl;
    cout << key << " in session.longs:   " << session.longs().at(key) << endl;

    return 0;
}

