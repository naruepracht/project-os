#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
void *add_item();
void *remove_item();
void *append_buffer();
void *remove_buffer();
int i;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int PRODUCERS, CONSUMERS, BUFFER_SIZE, REQUEST;
int buffer[100000];
int rear = 0, front = 0, request = 0, success = 0;
clock_t timer1, timer2 ,timeoutp,timeoutp2,timeoutc,timeoutc2;
const int timeproducers = 1000;
const int timeconsumers = 1000;
// Specifies that type of variable the function returns.
// main() must return an integer
int main (int argc, char*argv)
{ 

        printf("PRODUCERS:\n")  ;  
        scanf("%d", &PRODUCERS);
        printf("Consumer:\n");
        scanf("%d", &CONSUMERS);
        printf("Buffer:\n");
        scanf("%d", &BUFFER_SIZE);
        printf("Request:\n");
        scanf("%d", &REQUEST);

    timer1 = clock();   //Pull present cpu clock1

    pthread_t thread_producer[PRODUCERS];
    pthread_t thread_consumer[CONSUMERS];

/* Create independent threads each of which will used for producers */
    for( i=0; i<PRODUCERS; i++){
	
        pthread_create(&thread_producer[i], NULL, append_buffer, NULL);
}
/* Create independent threads each of which will used for consumers */
    for(i=0; i<CONSUMERS; i++){
	
        pthread_create(&thread_consumer[i], NULL, remove_buffer, NULL);
}
/* Wait till threads are complete before main continues. Unless we  */
/* wait we run the risk of executing an exit which will terminate   */
/* the process and all threads before the threads have completed.   */
    for(i=0; i<CONSUMERS; i++){
	
        pthread_join(thread_consumer[i], NULL);
}
    for(i=0; i<PRODUCERS; i++){
	
        pthread_join(thread_producer[i], NULL);
}
    timer2 = clock();   //Pull present cpu clock2
    float elapsed = ((float)(timer2 - timer1) / CLOCKS_PER_SEC);

    printf("\n");
    printf("# buf %d %d %d %d\n", PRODUCERS , CONSUMERS , BUFFER_SIZE , REQUEST);
    printf("Producers %d, Consumers %d\n", PRODUCERS, CONSUMERS);
    printf("Buffer size %d\n", BUFFER_SIZE);
    printf("Requests %d\n\n", request);
    printf("Successfully consumed %d requests (%.1f%%)\n", success, (float)success * 100 / request);
    printf("Elapsed Time %.2f s\n", elapsed);
    printf("Throughput %.2f successful requests/s\n", (float)(success) / elapsed);

    exit(EXIT_SUCCESS);
}

void *add_item() {
    buffer[front++] = 1;
    front = front % BUFFER_SIZE;
    printf("Append  Front %d Rear %d Buf %d\n", front , rear , buffer[front]);
}

void *remove_item() {
    buffer[rear++] = 0;
    rear = rear % BUFFER_SIZE;
    printf("Remove  Front %d Rear %d Buf %d\n", front , rear , buffer[rear]);
}

void *append_buffer() {
    //printf("Append thread number %ld\n", pthread_self());
    while(request<REQUEST) {
        if(!pthread_mutex_trylock(&mutex) && request<REQUEST) {
            if(buffer[front] == 0) {
                add_item();
                request++;
               // printf(" + thread %ld append success\n", pthread_self());
            }
            else {
            //printf("Buffer overflow\n");
                timeoutp = clock();
                while (buffer[front] != 0)
                {
                    timeoutp2 = clock();
                    if(buffer[front] == 0) { add_item(); request++; break; }
                    if((timeoutp2 - timeoutp) > timeproducers) { break; }
                    printf("Time Out\n");
            	}
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(NULL);
}

void *remove_buffer() {
    //printf("Remove thread number %ld\n", pthread_self());
    while(success<REQUEST) {
        if(!pthread_mutex_trylock(&mutex) && success<REQUEST) {
            if(buffer[rear] == 1) {
                remove_item();
                success++;
                //printf(" - thread %ld remove success\n", pthread_self());
            }
            else {
            //printf("Buffer underflow\n");
                timeoutc = clock();
                while (buffer[rear] != 1)
                {
                    timeoutc2 = clock();
                    if(buffer[rear] == 1) { remove_item(); success++; break; }
                    if((timeoutc2 - timeoutc) > timeconsumers) { break; }
                    printf("Time Out\n");
                }      	
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(NULL);
}
