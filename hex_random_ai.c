#include<stdio.h>
#include<stdlib.h>
#define RED 0
#define BLUE 1
int board[11][11] = {0};
char cmd[256];
char response[256];
int color = 0;
void get_move(char* cmd)
{
	int x,y;
	x = cmd[0] - 'A';
	y = cmd[1] - 'A';
	board[x][y] = (!color)+1;
}
void make_move(char* response)
{
	//randomly choose a move
	srand(time(0));
	int x,y;
	do
	{
		x = rand()%11;
		y = rand()%11;
	}while(board[x][y]!=0);
	board[x][y] = color+1;
	response[0] = x+'A';
	response[1] = y+'A';
	response[2] = '\0';
}
int main()
{
	while(1)
	{
		//Get command
		scanf("%s",cmd);
		

		//return name
		if(!strcmp(cmd,"name?"))
		{
			fflush(stdin);
			printf("name Test\n");
			fflush(stdout);
			continue;
		}

		//exit
		if(!strcmp(cmd,"exit"))
		{
			fflush(stdin);
			exit(0);
		}

		//wrong command
		if(!strcmp(cmd,"wrong"))
		{
			fflush(stdin);
			continue;
		}

		//start 
		if(!strcmp(cmd,"start"))
		{
			scanf("%s",cmd);
			//fflush(stdin);
			//Red
			if(!strcmp(cmd,"red"))
			{
				color = RED;
				make_move(response);
				printf("move %s\n",response );
				fflush(stdout);
			}
			else//Blue
			{
				color = BLUE;
			}
			
			continue;
		}

		//make move
		if(!strcmp(cmd,"move"))
		{
			
			scanf("%s",cmd);
			fflush(stdin);
			get_move(cmd);
			make_move(response);
			printf("move %s\n",response);
			fflush(stdout);
			continue;
		}
		
	}
}
