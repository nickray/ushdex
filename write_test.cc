#include "ushdex.h"
#include "feedstructs.h"

#include <iostream>
using namespace std;

int main ()
{
    ShmSession session;

    SessionKey price_key("CL.F.GLOB.0", "last_traded_price", session);
    SessionKey update_key("CL.F.GLOB.0", "last_update", session);

    session.doubles()[price_key] = 9590.;
    session.longs()[update_key] = 1360258008084400896;

    return 0;
}

