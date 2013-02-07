#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <functional>
#include <utility>

int main ()
{
   using namespace boost::interprocess;

   //Remove shared memory on construction and destruction
   struct shm_remove
   {
      shm_remove() { shared_memory_object::remove("MySharedMemory"); }
      //~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
   } remover;

   managed_shared_memory segment
      (create_only
      ,"MySharedMemory" //segment name
      ,65536);          //segment size in bytes

   typedef int    KeyType;
   typedef float  MappedType;
   typedef std::pair<const int, float> ValueType;

   typedef allocator<ValueType, managed_shared_memory::segment_manager>
      ShmemAllocator;

   typedef map<KeyType, MappedType, std::less<KeyType>, ShmemAllocator> MyMap;

   ShmemAllocator alloc_inst (segment.get_segment_manager());

   MyMap *mymap =
      segment.construct<MyMap>("MyMap")      //object name
                                 (std::less<int>() //first  ctor parameter
                                 ,alloc_inst);     //second ctor parameter

   //Insert data in the map
   for(int i = 0; i < 100; ++i){
      mymap->insert(std::pair<const int, float>(i, (float)i));
   }
   return 0;
}

