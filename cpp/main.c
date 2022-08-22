#include <stdio.h>
#include <string.h>
int main()
{
	const int a=4;
	*(int *)(&a)=10;
	int c[a];
	memset(c,0,sizeof (int) * 10);
	for(int i=0;i<10;i++) printf("%d\n",c[i]);
	printf("%d\n", a);
}
