#include "header.h"

void main(int argc,char **argv)
{
	if(argc != 2)
	{
		printf("USAGE: ./a.out src_file\n");
		return;
	}

	int lenn = strlen(argv[1]);       
	char *iFile = calloc( lenn + 1, sizeof(char));  //creating .i file
	strcpy(iFile , argv[1]);
	iFile[lenn-1] = 'i';

	FILE *Main = fopen(iFile,"w");    //opening .i file
	FILE *fpp = fopen(argv[1],"a");
	if(fpp == 0)
	{
		printf("File is not present\n");
		return;
	}
	int size = ftell(fpp);    // finding the total length of file
	fclose(fpp);
	free(iFile);

	FILE *fp = fopen(argv[1],"r");   // opening the src file to read data
	if(fp == 0)
	{
		printf("File is not present\n");
		return;
	}
	char ch; 

	char *data = calloc(size + 1,sizeof(char));  //allocating memory as per size of file
	int i = 0,longLine = 0,c = 0;
	while((ch = fgetc(fp)) != EOF)
	{
		c++;
		data[i++] = ch;
		if(ch == '\n')
		{
			if(c > longLine)      // finding long line length
				longLine = c; 
			c = 0;
		}
	}
	data[i] = 0;

	removeComments(data);  // removing comments in file

	FILE *ft = fopen("t","w");

	i = 0;
	while(data[i])
		fputc(data[i++], ft);
	fclose(ft);

	headerInclusion(Main,"t",size);   // including header file

	FILE *ftt = fopen("t" , "r");

	char *line = calloc(longLine + 1,sizeof(char));      
	char *s = calloc(size + 1,sizeof(char));
	char *name = calloc(longLine ,sizeof(char));
	char *body = calloc(longLine ,sizeof(char));

	int j = 0,k = 0,f,len = 0,flag = 0,m = 0;
	int Index = 0;

	while(fgets(line,longLine + 1,ftt))     // replacing macros
		if(strstr(line,"#define"))
			if(strstr(data , line))
			{
				for(i = 0,k = 0,f = 0,j = 0 , flag = 0, m = 0; line[i] ; i++)
				{
					if( f == 0 ) 
					{
						if(line[i] == 32) 
							m = 1;
						if(m == 1)
							if((line[i] >= 'a' && line[i] <= 'z') || (line[i] >= 'A' && line[i] <= 'Z') || line[i] == '_' || (line[i] >= '0' && line[i] <= '9'))
								f = 1;
					}
					if(f == 1)
					{
						if(line[i] == '(')
							flag = 1;
						if(flag == 0) 
							if(line[i] == 32)
							{
								f = 2;
								continue;
							}
						if(flag == 1)
							if(line[i] == ')')
								flag = 0;

						name[j++] = line[i];
					}
					if(f == 2)
					{
						if(line[i] == '\n')
							continue;
						body[k++] = line[i];
					}
				}
				name[j] = 0;
				body[k] = 0;
				replaceMacro(&data, &s, name, body, &size);
			}
	len = strlen(data);
	data[len] = 0;
	data = realloc(data ,(len - 1) * sizeof(char)); 
	char st[20]="main(";

	for( i = 0 , Index = 0 ,f = 0; data[i] ; i++)
	{
		if(data[i] == '\n')
			Index = i + 1;
		for(j = 0 ,k = i ; st[j] ; j++ ,k++)
			if(st[j] != data[k])
				break;

		if(st[j] == 0) {
			f = 1;
			break;
		}
	}
	if(f == 1)
		i = Index;
	else 
		i = 0;
	while(data[i])
		fputc(data[i++] , Main);

	free(data);
	free(name);
	free(body);
	free(line);

	remove("t");
	fclose(fp);
	fclose(Main);
	fclose(ftt);

}

