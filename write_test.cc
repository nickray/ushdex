#include "ushdex.h"
#include "feedstructs.h"

#include <iostream>
#include <unistd.h>
#include <time.h>
using namespace std;

int main ()
{
    ShmSession session;

    // unsynchronized writing
    SessionKey price_key("CL.F.GLOB.0", "last_traded_price", session);
    SessionKey update_key("CL.F.GLOB.0", "last_update", session);

    session.doubles()[price_key] = 9590.;
    session.longs()[update_key] = 1360258008084400896;

    cout << price_key << ": " << session.doubles()[price_key] << endl;
    cout << update_key << ": " << session.longs()[update_key] << endl;

    // synchronized writing
    Top1Data data;

    data.timestamp = 1360258008084400896;
    data.bid1 = 9611.;
    data.ask1 = 9612.;
    data.bid1vol = 43;
    data.ask1vol = 19;

    Top1Writer writer("CL.F.GLOB.0", session);
    writer.write(data);

    cout << "Wrote data for CL.F.GLOB.0:\n" << data << endl;

    // throughput test, allow read_test to catch up
    sleep(1);

    Top1Writer si_writer("SI.F.GLOB.0", session);
    timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 1;
    for(long i = 1; i != N + 1; ++i) {
        data.timestamp = data.bid1 = data.ask1 =
            data.bid1vol = data.ask1vol = i;
        si_writer.write(data);
        /*
        if(i & 1)
           usleep(1);
           */
        nanosleep(&ts, NULL);
        // remember that this system call itself probably
        // takes ~1 microsecond anyway...
    }

    return 0;
}

