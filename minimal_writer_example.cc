#include "book_rw.h"

#include <iostream>

int main () {
    std::string rel_contract("CL.F.GLOB.0");
    ush::BookWriter writer(5, rel_contract);

    ush::BookData data(5);

    data.timestamp = 1361776225482492;
    data.exchange_id = 23423;

    data.bids[3] = 9611.;
    data.asks[4] = 9612.;
    data.bidvols[0] = 43;
    data.askvols[2] = 19;

    writer.write(data);

    std::cout << "Wrote BookData for " << rel_contract << ":\n" << data << std::endl;

    return 0;
}
