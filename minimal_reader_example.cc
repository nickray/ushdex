#include "topN_rw.h"

int main ()
{
    ush::TopData data(5);

    std::string rel_contract("CL.F.GLOB.0");
    ush::TopReader reader(5, rel_contract);

    reader.read(data);

    std::cout << "Read Top5Data for " << rel_contract << ":\n" << data << std::endl;

    return 0;
}
