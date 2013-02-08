#include "ushdex.h"
#include "feedstructs.h"

#include <iostream>
using namespace std;

int main ()
{
    ShmSession session;

    // unsynchronized reading
    SessionKey price_key("CL.F.GLOB.0", "last_traded_price", session);
    SessionKey update_key("CL.F.GLOB.0", "last_update", session);

    cout << price_key << ": " << session.doubles()[price_key] << endl;
    cout << update_key << ": " << session.longs()[update_key] << endl;
    
    // synchronized reading
    Top1Data data;
    Top1Reader reader("CL.F.GLOB.0", session);

    reader.read(data);
    cout << "Read data for CL.F.GLOB.0:\n" << data << endl;

    return 0;
}

