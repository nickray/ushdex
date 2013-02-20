#include "ushdex.h"
#include "rw_trade.h"

#include "nano.h"

#include <iostream>
#include <unistd.h>
#include <time.h>
#include <cassert>
using namespace std;

int main ()
{
    ShmSession session;

    TradeData wr_data;
    wr_data.last_traded_price = 9697.;
    wr_data.last_traded_volume = 3;
    wr_data.cum_traded_volume = 673; 

    TradeWriter tr_writer("CL.F.GLOB.0", session);
    tr_writer.write(wr_data);
    cout << wr_data << endl;

    TradeData rd_data;
    TradeReader tr_reader("CL.F.GLOB.0", session);
    tr_reader.read(&rd_data);
    cout << rd_data << endl;

    assert(rd_data == wr_data);

    unsigned long iterations = 1e6;
    unsigned long a, b;
    a = nano();
    for(unsigned long i = 0; i != iterations; ++i)
    {   
        tr_writer.write(wr_data);
        tr_reader.read(&rd_data);
    }   
    b = nano();

    cout << iterations << " iterations took " << double(b - a)/1e9 << "sec, so " << double(b - a)/iterations << "nsec per iteration" << endl;

    return 0;
}

