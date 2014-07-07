#include <pthread.h>
#include <stdio.h>

#include "sender.h"
#include "receiver.h"

#define NUM_THREADS     5

int main (int argc, char *argv[])
{
   pthread_t threads[NUM_THREADS];
   long t;
   //for(t=0; t<NUM_THREADS; t++){
      printf("In main: creating thread %ld\n", t);
      int sen = pthread_create(&threads[t], NULL, sender::send, (void*) t);
      if (sen){
         printf("ERROR; return code from pthread_create() send is %d\n", sen);
         return 0;
      }

      int rc = pthread_create(&threads[t], NULL, receiver::receive, (void *)t);
      if (rc){
         printf("ERROR; return code from pthread_create() send is %d\n", rc);
	return 0;      
	}

   //}

   /* Last thing that main() should do */
   pthread_exit(NULL);
}
