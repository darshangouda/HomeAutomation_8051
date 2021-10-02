#include<reg51.h>
sbit rs = P2^0; 
sbit rw = P2^1;
sbit en = P2^2;
sfr lcd = 0x90; // Port 0

sbit load1=P3^2;
sbit load2=P3^3;
sbit load3=P3^4;
sbit load4=P3^5;

void delay(unsigned int t)
{
int i,j;
for(i=0;i<t;i++)
{
for(j=0;j<300;j++);
}
}

void lcdcmd(unsigned char c)
{
rs = 0; // com
rw = 0; // write
lcd = c; 
en = 1;
delay(2);
en = 0;
delay(2);
}

void lcddata(unsigned char d)
{
rs = 1; // data
rw = 0; // write
lcd = d;
en = 1;
delay(2);
en = 0;
delay(2);
}

void dis(char *str)
{
int i,j;
for(i=0;i<str[i]!='\0';i++)
lcddata(str[i]);

for(i=0;i<str[i]!='\0';i++)	
{ 
	j=1000;
	SBUF = str[i];
	while(TI == 0 && --j);
	TI = 0;
}
j=1000;
SBUF = '\n';
while(TI == 0 && --j);
TI = 0;
}


void UART_INIT()
{

SCON = 0x50; //8bit UART with varying baud rate (0101 0000)
TMOD = 0x20;
TH1 = 0xFD;//9600
TR1 = 1; 
	
//SBUF = a;// data
//while(TI == 0); // TI = 1
//TI = 0; // clear
}

void main()
{
char a;
int change,L1stat=0,L2stat=0,L3stat=0,L4stat=0;
load1=load2=load3=load4=0;
UART_INIT();
lcdcmd(0x38);// 8bit, 2 lines
lcdcmd(0x01); // clear
lcdcmd(0x0E); // cursor on
lcdcmd(0x80); // start position
L1stat=L2stat=L3stat=L4stat=0;
change=0;

a='*';
change=1;	
while(1)
{  

	if(RI==1)
	{
		if(RI==1)
		{
			a=SBUF;
			RI=0;
		}
		if(a=='1')
	  { if (L1stat==0)L1stat=1;
		  else L1stat=0;
		}
		else if(a=='2')
	  { if (L2stat==0)L2stat=1;
		  else L2stat=0;
		}
		else if(a=='3')
	  { if (L3stat==0)L3stat=1;
		  else L3stat=0;
		}
		else if(a=='4')
	  { if (L4stat==0)L4stat=1;
		  else L4stat=0;
		}
		if(a!=13)
    change=1;		
	  //delay(1000);
   a='*';		
	}
	
	if(change==1)
	{ 
		load1=L1stat;
		load2=L2stat;
		load3=L3stat;
		load4=L4stat;
		change=0;
		lcdcmd(0x01);
		lcdcmd(0x80);
		if(L1stat==1) {  dis("1 : ON "); }
		else          {  dis("1 : OFF");}
		if(L2stat==1) {  dis("  2 : ON"); }
		else          {  dis("  2 : OFF");}
		
		lcdcmd(0xc0);
		if(L3stat==1) {  dis("3 : ON "); }
		else          {  dis("3 : OFF");}
		if(L4stat==1) {  dis("  4 : ON"); }
		else          {  dis("  4 : OFF");}
		
	}
}	
}