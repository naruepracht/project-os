#include <stdio.h>
#include <stdlib.h>
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
int tail = 0, head = 0, request = 0, success = 0;
clock_t timer1, timer2;
// �кص���÷��ѧ��蹡���觤׹
// �ѧ��ѹ main �Ѻint 
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

    timer1 = clock();   //�֧ clock1 cpu �Ѩ�غѹ����

    pthread_t thread_producer[PRODUCERS];
    pthread_t thread_consumer[CONSUMERS];

/* ���ҧ loop 㹡�����ҧ thread producer   */
    for( i=0; i<PRODUCERS; i++){
	
        pthread_create(&thread_producer[i], NULL, append_buffer, NULL);
}
/* ���ҧ loop 㹡�����ҧ thread consumer */
    for(i=0; i<CONSUMERS; i++){
	
        pthread_create(&thread_consumer[i], NULL, remove_buffer, NULL);
}
/* �ͨ����thread ����������ó��͹���main �д��Թ��õ��  */
/* �� prodecers ��� consumers �ѹ ����Դ error ���èзӡ����ش    */
/* ��кǹ������ thread ������ ��͹����������ó�   */
    for(i=0; i<CONSUMERS; i++){
	
        pthread_join(thread_consumer[i], NULL);
}
    for(i=0; i<PRODUCERS; i++){
	
        pthread_join(thread_producer[i], NULL);
}
    timer2 = clock();   //�֧ clock2 cpu �Ѩ�غѹ����
    float elapsed = ((float)(timer2 - timer1) / CLOCKS_PER_SEC);

    printf("\n");
    printf("Producers %d, Consumers %d\n", PRODUCERS, CONSUMERS);
    printf("Buffer size %d\n", BUFFER_SIZE);
    printf("Requests %d\n\n", request);
    printf("Successfully consumed %d requests (%.1f%%)\n", success, (float)success * 100 / request);
    printf("Elapsed Time %.2f s\n", elapsed);
    printf("Throughput %.2f successful requests/s\n", (float)(success) / elapsed);

    exit(EXIT_SUCCESS);
}

void *add_item() { //���ҧ�ѧ���� add_item
    buffer[head++] = 1;//buffer[head++]�դ��=1;             
    head = head % BUFFER_SIZE; //head = head ����������� buffer_size
}

void *remove_item() {//���ҧ�ѧ���� remove_item
    buffer[tail++] = 0;//buffer[tail++]�դ��=0
    tail = tail % BUFFER_SIZE;//tail �դ����ҡѺ tail / buffer_size ��������
}

void *append_buffer() {//���ҧ�ѧ���� append_buffer
    printf("Append thread number %ld\n", pthread_self());//print ��� thread ������¡��
    while(request<REQUEST) {//��� request<REQUTEST �����loop while 
        if(!pthread_mutex_trylock(&mutex) && request<REQUEST) {//�Է���㹡����䢺ѿ���� ������ç������͹䢨���ͤ
            if(buffer[head] == 0) {
                add_item();
                request++;
                printf(" + thread %ld append success\n", pthread_self());
            }
            else {//�������� �ʴ� buffer overflow 
            printf("Buffer overflow\n");
            }
            pthread_mutex_unlock(&mutex);// �Ŵ��ͤmutex
        }
    }
    pthread_exit(NULL);
}

void *remove_buffer() {
    printf("Remove thread number %ld\n", pthread_self());
    while(success<REQUEST) {//㹢�з�� success �դ�ҹ��¡��� request
        if(!pthread_mutex_trylock(&mutex) && success<REQUEST) {// ��� pthread ����������ͤ mutex ��� success < Request ������ٻ if
            if(buffer[tail] == 1) {
                remove_item();
                success++;
                printf(" - thread %ld remove success\n", pthread_self());
            }
            else {
            printf("Buffer underflow\n");
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(NULL);
}
