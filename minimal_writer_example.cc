#include "topN_rw.h"

int main ()
{
    ush::TopData data(5);

    data.timestamp = 1361776225482492;
    data.input_id = 23423;

    data.bids[3] = 9611.;
    data.asks[4] = 9612.;
    data.bidvols[0] = 43;
    data.askvols[2] = 19;

    std::string rel_contract("CL.F.GLOB.0");
    ush::TopWriter writer(5, rel_contract);

    writer.write(data);

    std::cout << "Wrote Top5Data for " << rel_contract << ":\n" << data << std::endl;

    return 0;
}
