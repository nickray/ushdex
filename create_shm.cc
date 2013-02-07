#include "ushdex.h"

#include <iostream>
using namespace std;

int main ()
{
   shared_memory_object::remove(SHM_NAME);

   managed_shared_memory segment(create_only, SHM_NAME, 65536);
/*
   managed_shared_memory segment(create_only, "SHM_NAME", 1);

    terminate called after throwing an instance of 'boost::interprocess::interprocess_exception'
      what():  boost::interprocess_exception::library_error
*/

   void_allocator allocator(segment.get_segment_manager());

   DoubleDataExchange * double_dex = 
       segment.construct<DoubleDataExchange>("DoubleDataExchange") (key_less(), allocator);
   LongDataExchange * long_dex = 
       segment.construct<LongDataExchange>("LongDataExchange") (key_less(), allocator);

   double_dex->insert(DoubleValueType(KeyType("CL.GLOB.0", "last_traded_price", allocator), 9590.)); 

   long_dex->insert(LongValueType(KeyType("CL.GLOB.0", "last_update", allocator), 1360258008084400896)); 

   return 0;
}

