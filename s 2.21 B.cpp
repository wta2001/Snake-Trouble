#include<stdio.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include<windows.h>
#include<math.h>
#define XX 41
#define YY 20
#define LL 100
#define NN 1000
#define MM 250
#define VV 2
int DD=150;

struct snake{
	int x[LL+5],y[LL+5],u[6],f,l,t;
	double m;
};

struct bomb{
	int x,y,f,v,t;
};

int s[XX+5][YY+5]={0};
struct snake a,b;
struct bomb c[NN+5],cc[NN+5];
int t[XX+5][YY+5];
int n,m,h,aa=0,bb=0,tt;
int dx[4]={0,1,0,-1},dy[4]={1,0,-1,0};
double ppp[4][200][100][60];
	DWORD t1,t2;
#define pp(f,x,y,l) ppp[f][x+100][y+50][l+10]


void gotoxy(int x, int y)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

void pri();
int begi();
void reset();///////////////////////
void readsave();
void writesave();

double p(int f,int x,int y,int l)
{
	int lf,rf;
	if(pp(f,x,y,l)>=-1)
		return(pp(f,x,y,l));
	if(x==0&&y==-1&&l>=-1)
		return(pp(f,x,y,l)=0);
	if(x==0&&y==0&&l>=0)
		return(pp(f,x,y,l)=0);
	if(l<0||y<0)
		return(pp(f,x,y,l)=1);
	if(x>(y+1)/2||-x>(y+1)/2)
		return(pp(f,x,y,l)=1);
	lf=(f+1)%4;
	rf=(f+3)%4;
	pp(f,x,y,l)=.6*p(f,x+dx[f],y+dy[f]-2,l-2)+
				.2*p(lf,x+dx[lf],y+dy[lf]-2,l-2)+
				.2*p(rf,x+dx[rf],y+dy[rf]-2,l-2);
	return(pp(f,x,y,l));
}

void calp()
{
	int f,x,y,l;
	for(f=0;f<4;f++)
		for(x=-2*XX-15;x<=2*XX+15;x++)
			for(y=-XX-5;y<=XX+5;y++)
				for(l=-5;l<=XX+5;l++)
					pp(f,x,y,l)=-2;
	for(f=0;f<4;f++)
		for(x=-2*XX-13;x<=2*XX+13;x++)
			for(y=-XX-3;y<XX+3;y++)
				for(l=-3;l<XX+3;l++)
					p(f,x,y,l);
	for(l=-3;l<XX+3;l++)
		pp(0,0,-1,l)=pp(1,0,-1,l)=pp(3,0,-1,l)=1;
}

double fp(int f,int x,int y)
{
	int i,ddx,ddy,l,dex,dey,cx,cy;
	double ans=1;
	for(i=0;i<n;i++)
		if(c[i].v&&c[i].t>-1)
		{
			cx=c[i].x+dx[c[i].f];
			cy=c[i].y+dy[c[i].f];
			dex=x-cx;
			dey=y-cy;
			switch(c[i].f)
			{
				case 0:
					ddx=dex;
					ddy=dey;
					l=YY-1-cy;
					break;
				case 1:
					ddx=-dey;
					ddy=dex;
					l=XX-1-cx;
					break;
				case 2:
					ddx=-dex;
					ddy=-dey;
					l=cy;
					break;
				case 3:
					ddx=dey;
					ddy=-dex;
					l=cx;
					break;
			}
			ans*=p((f-c[i].f+4)%4,ddx,ddy,l);
		}
	return(ans);
}

int va(int f,int x,int y)
{
	int tt=s[(x+dx[f]+XX)%XX][(y+dy[f]+YY)%YY];
	return(tt!=1&&tt!=2&&tt!=6);
}

