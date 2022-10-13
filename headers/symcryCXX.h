#include"stdio.h"
#include"pthread.h"
#define NUM_THREADS 1


int strLength(char* str){
  int len {0};
  for(int i=0;str[i];i++){
    len++;
  }
  return len;
}


// BLKSIZE, fp, gp, buf are all global;
#define BLKSIZE 4096L

FILE *fp, *gp;
char buf[4096];

class symCryptor{
public:
  char key[256]{"r2_9!i3k7i6m"};
  int len_key {strLength(key)};
  void symCryption(char* buffer, int len_buffer){
    int round = len_buffer/len_key + (len_buffer%len_key)/len_buffer+1; 
    for(int n=0;n<round;n++){
      for(int i=0;key[i];i++){
	buffer[len_key*n+i] ^= key[i];
      }
    }
  }
};



long workload[NUM_THREADS][2];
FILE* tfp[NUM_THREADS]; // tfp store the start file pointer for each thread.

//FILE* thread_fp[NUM_THREADS]; // recieve the fp for each thread created.

void* thread_handler(void* args){
  pthread_t tid = pthread_self();
  long i = (long) args;
  printf("Thread %d=[%lu] is working\n",i, tid);
  tfp[i] = fp; // initialize the file pointer for this thread;
  fseek(tfp[i], workload[i][0] , SEEK_SET);
  long tsz = ftell(tfp[i]); // the file pointer for this thread.
  printf("Thread %d starts at %ld ends at %ld\n", i, workload[i][0], workload[i][1]);

  // encrypt the file sector by sector;
  int n;
  symCryptor sC;

  
  long t_workload_count = 0;

  while(ftell(tfp[i])< workload[i][1] -4096)
    {
      n=fread(buf,1,BLKSIZE,tfp[i]);
      sC.symCryption(buf, 4096);
      fseek(tfp[i], -4096L, SEEK_CUR);
      fwrite(buf, 1, n, tfp[i]);
      
      fseek(tfp[i], 19L, SEEK_CUR);
      t_workload_count += n;
    }
  tsz = ftell(tfp[i]);
  printf("!!!!!!!!!!!!workload[%d][0]=%lu, ftell(fp) = %ld, t_workload_count = %lu\n", i, workload[i][0], ftell(fp), t_workload_count);
  pthread_exit((void*)0);
}


void workload_assign(FILE* fp){
  int i;
  long sz;
  // get the size of file in bytes
  fseek(fp, 0L, SEEK_END);// set file ptr to the end of file
  sz = ftell(fp);
  fseek(fp, 0L, SEEK_SET); // reset the file pointer fp at its beginning. 
  printf("The Entire Size of source file = %ld bytes\n", sz); // printf the size of the file.
  

  // programming the threads' workload
  long average_workload = sz/NUM_THREADS;
  long rest_workload = sz%NUM_THREADS;
  workload[0][0] = 0L;
  workload[0][1] = average_workload;
  for (i=1;i<NUM_THREADS;i++){
    workload[i][0] = workload[i-1][1];
    workload[i][1] = workload[i][0] + average_workload;
  }
  workload[NUM_THREADS][1] += rest_workload;
  // assigned

  // checking the workload meets the total bytes of target file;
  long workload_count = 0;
  for (i=0;i<NUM_THREADS;i++){
    workload_count += (workload[i][1]-workload[i][0]); // counting axiom.
  }
  printf("#################workloads=%lu assigned!###################\n", workload_count);
  if(workload_count == sz)
    printf("#################workload matched exactly!###################\n");
  // checked
}
