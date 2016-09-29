#include <bits/stdc++.h>

using namespace std;

int n,t;
char s[1<<18];

int main()
{
	cin>>n>>t>>(s+1); s[0]=48;
	for(int i=0,f=0;s[i];i++)
		if(s[i]=='.') f=1;
		else if(s[i]>52 && f==1)
		{
			int p=i-1,k=0;
			for(;s[p+1]>52 && k<t;k++,p--)
			{
				if(s[p]=='.')
				{
					for(int i=p-1,c=1;i>=0;i--)
					{
						s[i]+=c;
						if(s[i]>57) s[i]-=10,c=1;
						else c=0;
					}
					s[p]=0; break;
				}
				else
					s[p]++;
			}
			s[p+2]=0;
			puts(s[0]==48?s+1:s);
			return 0;
		}
	puts(s+1);
	
}