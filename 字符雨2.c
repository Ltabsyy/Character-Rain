#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
#include <windows.h>
/**
 * 字符雨 Character Rain
 * by Ltabsyy
 **/
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

void gotoxy(int x, int y)
{
	COORD coord = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main()
{
	int RainIntensity = 1;//至少为1，值越大，雨越密集
	int RainFreshCycle = 50;//至少为0，值越大，雨下落速度越慢
	int SkyWidth = 120;//天空宽度120 189，值越大，雨越稀疏，需小于屏幕宽度
	int SkyHeight = 30;//天空高度30 50，需小于屏幕高度
	int i, j;
	struct Raindrop* drop;
	HANDLE hdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	// 隐藏光标
	CONSOLE_CURSOR_INFO cursor_info = {20, 0};
	SetConsoleCursorInfo(hdout, &cursor_info);
	//srand(time(0));//并不必要，反正看不出(doge)
	rain.numberOfDrops = 0;
	while(1)
	{
		// 获取窗口大小
		GetConsoleScreenBufferInfo(hdout, &csbi);
		SkyHeight = csbi.srWindow.Bottom-csbi.srWindow.Top+1;
		SkyWidth = csbi.dwSize.X;
		// 扩大存储空间
		drop =(struct Raindrop*) calloc(rain.numberOfDrops+RainIntensity, sizeof(struct Raindrop));
		for(i=0; i<rain.numberOfDrops; i++)
		{
			drop[i] = rain.drop[i];
			drop[i].y++;//雨滴下落
			if(drop[i].x >= SkyWidth) drop[i].x %= SkyWidth;
		}
		// 增加雨滴
		for(i=rain.numberOfDrops; i<rain.numberOfDrops+RainIntensity; i++)
		{
			drop[i].x = rand() % SkyWidth;
			drop[i].y = 0;
			drop[i].length = rand() % 4 + 2;//2~5长度
			drop[i].color = rand() % 0x10;
			drop[i].s =(char*) calloc(drop[i].length, sizeof(char));
			for(j=0; j<drop[i].length; j++)
			{
				drop[i].s[j] = rand() % 10 -0+'0';//0~9数字
			}
		}
		if(rain.numberOfDrops != 0) free(rain.drop);
		rain.drop = drop;
		rain.numberOfDrops += RainIntensity;
		// 输出
		for(i=0; i<rain.numberOfDrops; i++)
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
		Sleep(RainFreshCycle);
	}
	return 0;
}
