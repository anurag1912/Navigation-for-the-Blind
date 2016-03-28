#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
int j=0;
int d=0;
char c[10];
char NORTH[310][256];
char WEST[310][256];
char ORIENT[310][256];
void function();
int k=0;
int m=0;
int main()
{
	FILE *fp ;
	fp = fopen("data[1].txt", "r");
	FILE  *pFile;
      pFile= fopen("/dev/mygpio","r+");

	char ch[310][256];
    int k[310];
    char p[310];
    char list[310][256];
	int i=0;
	while(!feof(fp)){
fgets (list[i], 600, fp);
//printf("%s\n",list[i]);
i++;
}

	fclose(fp);
	i=0;
	while(i<310)
	{// printf("%s",list[i]);
		i++;
     }
     i=0;
     while(i<310)
     {	
     	j=0;
	   	while(list[i][j]!='.')
	   		j++;

	   	j++;
	   	while(list[i][j]!=',')
	   	{
	   		c[d]=list[i][j];
	   		j++;
	   		d++;
	   	}
	   	c[d]='a';
		d++;
		c[d]='\0';
	   	d=0;
	   	sprintf(NORTH[i],"%s",c);
	       // printf("North[%d]=%s\n",i,NORTH[0]);
	   	while(list[i][j]!='.')
	   		j++;
	   	j++;
	   	while(list[i][j]!=',')
	   	{
	   		c[d]=list[i][j];
	   		j++;
	   		d++;
	   	}
	   	c[d]='a';
		d++;
	   	c[d]='\0';
	   	d=0;
	   sprintf(WEST[i],"%s",c);
	   	//printf("WEST[%d]=%s  ",i,WEST[i]);
	   	while(list[i][j]!=' ')
	   		j++;
	   	j++;
	   	while(list[i][j]!=' ')
	   	{
	   		c[d]=list[i][j];
	   		j++;
	   		d++;
	   	}
	   	c[d]='a';
		d++;
	   	c[d]='\0';
	   	d=0;
	   sprintf(ORIENT[i],"%s",c);
	   	//printf("ORIENT[%d]=%s\n",i,ORIENT[i]);
	   	i++;
	     }
	  
	 
	  
i=0;
while(i<930)
{
  function();
  i++;
  }
  fclose(pFile);
return 0;
}
void function()
{ int  pFile;
 pFile= open("/dev/mygpio",O_RDWR);
if(m==0){
 while(m==0){
write(pFile,NORTH[k],strlen(NORTH[k]));
m++;
break;
 }
}else if(m==1){
  while(m==1){
write(pFile,WEST[k],strlen(WEST[k]));
m++;
break;
 }
}
else if(m==2){
 while(m==2){
write(pFile,ORIENT[k],strlen(ORIENT[k]));
m=0;
k++;
break;
 }
}
close(pFile);


}
