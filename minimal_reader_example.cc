#include "topN_rw.h"

int main ()
{
    ush::TopData<5> data;

    std::string rel_contract("CL.F.GLOB.0");
    ush::TopReader<5> reader(rel_contract);

    reader.read(data);

    std::cout << "Read Top5Data for " << rel_contract << ":\n" << data << std::endl;

    return 0;
}
