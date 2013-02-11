#include "ushdex.h"
#include "feedstructs.h"

#include <iostream>
using namespace std;

int main ()
{
    ShmSession session;

    // unsynchronized reading
    SessionKey price_key("CL.F.GLOB.0", "last_traded_price", session);
    SessionKey update_key("CL.F.GLOB.0", "last_update", session);

    cout << price_key << ": " << session.doubles()[price_key] << endl;
    cout << update_key << ": " << session.longs()[update_key] << endl;
    
    // synchronized reading
    Top1Data data;
    Top1Reader reader("CL.F.GLOB.0", session);

    reader.read(data);
    cout << "Read data for CL.F.GLOB.0:\n" << data << endl;

    // what happens if keys doesn't exist?
    Top1Reader ES_reader("ES.F.GLOB.0", session);
    ES_reader.read(data);
    cout << "Read data for ES.F.GLOB.0:\n" << data << endl;

    Top1Reader si_reader("SI.F.GLOB.0", session);
    long last(0);
    long num_read(0);
    long num_correct(0);
    do {
        si_reader.read(data);
        if (data.timestamp != last) {
            last = data.timestamp;
            ++num_read;
            if(
                (data.timestamp == data.bid1) and
                (data.timestamp == data.ask1) and
                (data.timestamp == data.bid1vol) and
                (data.timestamp == data.ask1vol)
              ) {
                ++num_correct;

            }
            cout << data << endl;
        }
    } while(last < N);

    cout << num_correct << ',' << num_read << ',' << N << endl;

    return 0;
}

