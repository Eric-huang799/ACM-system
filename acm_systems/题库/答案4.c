#include <stdio.h>
#include <ctype.h>
#include <string.h>
char d[26] = {'V','W','X','Y','Z','A','B','C','D','E','F','G',
'H','I','J','K','L','M','N','O','P','Q','R','S','T','U'};
int main(void)
{
    char a[256], b[256], c[256];
	while (fgets(a, sizeof(a), stdin),
	fgets(b, sizeof(b), stdin),
	fgets(c, sizeof(c), stdin),
	strcmp(a, "ENDOFINPUT\n") != 0)
{
	int len = strlen(b);
	int i;
	for (i = 0; i < len; ++i) {
		if (isalpha((unsigned char)b[i])) {
	int t = b[i] - 'A';
	putchar(d[t]);
	} 
		else {
	putchar(b[i]);
		}
	}
}
return 0;
}
h>
int main(){
int a,b,c,d;
scanf("%d %d %d %d",&a,&b,&c,&d);
int re=a*b+c*d;
printf("%d",re);
return 0;
}


