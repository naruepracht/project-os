#include <stdio.h>
#include <conio.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#define N 5;
#define NUM_THREADS 1
pthread_t tid[2]; 
pthread_mutex_t lock; 
int count=0;
int front=0;int rear=0;
char buffer[5];
int prodsleep=0;int consleep=0;
int b;
void *perform_work(void *arguments){
  scanf("%a",&b);
  int index = *((int *)arguments);
  printf("THREAD %d: Started.\n", index);

}
void add_items(void)
{
char item;
if (count<5)//���count <5 ���loop
{
printf("\n Enter Input Circular Buffer:");
scanf(" %c",&item);//�Ѻ��� char ������ item
buffer [front]=item;//buffer [front=0] = item
front = (front+1)%5;//front �դ��= (front+1)���5 ��������
count++;// count ��������1
if(consleep==1 && count==1)//���consleep=1 ��� count=1
{
printf("\n Consumer Wakeup ");
}
}
else
{
printf("\n Buffer is Overflow");
prodsleep=1;
}
}
void remove_items(void)
{
char item;
if (count>0)
{
item = buffer[rear];//item = buffer[rear=0]
buffer[rear]=' ';
printf("\n Remove_items: %c",item);
rear=(rear+1)%5;
count--;//count Ŵ����1 
if(prodsleep==1 && count==4)//���prodsleep=1 ��� count=4
{
printf("\n No buffer overflow");//�ʴ������ No buffer overflow
}
}
else
{
printf("\n No buffer underflow");
consleep=1;
}
}
void view(void)//�ѧ���� view 
{
int i;
printf("\n Buffer Data : ");
for(i=0;i<5;i++)//for ������� i=0 ;���͹� i<5 ;i������鹷���1
{
printf("| %c ",buffer[i]);
}
}
void Exit()//�ѧ����exit 
{

	printf("THREAD 0: Ended.\n");
}
main()
{
int i,choice,flag=0;
pthread_t threads[NUM_THREADS];
int thread_args[NUM_THREADS];
int g;
int result_code;
if (pthread_mutex_init(&lock, NULL) != 0) //��� pthread mutex �纤������lock �����ҡѺ0 
{ 
	printf("\n mutex init has failed\n"); 
	return 1; 
}
	for (g = 0; g < NUM_THREADS; g++) {
    printf("\nIN MAIN: Creating thread %d.\n", g);

    thread_args[g] = g;
    result_code = pthread_create(&threads[g], NULL, perform_work, &thread_args[g]);// �ӡ�����ҧ thread ��������threads[g] ���᷹ attribute �դ�� =Null 
    assert(!result_code);//�礤��result_code 
  }
    printf("IN MAIN: All threads are created.\n");
 
printf("\n 1:Add_Items ");
printf("\n 2:Remove_Items ");
printf("\n 3:View buffer data ");
printf("\n 4:Exit ");

	pthread_join(tid[0], NULL); 
	pthread_join(tid[1], NULL); 
	pthread_mutex_destroy(&lock);
	
do
{
printf("\n\n Enter your choice :");
scanf("%d",&choice);//�Ѻ��� int ������choice 
switch(choice)// ���͹� switch �ͧchoice 
{
case 1:add_items();//����� ��� 1 ŧ� ����ҿѧ���� add_item
	//wait for each thread to complete

break;
case 2:remove_items();
break;
case 3:view();
break;
case 4: Exit();
printf("..............Exiting................");
break;
default:printf("\n Invalid Choice!");//��������͹����� �ʴ������ Invalid Choice!
break;
}
}while(choice!=4);
}
