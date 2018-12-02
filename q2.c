#include <stdio.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define loop(i,a,b) for(int i=a;i<b;i++)
#define sf(n) scanf("%d",&n);
#define pf(n) printf("%d ",n);
#define initial_mutex(a) pthread_mutex_init(a,NULL);
#define create(a,b,c) pthread_create(&a,NULL,(void *) b,&c);
#define num 100005
typedef struct booth{
	int Booth_Id;
	int Evm_Cnt;
	int Evm_Slot;
	int Voter_Cnt;
	pthread_mutex_t Lock_Evm;
	pthread_mutex_t Lock_Struct;
	pthread_mutex_t Lock_Cast;
	int waiting;
	int cast;
	int ready;
	int temp;
	int allot;
	int Wait_Evm;
}booth;
booth b[15];
pthread_t Voter1[num];
pthread_t Evm1[num];
int count_evm,count_voter,wait1;
void polling_ready_evm(booth *b,int slot)
{
	printf("Evm no %d at booth no. %d ready with slots %d\n",b->temp,b->Booth_Id,slot);
	b->ready=1;
	for(;;)
	{
		while(b->Wait_Evm==1);
		b->Wait_Evm=1;
		while(b->Wait_Evm==1);
		if(b->waiting==slot || ((b->cast)+(b->waiting) == (b->Voter_Cnt)))
				break;
	}
	b->ready = 0;
	if((b->cast+b->waiting) != (b->Voter_Cnt))
	{
		b->waiting-=slot;
		b->allot = slot;
	}
	else
	{
		b->allot = b->waiting;
		b->waiting = 0;
	}
	while(b->allot>0);
}
void voter_wait_for_evm(booth *b)
{
	int slot = 0;
	for(;;)
	{
		if(b->ready!=1)
			slot++;
		else
			break;
	}
}
void voter_in_slot(booth *b)
{
	pthread_mutex_lock(&b->Lock_Struct);
	while(!(b->Wait_Evm));
	(b->waiting)++;
	b->Wait_Evm = 0;
	pthread_mutex_unlock(&b->Lock_Struct);
}
void evm_cast_vote(booth *b)
{
	(b->cast)++;
	(b->allot)--;
}
void vote(booth *b)
{
	voter_wait_for_evm(b);
	voter_in_slot(b);
	printf("Voter is waiting at booth no %d\n",b->Booth_Id);
	pthread_mutex_lock(&b->Lock_Cast);
	while(!(b->allot));
	evm_cast_vote(b);
	pthread_mutex_unlock(&b->Lock_Cast);
	count_voter--;
	int left;
	left = (b->Voter_Cnt)-(b->cast);
	printf("Voter casted his vote at booth %d and number of voters left is %d\n",b->Booth_Id,left);
}
void evm(booth *b)
{
	wait1 = 1;
	for(;;)
	{
		pthread_mutex_lock(&b->Lock_Evm);
		if(b->cast==b->Voter_Cnt)
		{
			pthread_mutex_unlock(&b->Lock_Evm);
			break;
		}
		polling_ready_evm(b,b->Evm_Slot);
		pthread_mutex_unlock(&b->Lock_Evm);
	}
	count_evm--;
}
void for_evm(int i,int j)
{
	b[i].temp = j;
	create(Evm1[count_evm],evm,b[i]);
	count_evm++;
	while(!wait1);
	wait1=0;
}
void for_voter(int i,int j)
{
	create(Voter1[count_voter],vote,b[i]);
	count_voter++;
}
int main()
{
	int n;
	printf("Number of booths needed ");
	sf(n);
	loop(i,0,n)
	{
		printf("Number of voters alloted to the booth%d ",i);
		sf(b[i].Voter_Cnt);
		printf("Number of Evm present the booth%d ",i);
		sf(b[i].Evm_Cnt);
		b[i].Evm_Slot = random()%10+1;
		b[i].Booth_Id = i;
		b[i].waiting = 0,b[i].cast = 0,b[i].ready=0,b[i].allot=0,b[i].Wait_Evm=0;
		initial_mutex(&b[i].Lock_Evm)
		initial_mutex(&b[i].Lock_Cast)
		initial_mutex(&b[i].Lock_Struct)
	}
	loop(i,0,n)
	{
		loop(j,0,b[i].Evm_Cnt)
			for_evm(i,j);
		loop(j,0,b[i].Voter_Cnt)
			for_voter(i,j);
	}
	while(count_evm || count_voter);
	return 0;
}