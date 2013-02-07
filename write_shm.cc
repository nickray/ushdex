#include "shm.h"

int main ()
{
   shared_memory_object::remove("MySharedMemory");
   managed_shared_memory segment
      (open_only
      ,"MySharedMemory" //segment name
      ,65536);          //segment size in bytes

   ShmemAllocator alloc_inst (segment.get_segment_manager());

   MyMap *mymap =
        segment.construct<MyMap>("MyMap")      //object name
                                   (std::less<int>() //first  ctor parameter
                                   ,alloc_inst);     //second ctor parameter
   return 0;
}

