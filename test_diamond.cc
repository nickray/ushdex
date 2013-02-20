#include "ushdex.h"
#include "feedstructs.h"

#include "trade.h"

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
    TradeWriter tr_writer("CL.F.GLOB.0", session);
    tr_writer.write(wr_data);
    cout << wr_data << endl;

    TradeData rd_data;
    TradeReader tr_reader("CL.F.GLOB.0", session);
    tr_reader.read(&rd_data);
    cout << rd_data << endl;

    assert(rd_data == wr_data);

    return 0;
}