int ff(int f,int x,int y,double m,int ff,int xx,int yy)
{
	double fp1,fp2,fp3,fq0,fq1,fq2,fq3,fq,max,ans;
	int lf,rf,lff,rff,opt,tp=0;
	lf=(f+1)%4;
	rf=(f+3)%4;
	lff=(ff+1)%4;
	rff=(ff+3)%4;
	
	c[n].f=ff;
	c[n].x=xx+dx[ff];
	c[n].y=yy+dy[ff];
	c[n].v=1;
	c[n].t=tt;
	n++;
	if(va(f,x,y))fp1=fp(f,(x+dx[f]+XX)%XX,(y+dy[f]+YY)%YY); else{fp1=0; tp=1;}
	if(va(lf,x,y))fp2=fp(lf,(x+dx[lf]+XX)%XX,(y+dy[lf])+YY%YY); else fp2=0;
	if(va(rf,x,y))fp3=fp(rf,(x+dx[rf]+XX)%XX,(y+dy[rf]+YY)%YY); else fp3=0;
	n--;
	
	
	fq1=fp(ff,(xx+dx[ff]+XX)%XX,(yy+dy[ff]+YY)%YY);
	fq2=fp(lff,(xx+dx[lff]+XX)%XX,(yy+dy[lff]+YY)%YY);
	fq3=fp(rff,(xx+dx[rff]+XX)%XX,(yy+dy[rff]+YY)%YY);
	fq0=.6*fq1+.2*(fq2+fq3);
	c[n].f=f;
	c[n].x=x+dx[f];
	c[n].y=y+dy[f];
	c[n].v=1;
	c[n].t=tt;
	n++;
	fq1=fp(ff,(xx+dx[ff]+XX)%XX,(yy+dy[ff]+YY)%YY);
	fq2=fp(lff,(xx+dx[lff]+XX)%XX,(yy+dy[lff]+YY)%YY);
	fq3=fp(rff,(xx+dx[rff]+XX)%XX,(yy+dy[rff]+YY)%YY);
	fq=.6*fq1+.2*(fq2+fq3);
	n--;
	if(abs(fp2-fp3)<.01&&fp2>.3&&fp3>.3)
		if(opt=rand()%2)
		{
			opt=1;
			max=fp2;
		}
	  	  else
		{
			opt=3;
			max=fp3;
		}
	  else
	if(fp2>=fp3)
	{
		opt=1;
		ans=max=fp2;
	}
	  else
	{
		opt=3;
		ans=max=fp3;
	}

	if(fp1>=max)
		if((fq0-fq)*sqrt(m)>.4&&m>=1)
			opt=4,ans=fp2;
		  else
		{
			if(fp1-max>.01||rand()%7)
				opt=2,ans=fp2;
		}
	  else
	if((fq0-fq)>.9*(max-fp1)&&(fq0-fq)*sqrt(m)>.4&&m>=1)
		opt=4,ans=fp2;
	if(h==1)gotoxy(0,YY+8);else gotoxy(0,YY+12);
	if(tp&&max<0.1 && x+dx[f]>=0&&x+dx[f]<XX&&y+dy[f]>=0&&y+dy[f]<YY)
		if(m>=2||(m>=1&&(va((f+1)%4,x+dx[f],y+dy[f])||va((f+3)%4,x+dx[f],y+dy[f]))))
			opt=4;
	if(opt==2&&((h==1?a.l:b.l)>=5)&&m>=5&&(rand()%2))
		opt=5;
	printf("%.3f\t%.3f\t%.3f\t\n%.3f\t%.3f\t\t%.3f\t\n%d\n",fp1,fp2,fp3,fq0,fq,max,opt);
	return(opt);
}

int mine(int x,int y,int f);

