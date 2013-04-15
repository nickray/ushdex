#include "book_rw.h"
#include "trade_rw.h"
#include "red_rw.h"
using namespace ush;

#include <iostream>
#include <sstream>
using namespace std;

int main (int argc, char **argv)
{
    BookData data(10);

    data.timestamp = 1360258008084400896;
    data.bids[0] = 9611.;
    data.asks[1] = 9612.;
    data.bidvols[2] = 43;
    data.askvols[3] = 19;
    data.askvols[9] = 19;

    char buffer[1500];

    long size = data.serialize("CL.F.GLOB.0", buffer);
    cout << "Serialized to binary of size " << size << endl;

    BookData data2(10);
    string rel_contract;
    data2.deserialize(buffer, rel_contract);
    cout << "Deserialized from binary, this is " << rel_contract << endl;

    cout << "was: " << data << endl;
    cout << "is:  " << data2 << endl;

    const long M(million);
    long before, after;
    before = nano(); 
    for(long i = 0; i != M; ++i)
        size = data.serialize("CL.F.GLOB.0", buffer);
    after = nano();
    cout << "BookData(10).serialize takes " << float(after - before)/M << " nanoseconds" << endl;
    before = nano(); 
    for(long i = 0; i != M; ++i)
        size = data2.deserialize(buffer, rel_contract);
    after = nano();
    cout << "BookData(10).deserialize takes " << float(after - before)/M << " nanoseconds" << endl;

    bool error(false);
    stringstream stream;
    for(unsigned i = 0; i != 1000; ++i) {
        stringstream instream, outstream;
        data.serialize("CL.F.GLOB.0", buffer);
        data2.deserialize(buffer, rel_contract);
        instream << data;
        outstream << data2;
        error |= (instream.str() != outstream.str());
    }
    if(error)
        cout << "Error in BookData serialization process!" << endl;
    else
        cout << "No error in BookData serialization process!" << endl;

    TradeData td1, td2;
    td1.price = 1.234;
    td1.aggressor = -1;
    td1.cum_volume = 567; 
    error = false;
    for(unsigned i = 0; i != M; ++i) {
        td1.serialize(rel_contract, buffer);
        td2.deserialize(buffer, rel_contract);
        error |= (memcmp(&td1, &td2, sizeof(TradeData)) != 0);
    }
    if(error)
        cout << "Error in TradeData serialization process!" << endl;
    else
        cout << "No error in TradeData serialization process!" << endl;

    RedData rd1, rd2;
    rd1.bid = 1.234;
    rd1.ask = 1.223;
    error = false;
    for(unsigned i = 0; i != M; ++i) {
        rd1.serialize(rel_contract, buffer);
        rd2.deserialize(buffer, rel_contract);
        error |= (memcmp(&rd1, &rd2, sizeof(RedData)) != 0);
    }
    if(error)
        cout << "Error in RedData serialization process!" << endl;
    else
        cout << "No error in RedData serialization process!" << endl;


    return 0;
}
