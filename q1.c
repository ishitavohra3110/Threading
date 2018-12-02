#include <stdio.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define loop(i,a,b) for(int i=a;i<b;i++)
#define sf(n) scanf("%d",&n)
#define pf(n) printf("%d ",n)
#define num  10005
int arrival,start[num],catgeory[num],pos[num],gp[num][4],vis[num],cnt_player[num],cnt_refree[num],warm,equip;
// -1 for player
// 1 for refree
//int organ;
int values[num];
int players,refrees,left,gp_no,n;
pthread_mutex_t play,ref,org,group,court;//when they'll be ready to play
void warmup(int play)
{
	//timer++;
	//pthread_mutex_lock(&warm);

	printf("Player%d doing warmup\n",pos[play]);
	sleep(1);
	warm = 0;
	//pthread_mutex_unlock(&warm);

}
void adjustEquipment(int ref)
{

	printf("Refree%d is adjusting the Equipment\n",pos[ref]);
	sleep(0.5);
	equip = 0;
}
void enterCourt(int j)
{
	//pthread_mutex_lock(&court);
	printf("Players and Refree enter the court\n");
	int ind1,ind2,ind3,take;
	ind1 = gp[j][0];
	ind2 = gp[j][1];
	ind3 = gp[j][2];
	pf(ind1);
	pf(ind2);
	pf(ind3);
	printf("\n");
	if(catgeory[ind1]==1)
	{
		take = ind1;
		equip = 1;
		while(equip)
			adjustEquipment(take);
	}
	else{
		warm = 1;
		while(warm)
			warmup(ind1);
	}
	if(catgeory[ind2]==1)
	{
		take = ind2;
		equip = 1;
		while(equip)
			adjustEquipment(take);
	}
	else
	{
		warm = 1;
		while(warm)
			warmup(ind2);
	}
	if(catgeory[ind3]==1)
	{
		take = ind3;
		equip = 1;
		while(equip)
			adjustEquipment(take);
	}
	else
	{
		warm = 1;
		while(warm)
			warmup(ind3);
	}
	
	printf("The game%d has started\n",gp_no);
	//pthread_mutex_unlock(&court);

}
void form_gp()
{
	pthread_mutex_lock(&group);
	loop(j,gp_no,n+4)
	{
		loop(i,0,players+refrees)
		{
			if(!vis[i])
			{
				if(catgeory[i]==-1 && cnt_player[j]<2)
					vis[i]=1,cnt_player[j]++,gp[j][values[j]++]=i;
				else if(catgeory[i]==1 && cnt_refree[j]<1)
					vis[i]=1,cnt_refree[j]++,gp[j][values[j]++]=i;
				if(cnt_player[j]==2 && cnt_refree[j]==1)
				{
					gp_no++;
					enterCourt(j);
					break;
				}
			}
		}
	}
	pthread_mutex_unlock(&group);
}
void meetOrganizer(int i)
{
	pthread_mutex_lock(&org);
	if(catgeory[i]==-1)
		printf("Player%d has meet the Organizer\n",pos[i]);
	else if(catgeory[i]==1)
		printf("Refree%d has meet the Organizer\n",pos[i]);
	form_gp();
	//organ = 0;
	pthread_mutex_unlock(&org);
}
void *Player(void* i)//entered academy
{
	pthread_mutex_lock(&play);
	int a = *((int*) i);
	players++;
	catgeory[a]=-1;
	pos[a]=players;
	printf("Player%d has entered the Academy\n",players);
	meetOrganizer(a);
	pthread_mutex_unlock(&play);
}
void *Refree(void* i)//entered academy
{
	pthread_mutex_lock(&ref);
	int a = *((int*) i);
	refrees++;
	catgeory[a]=1;
	pos[a]=refrees;
	printf("Refree%d has entered the Academy\n",refrees);
	//organ =1;
	//while(organ);
	meetOrganizer(a);
	pthread_mutex_unlock(&ref);
}
int main()
{
	int val;
	sf(n);
	left = 3*n;
	double prob,prob1;
	int total;
	loop(i,0,3*n)
		start[i]=random()%3;
	pthread_mutex_init(&play,NULL);
	pthread_mutex_init(&ref,NULL);
	pthread_mutex_init(&org,NULL);
	pthread_mutex_init(&group,NULL);
	//pthread_mutex_init(&court,NULL);
	loop(i,0,3*n)
	{
		total = 3*n-players-refrees;
		prob = (float)(2*n-players)/(float)(total);
		pthread_t tid;
    	int *arg = malloc(sizeof(*arg));
    	*arg = i;
		if(prob<0.5)
			pthread_create(&tid,NULL,Refree,arg);
		else
			pthread_create(&tid,NULL,Player,arg);
		sleep(start[i]);
		sleep(5);
	}
	//while(players!=2*n || refrees!=n);
	printf("\n");
	//loop(i,0,3*n)
	//	pf(pos[i]);
}