int main(){
	int op1[6]={0,1,0,0,0,0};
	int op2[6]={0,0,0,1,0,0};
	int op3[6]={0,0,0,0,1,0};
	int op4[6]={0,0,0,0,0,1};
	int mode,opt,i,j,k,rx,ry,dx[4]={0,1,0,-1},dy[4]={1,0,-1,0};
	srand(time(NULL));
	calp();
	
	mode=begi();
	A:if(mode)reset();else readsave();
	tt=0;
	t1=GetTickCount();
	while(1){
		h=1;
		t2=GetTickCount();
		if(mode<=2)
		{
			a.u[1]=GetAsyncKeyState('A');
			a.u[2]=GetAsyncKeyState('D');
			a.u[3]=GetAsyncKeyState('W');
			a.u[4]=GetAsyncKeyState('S');
		}
		else
		{
			opt=ff(a.f,a.x[0],a.y[0],a.m,b.f,b.x[0],b.y[0]);
			a.u[1]=op1[opt];
			a.u[2]=op2[opt];
			a.u[3]=op3[opt];
			a.u[4]=op4[opt];
		}
		if(a.u[1])
			a.f=(a.f+1)%4;
		if(a.u[2])
			a.f=(a.f+3)%4;
		if(a.u[4]&&a.m>=1&&a.l>=2)
		{
			m++;
			a.m-=1;
			t[a.x[a.l]][a.y[a.l]]++;
			s[a.x[a.l]][a.y[a.l]]=0;
			s[a.x[a.l-1]][a.y[a.l-1]]=0;
			a.l-=2;
		}
		if(a.u[3]&&a.m>=1)
		{
			a.t=1;
			a.m-=1;
			c[n].v=1;
			c[n].t=tt;
			c[n].f=a.f;
			c[n].x=a.x[0];
			c[n].y=a.y[0];
			for(i=1;i<=VV;i++)
				if(c[n].v)
				{
					c[n].x+=dx[c[n].f];
					c[n].y+=dy[c[n].f];
					if(s[c[n].x][c[n].y]==1)
					{
						c[n].v=0;
						s[c[n].x][c[n].y]=0;
						for(j=0;j<=a.l;j++)
							if(c[n].x==a.x[j]&&c[n].y==a.y[j])
								break;
						for(k=j+1;k<=a.l;k++)
							s[a.x[k]][a.y[k]]=6;
						a.m=a.m*j/(a.l+1);
						a.l=j-1;
						if(j==0)
						{
							gotoxy(0,YY+5);
							printf("\n2 win(1) \n");
							bb++;
							while(!GetAsyncKeyState(VK_SPACE))Sleep(50);Sleep(500);goto A;
							return 0;
						}
					}
					if(s[c[n].x][c[n].y]==2)
					{
						c[n].v=0;
						s[c[n].x][c[n].y]=0;
						for(j=0;j<=b.l;j++)
							if(c[n].x==b.x[j]&&c[n].y==b.y[j])
								break;
						for(k=j+1;k<=b.l;k++)
							s[b.x[k]][b.y[k]]=6;
						b.m=b.m*j/(b.l+1);
						b.l=j-1;
						if(j==0)
						{
							gotoxy(0,YY+5);
							printf("\n1 win(1) \n");
							aa++;
							while(!GetAsyncKeyState(VK_SPACE))Sleep(50);Sleep(500);goto A;
							return 0;
						}
					}
					if(s[c[n].x][c[n].y]==6)
					{
						c[n].v=0;
						s[c[n].x][c[n].y]=0;
					}
					if(s[c[n].x][c[n].y]==4)
					{
						for(j=0;j<n;j++)
							if(c[j].v &&
							   c[n].x==c[j].x &&
							   c[n].y==c[j].y)
									break;
						if(c[j].f!=c[n].f)
						{
							c[n].v=0;
							c[j].v=0;
							s[c[j].x][c[j].y]=0;
						}
					}
					if(s[c[n].x][c[n].y]!=0)
						s[c[n].x][c[n].y]=0;
					if(mine(c[n].x,c[n].y,c[n].f))
						c[n].v=0;
				}
				n++;
		}		
		else
			a.t=0;
		if(s[a.x[0]+dx[a.f]][a.y[0]+dy[a.f]]==5)
			a.m+=3;	
		if(s[a.x[0]+dx[a.f]][a.y[0]+dy[a.f]]==3)
		{
			a.l++;
			a.m+=1;
			for(i=a.l;i>0;i--)
			{
				a.x[i]=a.x[i-1];
				a.y[i]=a.y[i-1];
			}
			a.x[0]+=dx[a.f];
			a.y[0]+=dy[a.f];
a.x[0]=(a.x[0]+XX)%XX;
a.y[0]=(a.y[0]+YY)%YY;
			s[a.x[0]][a.y[0]]=1;
		}
		else
		{
			s[a.x[a.l]][a.y[a.l]]=0;
			for(i=a.l;i>0;i--)
			{
				a.x[i]=a.x[i-1];
				a.y[i]=a.y[i-1];
			}
			if(//a.x[0]+dx[a.f]<0 || a.x[0]+dx[a.f]>=XX||
			   //a.y[0]+dy[a.f]<0 || a.y[0]+dy[a.f]>=YY||
			   !va(a.f,a.x[0],a.y[0]))
			{
				gotoxy(0,YY+5);
				printf("\n2 win(2) \n");
				bb++;
				while(!GetAsyncKeyState(VK_SPACE))Sleep(50);Sleep(500);goto A;
				return 0;
			}
			a.x[0]+=dx[a.f];
			a.y[0]+=dy[a.f];
a.x[0]=(a.x[0]+XX)%XX;
a.y[0]=(a.y[0]+YY)%YY;
			s[a.x[0]][a.y[0]]=1;
		}
		
			for(j=1;j<=VV/2;j++)
			for(i=0;i<n;i++)
			{
				if(c[i].v&&c[i].t<tt&&c[i].t>-1)
				{
					if((c[i].x!=a.x[0]||c[i].y!=a.y[0])&&(c[i].x!=b.x[0]||c[i].y!=b.y[0]))s[c[i].x][c[i].y]=0;
					if(c[i].x+dx[c[i].f]<0||c[i].x+dx[c[i].f]>=XX||
					   c[i].y+dy[c[i].f]<0||c[i].y+dy[c[i].f]>=YY)
					{
						c[i].v=0; 
						continue;
					}
					if(s[c[i].x+dx[c[i].f]][c[i].y+dy[c[i].f]]==1)
					{
						c[i].v=0;
						s[c[i].x+dx[c[i].f]][c[i].y+dy[c[i].f]]=0;
						for(j=0;j<=a.l;j++)
							if(c[i].x+dx[c[i].f]==a.x[j]&&c[i].y+dy[c[i].f]==a.y[j])
								break;
						for(k=j+1;k<=a.l;k++)
							s[a.x[k]][a.y[k]]=6;
						a.m=a.m*j/(a.l+1);
						a.l=j-1;
						if(j==0)
						{
							gotoxy(0,YY+5);
							printf("\n2 win(1.)\n");
							bb++;
							while(!GetAsyncKeyState(VK_SPACE))Sleep(50);Sleep(500);goto A;
							return 0;
						}
					}
					if(s[c[i].x+dx[c[i].f]][c[i].y+dy[c[i].f]]==2)
					{
						c[i].v=0;
						s[c[i].x+dx[c[i].f]][c[i].y+dy[c[i].f]]=0;
						for(j=0;j<=b.l;j++)
							if(c[i].x+dx[c[i].f]==b.x[j]&&c[i].y+dy[c[i].f]==b.y[j])
								break;
						for(k=j+1;k<=b.l;k++)
							s[b.x[k]][b.y[k]]=6;
						b.m=b.m*j/(b.l+1);
						b.l=j-1;
						if(j==0)
						{
							gotoxy(0,YY+5);
							printf("\n1 win(1.)\n");
							aa++;
							while(!GetAsyncKeyState(VK_SPACE))Sleep(50);Sleep(500);goto A;
							return 0;
						}
					}
					if(s[c[i].x+dx[c[i].f]][c[i].y+dy[c[i].f]]==6)
					{
						c[i].v=0;
						s[c[i].x+dx[c[i].f]][c[i].y+dy[c[i].f]]=0;
					}
					if(s[c[i].x+dx[c[i].f]][c[i].y+dy[c[i].f]]==4)
					{
						c[i].v=0;
						for(j=0;j<n;j++)
							if(c[j].v && j!=i &&
							   c[i].x+dx[c[i].f]==c[j].x &&
							   c[i].y+dy[c[i].f]==c[j].y)
									break;
						c[j].v=0;
						s[c[j].x][c[j].y]=0;	
					}
					if(s[c[i].x+dx[c[i].f]][c[i].y+dy[c[i].f]]!=0)
						s[c[i].x+dx[c[i].f]][c[i].y+dy[c[i].f]]=0;
					if(mine(c[i].x+dx[c[i].f],c[i].y+dy[c[i].f],c[i].f))
						c[i].v=0;
					c[i].x+=dx[c[i].f];
					c[i].y+=dy[c[i].f];
				}
				if(c[i].v)
					s[c[i].x][c[i].y]=4;
				if(c[i].t<=-1)
					c[i].t=0;
			}
			
		s[a.x[0]][a.y[0]]=1;
		s[b.x[0]][b.y[0]]=2;
		
		gotoxy(0,0);
		tt++;
		pri();
		t1=GetTickCount();
		if(mode<=2)
			Sleep(DD-t1+t2);
//		while(!GetAsyncKeyState(VK_SPACE))Sleep(50);
		
		
		
		//b
		h=2;
		t2=GetTickCount();
		if(mode==1)
		{
			b.u[1]=GetAsyncKeyState(VK_LEFT);
			b.u[2]=GetAsyncKeyState(VK_RIGHT);
			b.u[3]=GetAsyncKeyState(VK_UP);
			b.u[4]=GetAsyncKeyState(VK_DOWN);
		}
		else
		{
			opt=ff(b.f,b.x[0],b.y[0],b.m,a.f,a.x[0],a.y[0]);
			b.u[1]=op1[opt];
			b.u[2]=op2[opt];
			b.u[3]=op3[opt];
			b.u[4]=op4[opt];
		}
		if(b.u[1])
			b.f=(b.f+1)%4;
		if(b.u[2])
			b.f=(b.f+3)%4;
		if(b.u[4]&&b.m>=1&&b.l>=2)
		{
			m++;
			b.m-=1;
			t[b.x[b.l]][b.y[b.l]]++;
			s[b.x[b.l]][b.y[b.l]]=0;
			s[b.x[b.l-1]][b.y[b.l-1]]=0;
			b.l-=2;
		}
		if(b.u[3]&&b.m>=1)
		{
			b.t=1;
			b.m-=1;
			c[n].v=1;
			c[n].t=tt;
			c[n].f=b.f;
			c[n].x=b.x[0];
			c[n].y=b.y[0];
			for(i=1;i<=VV;i++)
				if(c[n].v)
				{
					c[n].x+=dx[c[n].f];
					c[n].y+=dy[c[n].f];
					if(s[c[n].x][c[n].y]==1)
					{
						c[n].v=0;
						s[c[n].x][c[n].y]=0;
						for(j=0;j<=a.l;j++)
							if(c[n].x==a.x[j]&&c[n].y==a.y[j])
								break;
						for(k=j+1;k<=a.l;k++)
							s[a.x[k]][a.y[k]]=6;
						a.m=a.m*j/(a.l+1);
						a.l=j-1;
						if(j==0)
						{
							gotoxy(0,YY+5);
							printf("\n2 win(1) \n");
							bb++;
							while(!GetAsyncKeyState(VK_SPACE))Sleep(50);Sleep(500);goto A;
							return 0;
						}
					}
					if(s[c[n].x][c[n].y]==2)
					{
						c[n].v=0;
						s[c[n].x][c[n].y]=0;
						for(j=0;j<=b.l;j++)
							if(c[n].x==b.x[j]&&c[n].y==b.y[j])
								break;
						for(k=j+1;k<=b.l;k++)
							s[b.x[k]][b.y[k]]=6;
						b.m=b.m*j/(b.l+1);
						b.l=j-1;
						if(j==0)
						{
							gotoxy(0,YY+5);
							printf("\n1 win(1) \n");
							aa++;
							while(!GetAsyncKeyState(VK_SPACE))Sleep(50);Sleep(500);goto A;
							return 0;
						}
					}
					if(s[c[n].x][c[n].y]==6)
					{
						c[n].v=0;
						s[c[n].x][c[n].y]=0;
					}
					if(s[c[n].x][c[n].y]==4)
					{
						for(j=0;j<n;j++)
							if(c[j].v &&
							   c[n].x==c[j].x &&
							   c[n].y==c[j].y)
									break;
						if(c[j].f!=c[n].f)
						{
							c[n].v=0;
							c[j].v=0;
							s[c[j].x][c[j].y]=0;
						}
					}
					if(s[c[n].x][c[n].y]!=0)
						s[c[n].x][c[n].y]=0;
					if(mine(c[n].x,c[n].y,c[n].f))
						c[n].v=0;
				}
				n++;
			}
		else
			b.t=0;
		if(s[b.x[0]+dx[b.f]][b.y[0]+dy[b.f]]==5)
			b.m+=3;
		if(s[b.x[0]+dx[b.f]][b.y[0]+dy[b.f]]==3)
		{
			b.l++;
			b.m+=1;
			for(i=b.l;i>0;i--)
			{
				b.x[i]=b.x[i-1];
				b.y[i]=b.y[i-1];
			}
			b.x[0]+=dx[b.f];
			b.y[0]+=dy[b.f];
			s[b.x[0]][b.y[0]]=2;
		}
		else
		{
			s[b.x[b.l]][b.y[b.l]]=0;
			for(i=b.l;i>0;i--)
			{
				b.x[i]=b.x[i-1];
				b.y[i]=b.y[i-1];
			}
			if(//b.x[0]+dx[b.f]<0 || b.x[0]+dx[b.f]>=XX||
			   //b.y[0]+dy[b.f]<0 || b.y[0]+dy[b.f]>=YY||
			   !va(b.f,b.x[0],b.y[0]))
			{
				gotoxy(0,YY+5);
				if(b.x[0]+dx[b.f]==a.x[0] && b.y[0]+dy[b.f]==a.y[0])
				{
					printf("end");
					while(!GetAsyncKeyState(VK_SPACE))Sleep(50);Sleep(500);goto A;
				}
			      else
				{
					printf("\n1 win(2) \n");
					aa++;
					while(!GetAsyncKeyState(VK_SPACE))Sleep(50);Sleep(500);goto A;
			 	}
				return 0;
			}
			b.x[0]+=dx[b.f];
			b.y[0]+=dy[b.f];
b.x[0]=(b.x[0]+XX)%XX;
b.y[0]=(b.y[0]+YY)%YY;
			s[b.x[0]][b.y[0]]=2;
		}
		
		
		for(j=1;j<=VV/2;j++)
			for(i=0;i<n;i++)
			{
				if(c[i].v&&c[i].t<tt&&c[i].t>-1)
				{
					if((c[i].x!=a.x[0]||c[i].y!=a.y[0])&&(c[i].x!=b.x[0]||c[i].y!=b.y[0]))s[c[i].x][c[i].y]=0;
					if(c[i].x+dx[c[i].f]<0||c[i].x+dx[c[i].f]>=XX||
					   c[i].y+dy[c[i].f]<0||c[i].y+dy[c[i].f]>=YY)
					{
						c[i].v=0; 
						continue;
					}
					if(s[c[i].x+dx[c[i].f]][c[i].y+dy[c[i].f]]==1)
					{
						c[i].v=0;
						s[c[i].x+dx[c[i].f]][c[i].y+dy[c[i].f]]=0;
						for(j=0;j<=a.l;j++)
							if(c[i].x+dx[c[i].f]==a.x[j]&&c[i].y+dy[c[i].f]==a.y[j])
								break;
						for(k=j+1;k<=a.l;k++)
							s[a.x[k]][a.y[k]]=6;
						a.m=a.m*j/(a.l+1);
						a.l=j-1;
						if(j==0)
						{
							gotoxy(0,YY+5);
							printf("\n2 win(1*)\n");
							bb++;
							while(!GetAsyncKeyState(VK_SPACE))Sleep(50);Sleep(500);goto A;
							return 0;
						}
					}
					if(s[c[i].x+dx[c[i].f]][c[i].y+dy[c[i].f]]==2)
					{
						c[i].v=0;
						s[c[i].x+dx[c[i].f]][c[i].y+dy[c[i].f]]=0;
						for(j=0;j<=b.l;j++)
							if(c[i].x+dx[c[i].f]==b.x[j]&&c[i].y+dy[c[i].f]==b.y[j])
								break;
						for(k=j+1;k<=b.l;k++)
							s[b.x[k]][b.y[k]]=6;
						b.m=b.m*j/(b.l+1);
						b.l=j-1;
						if(j==0)
						{
							gotoxy(0,YY+5);
							printf("\n1 win(1*)\n");
							aa++;
							while(!GetAsyncKeyState(VK_SPACE))Sleep(50);Sleep(500);goto A;
							return 0;
						}
					}
					if(s[c[i].x+dx[c[i].f]][c[i].y+dy[c[i].f]]==6)
					{
						c[i].v=0;
						s[c[i].x+dx[c[i].f]][c[i].y+dy[c[i].f]]=0;
					}
					if(s[c[i].x+dx[c[i].f]][c[i].y+dy[c[i].f]]==4)
					{
						c[i].v=0;
						for(j=0;j<n;j++)
							if(c[j].v && j!=i &&
							   c[i].x+dx[c[i].f]==c[j].x &&
							   c[i].y+dy[c[i].f]==c[j].y)
									break;
						c[j].v=0;
						s[c[j].x][c[j].y]=0;	
					}
					if(s[c[i].x+dx[c[i].f]][c[i].y+dy[c[i].f]]!=0)
						s[c[i].x+dx[c[i].f]][c[i].y+dy[c[i].f]]=0;
					if(mine(c[i].x+dx[c[i].f],c[i].y+dy[c[i].f],c[i].f))
						c[i].v=0;
					c[i].x+=dx[c[i].f];
					c[i].y+=dy[c[i].f];
				}
				if(c[i].v)
					s[c[i].x][c[i].y]=4;
				if(c[i].t<=-1)
					c[i].t=0;
			}
		
		s[a.x[0]][a.y[0]]=1;
		s[b.x[0]][b.y[0]]=2;
		
		if(tt%11==1)
		{
			do{
				rx=rand()%XX;
				ry=rand()%YY;
			}while(s[rx][ry]!=0&&rand()%3);
			if(s[rx][ry]==0)
				s[rx][ry]=3;//c
		}
		
		if(tt%21==0)
		{
			do{
				rx=rand()%XX;
				ry=rand()%YY;
			}while(s[rx][ry]!=0&&rand()%3);
			if(s[rx][ry]==0)
				s[rx][ry]=5;//e
		}
		gotoxy(0,0);
		tt++;
		pri();
		t1=GetTickCount();
		if(mode<=2)
			Sleep(DD-t1+t2);
//		while(!GetAsyncKeyState(VK_SPACE))Sleep(50);
	}
	return 0;
}

