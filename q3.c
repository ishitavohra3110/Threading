#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#define loop(i,a,b) for(int i=a;i<b;i++)
#define sf(n) scanf("%d",&n);
#define pf(n) printf("%d ",n);	
const int num = 100005;
int b[100005];
int* shm_array;
void insertion_sort(int arr[],int n)
{
	int temp,j;
	loop(i,1,n)
	{
		temp = arr[i];
		j = i-1;
		while(j>=0 && arr[j]>temp)
		{
			arr[j+1] = arr[j];
			j--;
		}
		arr[j+1]=temp;
	}
	//printf("insertion sort");
	//loop(i,0,n)
	//	pf(arr[i]);
//	printf("\n");
	return;
}
void merge(int a[],int low,int mid,int high)
{
	int count=high-low+1;
	int i=low,k=mid+1,j=0;
	while(i<=mid && k<=high)
	{
		if(a[i]<a[k])
			b[j++]=a[i++];
		else if(a[k]<a[i])
			b[j++]=a[k++];
		else if(a[i]==a[k])
			b[j++]=a[i++],b[j++]=a[k++];
	}
	while(i<=mid)
		b[j++]=a[i++];
	while(k<=high)
		b[j++]=a[k++];
	//loop(i,0,j)
	//	pf(b[i]);
//	printf("\n");
	loop(i,0,count)
		a[low++]=b[i];
}
void mergesort(int a[],int l,int r)
{
	int i,sz;
	sz = r-l+1;
	if(sz<=5)
	{
		insertion_sort(a+l,sz);
		return;
	}
	int lpid,rpid;
	lpid = fork();
	if(lpid<0)
	{
		printf("Left Child process is not created\n");
		exit(0);
	}
	else if(!lpid)
	{
		mergesort(a,l,l+sz/2-1);
		exit(0);
	}
	else
	{
		rpid = fork();
		if(rpid<0)
		{
			printf("Right Child is not created\n");
			exit(0);
		}
		else if(!rpid)
		{
			mergesort(a,l+sz/2,r);
			exit(0);
		}
	}
	int status;
	waitpid(lpid,&status,0);
	waitpid(rpid,&status,0);
	merge(a,l,l+sz/2-1,r);

}
int main()
{
	int n,shmid;
	key_t key = IPC_PRIVATE;
	sf(n);
	size_t SHM_SIZE = sizeof(int)*n;
	shmid = shmget(key,SHM_SIZE,IPC_CREAT | 0666);
	//if key has the value IPC_PRIVATE, a shared memory segment with size equal to SHM_SIZE is created.
	if(shmid<0)
	{
		perror("shmget");
		return 0;
	}
	//shmat accepts the shmid and attaches the indicated shared memory to the program's address space.
	shm_array = shmat(shmid,0,0);
	int val;
	val = (intptr_t)shm_array;
	if(val == -1)
	{
		perror("shmat");
		return 0;
	}
	loop(i,0,n)
	{
		sf(shm_array[i]);
		//pf(i);
	}
	mergesort(shm_array,0,n-1);
	loop(i,0,n)
		pf(shm_array[i]);
	return 0;



}