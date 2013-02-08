#include "ushdex.h"

#include <iostream>
using namespace std;

int main ()
{
   // recreate shared memory segment and get allocator for it
   shared_memory_object::remove(SHM_NAME);
   managed_shared_memory segment(create_only, SHM_NAME, 65536);
   void_allocator allocator(segment.get_segment_manager());

   // construct the two hash tables
   DoubleDataExchange & double_dex
       (*segment.construct<DoubleDataExchange>("DoubleDataExchange") (key_less(), allocator));
   LongDataExchange & long_dex
       (*segment.construct<LongDataExchange>("LongDataExchange") (key_less(), allocator));

   /*
    * the above part belongs to create_shm
    *
    * the below part should be split between write_shm and read_shm
    *
    */

   // our keys for today
   Key price_key("CL.F.GLOB.0", "last_traded_price", allocator);
   Key update_key("CL.F.GLOB.0", "last_update", allocator);
   
   // insert some data
   double_dex[price_key] = 9590.;
   long_dex[update_key] = 1360258008084400896;

   // direct access works once the data has been located via the keys
   long & my_last_update(long_dex[update_key]);
   cout << "I haven't forgotten: " << my_last_update << endl;
   my_last_update /= 37;
   cout << "Manipulated: " << long_dex[update_key] << endl;

   return 0;
}

