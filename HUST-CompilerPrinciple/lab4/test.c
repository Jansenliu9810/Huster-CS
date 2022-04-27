int a,b,c;
float m,n;
char c1,c2;
char a[10];
int fibo(int a)
{
	int i;
	if(a == 1 || a == 2){
	  return 1;
        }
	for(i=1;i<15;i++) {
          
	}
	return fibo(a-1)+fibo(a-2);
}
int main()
{
	int m,n,i;
	char c;
	float ar[20];
	m=read();
	i=1;
	i++;
	--i;
	m+=i+15;
	while(i <= m){
		n=fibo(i);
		write(n);
		i=i+1;
	}
	return 1;
}

