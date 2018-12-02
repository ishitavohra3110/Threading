#include<stdio.h>
int b[100005];
void mergesort(int a[],int low,int mid,int high)
{
	//printf("%d %d %d @",low,mid,high);
	int i,j,l;
	l=0;
	i=low;
	j=mid+1;
	int k=0;
	while(i<=mid && j<=high)
	{
		if(a[i]<a[j])
		{
			b[l]=a[i];
			l++;
			i++;
		}
		else
		{
			b[l]=a[j];
			l++;
			j++;
		}
	}
	while(i<=mid)
		b[l++]=a[i++];
	while(j<=high)
		b[l++]=a[j++];
	for(k=0;k<l;k++)
		a[low+k]=b[k];
	return;
}

		

void merge(int a[],int low,int high)
{
	int mid;
	if(low<high)
	{
		mid=(low+high)/2;
		merge(a,low,mid);
		merge(a,mid+1,high);
		mergesort(a,low,mid,high);
	}
}

int main()
{
	int n,a[100005];
	scanf("%d",&n);
	int i;
	for(i=0;i<n;i++)
		scanf("%d",&a[i]);
	merge(a,0,n-1);
	for(i=0;i<n;i++)
		printf("%d ",a[i]);
	return 0;
}