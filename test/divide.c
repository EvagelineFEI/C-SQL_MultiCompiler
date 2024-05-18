#include <stdio.h>
int prime(int n){
    int i;
    int m=0;
    for(i=2;i<n;i++){
        if(n%i==0)m++;
    }
    if(m==0)return 1;
    else return 0;
}
int main()
{
    int n;
    scanf("%d",&n);
    if(n>6&&(n%2==0)){
        int j;
        for(j=2;j<=(n/2);j++){
            if(prime(j)&&prime(n-j))printf("%d %d\n",j,n-j);
        }
    }
    else printf("ERROR");

    return 0;
}