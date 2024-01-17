#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
#include <windows.h>
/**
 * 字符雨 Character Rain
 * by Ltabsyy
 * https://github.com/Ltabsyy/Character-Rain
 */
#define RainIntensity 1//至少为1，值越大，雨越密集
#define RainFreshCycle 50//至少为0，值越大，雨下落速度越慢
#define SkyWidth 120//天空宽度120 189，值越大，雨越稀疏，需小于屏幕宽度
#define SkyHeight 30//天空高度30 50，需小于屏幕高度

struct Raindrop
{
	int x, y;//雨滴头部坐标
	int length;//雨滴含头长度
	int color;//雨滴颜色
	char* s;//从头开始字符串
};
struct Raindrops
{
	struct Raindrop* drop;
	int numberOfDrops;
}rain;
/*struct Sky
{
	char space;
	int color;
}sky[SkyHeight][SkyWidth];*/

void gotoxy(int x, int y)
{
	COORD coord = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void showcursor(int visible)
{
	CONSOLE_CURSOR_INFO cursor_info = {20, visible};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void InitRain()
{
	//srand(time(0));//并不必要，反正看不出(doge)
	rain.drop = NULL;
	rain.numberOfDrops = 0;
}

/*int SkyWidth = 120;//天空宽度120 189，值越大，雨越稀疏，需小于屏幕宽度
int SkyHeight = 30;//天空高度30 50，需小于屏幕高度
void FreshSky()
{
	HANDLE hdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	// 隐藏光标，避免切换全屏后光标显示
	CONSOLE_CURSOR_INFO cursor_info = {20, 0};
	SetConsoleCursorInfo(hdout, &cursor_info);
	// 获取窗口大小
	GetConsoleScreenBufferInfo(hdout, &csbi);
	SkyHeight = csbi.srWindow.Bottom-csbi.srWindow.Top+1;
	SkyWidth = csbi.dwSize.X;
}*/

void FreshRain()
{
	int i, j, clean;
	struct Raindrop* drop;
	// 落地雨滴清理
	clean = 0;
	for(i=0; i<rain.numberOfDrops; i++)
	{
		if(rain.drop[i].y - rain.drop[i].length + 1 >= SkyHeight)
		{
			clean = i+1;//记录清理数量
		}
		else
		{
			break;//仅清理连续部分
		}
	}
	rain.numberOfDrops -= clean;
	// 刷新存储空间
	drop =(struct Raindrop*) calloc(rain.numberOfDrops+RainIntensity, sizeof(struct Raindrop));
	for(i=0; i<rain.numberOfDrops; i++)
	{
		//drop[i] = rain.drop[i];
		drop[i] = rain.drop[i+clean];//跳过落地雨滴
	}
	// 雨滴下落
	for(i=0; i<rain.numberOfDrops; i++)
	{
		//if(drop[i].x >= SkyWidth) drop[i].x %= SkyWidth;
		drop[i].y++;
	}
	// 增加雨滴
	for(i=rain.numberOfDrops; i<rain.numberOfDrops+RainIntensity; i++)
	{
		drop[i].x = rand() % SkyWidth;
		drop[i].y = 0;
		drop[i].length = rand() % 4 + 2;//2~5长度
		//drop[i].color = 0x0a;
		drop[i].color = rand() % 0x10;
		drop[i].s =(char*) calloc(drop[i].length, sizeof(char));
		for(j=0; j<drop[i].length; j++)
		{
			//drop[i].s[j] = rand() % 2 -0+'0';//01数字
			drop[i].s[j] = rand() % 10 -0+'0';//0~9数字
		}
	}
	if(rain.numberOfDrops != 0) free(rain.drop);
	rain.drop = drop;
	rain.numberOfDrops += RainIntensity;
}

void PrintRain()
{
	int i, j;
	HANDLE hdout = GetStdHandle(STD_OUTPUT_HANDLE);
	gotoxy(0, 0);
	/*for(i=0; i<SkyHeight; i++)
	{
		for(j=0; j<SkyWidth; j++)
		{
			sky[i][j].space = ' ';
			sky[i][j].color = 0x07;
		}
	}
	for(i=0; i<rain.numberOfDrops; i++)
	{
		for(j=0; j<rain.drop[i].length; j++)
		{
			if(rain.drop[i].y-j >= 0 && rain.drop[i].y-j < SkyHeight)
			{
				sky[rain.drop[i].y-j][rain.drop[i].x].space = rain.drop[i].s[j];
				sky[rain.drop[i].y-j][rain.drop[i].x].color = rain.drop[i].color;
			}
		}
	}
	for(i=0; i<SkyHeight; i++)
	{
		for(j=0; j<SkyWidth; j++)
		{
			SetConsoleTextAttribute(hdout, sky[i][j].color);
			printf("%c ", sky[i][j].space);
			SetConsoleTextAttribute(hdout, 0x07);
		}
		printf("\n");
	}*/
	for(i=0; i<rain.numberOfDrops; i++)//按雨滴绘制，优化性能，减少果冻感
	{
		SetConsoleTextAttribute(hdout, rain.drop[i].color);
		for(j=0; j<rain.drop[i].length; j++)
		{
			if(rain.drop[i].y-j >= 0 && rain.drop[i].y-j < SkyHeight)
			{
				gotoxy(rain.drop[i].x, rain.drop[i].y-j);
				printf("%c", rain.drop[i].s[j]);
			}
		}
		SetConsoleTextAttribute(hdout, 0x07);
		if(rain.drop[i].y-j >= 0 && rain.drop[i].y-j < SkyHeight)//j为length
		{
			gotoxy(rain.drop[i].x, rain.drop[i].y-j);
			printf(" ");//尾部空格覆写
		}
	}
}

int main()
{
	/*printf("字符雨Character Rain\n按回车开始");
	getchar();
	system("cls");//清除提示字符*/
	showcursor(0);//隐藏光标
	InitRain();
	while(1)
	{
		//FreshSky();
		FreshRain();
		PrintRain();
		Sleep(RainFreshCycle);
	}
	return 0;
}
