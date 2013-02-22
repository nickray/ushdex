#include "session.h"
#include "topN_rw.h"
using namespace ush;

#include <iostream>
#include <unistd.h>
#include <time.h>
using namespace std;

typedef TopData<20> Top20Data;
typedef std::pair<const Key, Top20Data> Top20ValueType;
typedef boost::interprocess::allocator<Top20ValueType, segment_manager_t> Top20ValueTypeAllocator;
typedef boost::interprocess::map<Key, Top20Data, key_less, Top20ValueTypeAllocator> Top20DataExchange;

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
    TopData<1> data;

    data.timestamp = 1360258008084400896;
    data.bids[0] = 9611.;
    data.asks[0] = 9612.;
    data.bidvols[0] = 43;
    data.askvols[0] = 19;

    TopWriter<1> writer("CL.F.GLOB.0", session);
    writer.write(data);

    cout << "Wrote data for CL.F.GLOB.0:\n" << data << endl;

    /*
    // throughput test, allow read_test to catch up
    sleep(1);

    TopWriter<1> si_writer("SI.F.GLOB.0", session);
    timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 1;
    for(long i = 1; i != million + 1; ++i) {
        data.timestamp = data.bids[0] = data.asks[0] =
            data.bidvols[0] = data.askvols[0] = i;
        si_writer.write(data);
        nanosleep(&ts, NULL);
        // remember that this system call itself probably
        // takes ~57 microseconds anyway...
    }
    */

    // higher TopData levels
    const long M(million);
    long before, after;
 
    // 0
    TopData<1> data1;
    TopWriter<1> writer1("FDAX.F.XEUR.0", session);
    data1.timestamp = 0.;
    data1.bids[0] = 0.;

    TopReader<1> reader1("FDAX.F.XEUR.0", session);

    before = nano();
    for(long i = 0; i != M; ++i) {
        writer1.write(data1);
        reader1.read(data1);
    }
    after = nano();
    cout << "Throughput for TopData<1>: " << float(after - before)/M << " nanoseconds." << endl;

    // 5
    TopData<5> data5;
    TopWriter<5> writer5("FDAX.F.XEUR.0", session);
    data5.timestamp = 0.;
    data5.bids[0] = 0.;

    TopReader<5> reader5("FDAX.F.XEUR.0", session);

    before = nano();
    for(long i = 0; i != M; ++i) {
        writer5.write(data5);
        reader5.read(data5);
    }
    after = nano();
    cout << "Throughput for TopData<5>: " << float(after - before)/M << " nanoseconds." << endl;

    // 20
    TopData<20> data20;
    TopWriter<20> writer20("FDAX.F.XEUR.0", session);
    data20.timestamp = 0.;
    data20.bids[0] = 0.;

    TopReader<20> reader20("FDAX.F.XEUR.0", session);

    before = nano();
    for(long i = 0; i != M; ++i) {
        writer20.write(data20);
        reader20.read(data20);
    }
    after = nano();
    cout << "Throughput for TopData<20>: " << float(after - before)/M << " nanoseconds." << endl;

    /*
    // variant with direct memcopying
    Top20DataExchange * t20ex;
    try {
        session.segment->construct<Top20DataExchange>("Top20DataExchange") (key_less(), *(session.allocator));
        cout << "created" << endl;
    } catch(boost::interprocess::interprocess_exception) {
        session.segment->find<Top20DataExchange>("Top20DataExchange").first;
        cout << "found" << endl;
    }
    //session.segment->construct<Top20DataExchange>("Top20DataExchange") (key_less(), *(session.allocator));

    SessionKey key("CL.F.GLOB.0", "Top20Data", session);
    cout << "a" << endl;
    TopData<20> my_data((*t20ex)[key]);
    cout << "b" << endl;

    before = nano();
    TopData<20> my_data_out;
    TopData<20> my_data_in;
    long actual_ctr;
    volatile long * ctr(&actual_ctr);
    long prior_ctr, posterior_ctr;

    for(long i = 0; i != M; ++i) {
        store<long>(ctr, *ctr + 1);
        my_data = my_data_out;
        store<long>(ctr, *ctr + 1);
        do {
            prior_ctr = load<long>(ctr);
            my_data_in = my_data;
            posterior_ctr = load<long>(ctr);
        } while ((posterior_ctr != prior_ctr) || (posterior_ctr & 1));
    }
    after = nano();
    cout << "Throughput for TopDataExchange directly: " << float(after - before)/M << " nanoseconds." << endl;
    */

    return 0;
}

