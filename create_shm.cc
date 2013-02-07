#include "ushdex.h"

#include <iostream>
using namespace std;

int main ()
{
   shared_memory_object::remove("MySharedMemory");

   managed_shared_memory segment(create_only, "MySharedMemory", 65536);
/*
   managed_shared_memory segment(create_only, "MySharedMemory", 1);

    terminate called after throwing an instance of 'boost::interprocess::interprocess_exception'
      what():  boost::interprocess_exception::library_error
*/

   LongShmAllocator long_allocator(segment.get_segment_manager());
   DoubleShmAllocator double_allocator(segment.get_segment_manager());

   LongDataExchange * long_dex = 
       segment.construct<LongDataExchange>("LongDataExchange") (std::less<KeyType>(), long_allocator);

   DoubleDataExchange * double_dex = 
       segment.construct<DoubleDataExchange>("DoubleDataExchange") (std::less<KeyType>(), long_allocator);

   cout << "Before allocation, " << segment.get_free_memory() << " free memory remains.\n";

   // Set to zero
   for(KeyType i = 0; i != 100; ++i) {
       long_dex->insert(std::pair<const KeyType, long>(i, 0));
       double_dex->insert(std::pair<const KeyType, double>(i, 0.));
   }

   cout << "After allocation, " << segment.get_free_memory() << " free memory remains.\n";

   return 0;
}

