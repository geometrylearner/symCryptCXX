#include"symcryCXX.h"
long taskids[NUM_THREADS];
int main(int argc, char* argv[]){
  pthread_t threads[NUM_THREADS]; 
  void* status;
  int rc;
  int i;

  fp = fopen(argv[1], "r+");
  workload_assign(fp);

  for(i=0;i<NUM_THREADS;i++){
    taskids[i] = i;
    rc = pthread_create(&threads[i], NULL, thread_handler,(void*) taskids[i]);
  }

  
  printf("Main: threads must be joined\n");
  for(i=0;i<NUM_THREADS;i++){
    pthread_join(threads[i], &status);
    printf("Main: joined with %d [%lu]: status=%d\n",
	   i, threads[i]);
  }
  fclose(fp); 
  printf("##############################done###############################\n\n");
  pthread_exit(NULL);
  return 0; 
}
