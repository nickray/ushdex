#include "book_rw.h"
using namespace ush;

#include <iostream>
using namespace std;

int main (int argc, char **argv)
{
    ShmSession session(connect_only);

    // unsynchronized writing
    SessionKey price_key("CL.F.GLOB.0", "last_traded_price", session);
    SessionKey update_key("CL.F.GLOB.0", "last_update", session);

    session.doubles()[price_key] = 9590.;
    session.longs()[update_key] = 1360258008084400896;

    cout << price_key << ": " << session.doubles()[price_key] << endl;
    cout << update_key << ": " << session.longs()[update_key] << endl;

    // synchronized writing
    BookWriter writer(10, "CL.F.GLOB.0");
    BookData data(10);

    data.timestamp = 1360258008084400896;
    data.bids[0] = 9611.;
    data.asks[0] = 9612.;
    data.bidvols[0] = 43;
    data.askvols[0] = 19;

    writer.write(data);

    cout << "Wrote data for CL.F.GLOB.0:\n" << data << endl;

    if(argc > 1) {
        // throughput test, allow read_test to catch up
        sleep(1);

        BookWriter si_writer(1, "SI.F.GLOB.0");
        for(long i = 1; i != million + 1; ++i) {
            data.timestamp = data.bids[0] = data.asks[0] =
                data.bidvols[0] = data.askvols[0] = i;
            si_writer.write(data, true);
            //nanosleep(&ts, NULL);
            // remember that this system call itself probably
            // takes ~57 microseconds anyway...
        }
    }

    // higher BookData levels
    const long M(million);
    long before, after;
 
    data.timestamp = 0.;
    data.bids[0] = 0.;

    for(long N = 1; N <= 20; ++N) {
        BookWriter writer(N, "FDAX.F.XEUR.0");
        BookReader reader("FDAX.F.XEUR.0");
        BookData dataN(reader.depth);

        before = nano();
        for(long i = 0; i != M; ++i) {
            writer.write(dataN);
            reader.read(dataN);
        }
        after = nano();
        cout << "Throughput for BookData(" << N << "): " << 
            float(after - before)/M << " nanoseconds." << endl;
    }

    return 0;
}
