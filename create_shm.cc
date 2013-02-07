#include "ushdex.h"

#include <iostream>
using namespace std;

int main ()
{
   shared_memory_object::remove(SHM_NAME);

   managed_shared_memory segment(create_only, SHM_NAME, 65536);
/*
 * If the size is too small, we can get sudden exceptions:
 *
   managed_shared_memory segment(create_only, "SHM_NAME", 1);

    terminate called after throwing an instance of 'boost::interprocess::interprocess_exception'
      what():  boost::interprocess_exception::library_error
*/

   void_allocator allocator(segment.get_segment_manager());

   DoubleDataExchange * double_dex = 
       segment.construct<DoubleDataExchange>("DoubleDataExchange") (key_less(), allocator);
   LongDataExchange * long_dex = 
       segment.construct<LongDataExchange>("LongDataExchange") (key_less(), allocator);

   // Insert some double data
   double_dex->insert(DoubleValueType(KeyType("CL.GLOB.0", "last_traded_price", allocator), 9590.)); 

   // Some long data too
   KeyType my_key("CL.GLOB.0", "last_update", allocator);
   long_dex->insert(LongValueType(my_key, 1360258008084400896)); 

   // Direct access works, once the data has been located via the keys
   long & my_last_update((*long_dex)[my_key]);

   cout << "I haven't forgotten: " << my_last_update << endl;

   my_last_update /= 37;
   cout << "Manipulated: " << long_dex->at(my_key) << endl;

   return 0;
}