void pri()
{
	int i,j;
	int p[10]={' ','1','2','c','!','e','#'};
	int q[3]={' ','!'};
	printf("X");
	for(i=0;i<XX;i++)
		printf("X");
	printf("X\n");
	for(j=0;j<YY;j++)
	{
		printf("X");
		for(i=0;i<XX;i++)
			printf("%c",p[s[i][j]]);
		printf("X%d",j%10);
		printf("  X");
		for(i=0;i<XX;i++)
		{
			if(t[i][j])
				printf("*");
			  else
				printf(" ");
		}
		printf("X\n");
	}
	for(i=0;i<=XX;i++)
		printf("X");
	printf("X\n ");
	for(i=0;i<XX;i++)
		printf("%d",i%10);
	printf("\n1:%-4.1f %c %-3d %-3d\n2:%-4.1f %c %-3d %-3d\n%-3d%-3d%-4d%-5d\n",a.m,q[a.t],a.x[0],a.y[0],b.m,q[b.t],b.x[0],b.y[0],aa,bb,n,tt);
}

int begi()	
{
	int i,j,t=0;
	int x[5]={XX/2-6,XX/2-6,XX/2-8};
	int y[5]={YY/4,YY*2/3-1,YY*2/3+1};
	char w[5][20]={"Snake Trouble","Shift: Start","Space: computer"/*,"F2 Continue"*/};
	for(i=0;i<=XX;i++)
		printf("X");
	printf("X\n");
	for(j=1;j<=YY;j++)
	{
		printf("X");
		if(y[t]==j)
		{
			gotoxy(x[t],j);
			printf("%s",w[t]);
			t++;
		}
		gotoxy(XX+1,j);
		printf("X\n");
	}
	for(i=0;i<=XX;i++)
		printf("X");
	printf("X\n");
	while(1)
	{
		if(GetAsyncKeyState(VK_SHIFT))
			return 1;
		if(GetAsyncKeyState(VK_F2))
			return 0;
		if(GetAsyncKeyState(VK_SPACE))
			return 2;
		if(GetAsyncKeyState(VK_TAB))
		{
			DD=50;
			return 3;
		}
		Sleep(50);
	}
}

