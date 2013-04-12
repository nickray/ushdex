#include "book_rw.h"
using namespace ush;

#include <iostream>
using namespace std;

int main (int argc, char **argv)
{
    BookData data(10);

    data.timestamp = 1360258008084400896;
    data.bids[0] = 9611.;
    data.asks[1] = 9612.;
    data.bidvols[2] = 43;
    data.askvols[3] = 19;

    char buffer[1500];

    long size = data.to_binary("CL.F.GLOB.0", buffer);
    cout << "Serialized to binary of size " << size << endl;

    BookData data2(10);
    string rel_contract;
    data2.from_binary(buffer, rel_contract);
    cout << "Deserialized from binary, this is " << rel_contract << endl;

    cout << "was: " << data << endl;
    cout << "is:  " << data2 << endl;

    return 0;
}
