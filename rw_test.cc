#include "trade_rw.h"
#include "red_rw.h"
using namespace ush;

#include <iostream>
#include <cassert>
using namespace std;

int main ()
{
    TradeData wr_data;
    wr_data.timestamp = micro(); 
    wr_data.price = 9697.;
    wr_data.volume = 3;
    wr_data.aggressor = +1;
    wr_data.type = 0;
    wr_data.cum_volume = 673; 

    TradeWriter tr_writer("CL.F.GLOB.0");
    tr_writer.write(wr_data);
    cout << wr_data << endl;

    TradeData rd_data;
    TradeReader tr_reader("CL.F.GLOB.0");
    tr_reader.read(rd_data);

    cout << rd_data << endl;
    assert(rd_data == wr_data);

    unsigned long iterations = 1e7;
    unsigned long a, b;
    b = nano();
    for(unsigned long i = 0; i != iterations; ++i)
    {   
        tr_writer.write(wr_data);
        tr_reader.read(rd_data);
    }   
    a = nano();
    cout << iterations << " iterations took " << double(a - b)/1e9 << "sec, so " << double(a - b)/iterations << "nsec per iteration" << endl;

    RedData data_out, data_in;
    data_out.timestamp = micro(); 
    data_out.bid = 9061.3;
    data_out.ask = 9061.6;
    RedWriter w("XX.X.XXXX.0");
    RedReader r("XX.X.XXXX.0");
    w.write(data_out);
    r.read(data_in);

    cout << data_out << endl << data_in << endl;
    assert(data_out == data_in);
    
    return 0;
}