int ccc[5][5]={{0,1,3},{0,1,2},{1,2,3},{0,2,3}};
int mine(int x,int y,int f)
{
	int v,i,a,b;
	if(t[x][y])
	{
		t[x][y]--;
		v=rand()%3?3:2;
		if(v==3)
			for(i=0;i<v;i++)
			{
				c[n].v=1;
				c[n].t=-2;
				c[n].f=ccc[f][i];
				c[n].x=x;
				c[n].y=y;
				n++;
			}
		  else
			{
				a=rand()%3;
				do{ 
				b=rand()%3;
				}while(a==b); 
				c[n].v=1;
				c[n].t=-2;
				c[n].f=ccc[f][a];
				c[n].x=x;
				c[n].y=y;
				n++;
				c[n].v=1;
				c[n].t=-2;
				c[n].f=ccc[f][b];
				c[n].x=x;
				c[n].y=y;
				n++;
			}
		return 1;
	}
	return 0;
}

void reset()
{
	int i,j,rx,ry;
	n=m=0;
	for(j=0;j<YY;j++)
		for(i=0;i<XX;i++)
			s[i][j]=t[i][j]=0;
	
	a.x[0]=rand()%XX;
	a.y[0]=rand()%YY;
	a.f=rand()%4;
	a.l=3;
	a.m=15.0;
	s[a.x[0]][a.y[0]]=1;
	
	do{
		b.x[0]=rand()%XX;
	}while(abs(b.x[0]-a.x[0])<=XX/3 || abs(b.x[0]-a.x[0])>=2*XX/3);
	do{
		b.y[0]=rand()%YY;
	}while(abs(b.y[0]-a.y[0])<=YY/3 || abs(b.y[0]-a.y[0])>=2*YY/3);
	do{
		b.f=rand()%4;
	}while(b.f%2==a.f%2);
	b.l=3;
	b.m=15.0;
	s[b.x[0]][b.y[0]]=2;
	
	for(i=1;i<=3;i++)
	{
		do{
			rx=rand()%XX;
			ry=rand()%YY;
		}while(s[rx][ry]!=0);
		s[rx][ry]=3;//c
	}
	for(i=1;i<=2;i++)
	{
		do{
			rx=rand()%XX;
			ry=rand()%YY;
		}while(s[rx][ry]!=0);
		s[rx][ry]=5;//e
	}
}
 
