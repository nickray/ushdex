#include "book_rw.h"

#include <iostream>

int main () {
    std::string rel_contract("CL.F.GLOB.0");
    ush::BookWriter writer(5, 1, rel_contract);

    ush::BookData data(writer);

    data.timestamp = 1361776225482492;
    data.input_id = 23423;

    data.bids[3] = 9611.;
    data.asks[4] = 9612.;
    data.bidvols[0] = 43;
    data.askvols[2] = 19;
    data.implied_bids[0] = 9610.;

    writer.write(data);

    std::cout << "Wrote BookData for " << rel_contract << ":\n" << data << std::endl;

    return 0;
}
