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

   DataExchange<long> * long_dex = 
       segment.construct<DataExchange<long>>("LongDataExchange") (key_less(), allocator);

   long_dex->insert(LongValueType(KeyType("CL.GLOB.0", "last_traded_price", allocator), 9590L)); 

   return 0;
}

