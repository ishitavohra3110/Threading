#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define sf(n) scanf("%d",&n)
#define pf(n) printf("%d ",n)
#define loop(i,a,b) for(int i=a;i<b;i++)
#define num 100005
int a[num],n;
typedef struct ind{
	int low;
	int high;
}ind;
void merge(int low,int high)
{
	int mid = (low+high)/2;
	int l = low;
	int r = mid+1;
	int b[high-low+1];
	int j=0;
	while(l<=mid && r<=high)
	{
		if(a[l]<a[r])
			b[j++] = a[l++];
		else if(a[l]>a[r])
			b[j++] = a[r++];
		else
			b[j++]=a[l++],b[j++]=a[r++];
	}
	while(l<=mid)
		b[j++]=a[l++];
	while(r<=high)
		b[j++]=a[r++];
	loop(i,0,high-low+1)
		a[low+i]=b[i];
}
void *mergesort(void *index)
{
	ind *node = (ind *)index;
	int mid = (node->low+node->high)/2;
	ind indx[num];
	pthread_t numbers[num];
	indx[0].low = node->low;
	indx[0].high = mid;
	indx[1].low = mid+1;
	indx[1].high = node->high;
	if(node->low >= node->high)
		return NULL;
	loop(i,0,2)
		pthread_create(&numbers[i],NULL,mergesort,&indx[i]);
	loop(i,0,2)
		pthread_join(numbers[i],NULL);
	merge(node->low,node->high);

}
int main()
{
	sf(n);
	loop(i,0,n)
		sf(a[i]);
	pthread_t tid;
	ind node;
	node.low = 0;
	node.high = n-1;
	pthread_create(&tid,NULL,mergesort,&node);
	pthread_join(tid,NULL);
	loop(i,0,n)
		pf(a[i]);
	return 0;

}