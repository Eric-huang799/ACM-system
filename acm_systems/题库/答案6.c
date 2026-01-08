#include<stdio.h>
long long memo[1000];
int feb(int a){
	if(a>2){ memo[a-1]=memo[a-2]+memo[a-3]; return memo[a-1];
	}
		else if(a==1){ memo[0]=0; return memo[0];
		}
		else if(a==2){ memo[1]=1; return memo[1];
		}
	}
	int main(){
		int N; scanf("%d",&N);
		int j;
		for(int i=1;i<=N;i++){
			j=feb(i);
			printf("%d ",j);
		}return 0;
	}

