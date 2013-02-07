#include "shm.h"

int main ()
{
   shared_memory_object::remove("MySharedMemory");
   managed_shared_memory segment
      (create_only
      ,"MySharedMemory" //segment name
      ,65536);          //segment size in bytes
   return 0;
}