void readsave()
{
	char ch;
	int i,j;
	FILE*f=fopen("save.txt","r");
	for(j=0;j<YY;j++)
	{
		for(i=0;i<XX;i++)
		{
			fscanf(f,"%c",c);
			s[i][j]=ch-'0';
		}
		fscanf(f,"\n");
	}
	for(i=0;i<LL;i++)
		fscanf(f,"%d%d",a.x[i],a.y[i]);
	fscanf(f,"%d%d\n",a.f,a.l);
	
	for(i=0;i<LL;i++)
		fscanf(f,"%d%d",b.x[i],b.y[i]);
	fscanf(f,"%d%d\n",b.f,b.l);
	
	fscanf(f,"%d\n",n);
	for(i=0;i<NN;i++)
		fscanf(f,"%d%d%d%d\n",c[i].x,c[i].y,c[i].f,c[i].v);
	fclose(f);
}

void writesave()
{
	int i,j;
	FILE*f=fopen("save.txt","w");
	for(j=0;j<YY;j++)
	{
		for(i=0;i<XX;i++)
			fprintf(f,"%c",s[i][j]+'0');
		fprintf(f,"\n");
	}
	fprintf(f,"\n");
	for(i=0;i<LL;i++)
		fprintf(f,"%d %d\n",a.x[i],a.y[i]);
	fprintf(f,"%d %d\n\n",a.f,a.l);
	
	for(i=0;i<LL;i++)
		fprintf(f,"%d %d\n",b.x[i],b.y[i]);
	fprintf(f,"%d %d\n\n",b.f,b.l);
	
	fprintf(f,"%d\n",n);
	for(i=0;i<NN;i++)
		fprintf(f,"%d %d %d %d\n",c[i].x,c[i].y,c[i].f,c[i].v);
	fclose(f);
}
