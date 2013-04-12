#include "book_rw.h"
using namespace ush;

#include <iostream>
using namespace std;

int main ()
{
    ShmSession session(connect_only);

    // unsynchronized reading
    SessionKey price_key("CL.F.GLOB.0", "last_traded_price", session);
    SessionKey update_key("CL.F.GLOB.0", "last_update", session);

    cout << price_key << ": " << session.doubles()[price_key] << endl;
    cout << update_key << ": " << session.longs()[update_key] << endl;
    
    // synchronized reading
    BookReader reader("CL.F.GLOB.0");
    BookData data(reader.depth);

    reader.read(data);
    cout << "Read data for CL.F.GLOB.0:\n" << data << endl;

    // what happens if keys doesn't exist?
    BookReader ES_reader("ES.F.GLOB.0");
    ES_reader.read(data);
    cout << "Read data for ES.F.GLOB.0:\n" << data << endl;

    cout << "best_bid:" << data.bids[0] << endl;
    cout << "best_ask:" << data.asks[0] << endl;
    data.bids[0] = data.asks[0] = 3.;
    cout << "best_bid:" << data.bids[0] << endl;
    cout << "best_ask:" << data.asks[0] << endl;

    // throughput test
    BookReader si_reader("SI.F.GLOB.0");
    int N(million);
    long num_read(0);
    long num_correct(0);
    long last(0);
    do {
        si_reader.read_next(data, true);
        ++num_read;
        if(
            (data.timestamp == data.bids[0]) and
            (data.timestamp == data.asks[0]) and
            (data.timestamp == data.bidvols[0]) and
            (data.timestamp == data.askvols[0])
          ) 
            ++num_correct;
        //cout << data << endl;
        if (data.output_id  != last + 1)
            cout << ":: " << data << endl;
        last = data.output_id;
    } while(data.timestamp < N);

    cout << "Missed " << (N - num_read) << " out of " << N << " ticks, and parsed " << 
        (num_read - num_correct) << " out of " << num_read << " incorrectly." << endl;

    return 0;
}
