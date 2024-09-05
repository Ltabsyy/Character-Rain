#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
#include <unistd.h>
/**
 * 字符雨 Character Rain
 * by Ltabsyy
 * https://github.com/Ltabsyy/Character-Rain
 */
#define RainIntensity 1//至少为1，值越大，雨越密集
#define RainFreshCycle 50//至少为0，值越大，雨下落速度越慢
int SkyWidth = 120;//天空宽度120 189，值越大，雨越稀疏，需小于屏幕宽度
int SkyHeight = 30;//天空高度30 50，需小于屏幕高度

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

//0 black 1 red 2 green 3 yellow 4 blue 5 purple 6 cyan 7 white
const int cl[8] = {0, 4, 2, 6, 1, 3, 5, 7};
void SetColor(int color)
{
	int backColor, foreColor, foreLight = 2;
	backColor = color/16;
	foreColor = color%16;
	if(backColor < 8) backColor = cl[backColor];
	else backColor = cl[backColor-8];
	if(foreColor < 8) foreColor = cl[foreColor];
	else foreColor = cl[foreColor-8], foreLight = 1;
	printf("\033[4%d;3%d;%dm", backColor, foreColor, foreLight);
}

void PrintRain()
{
	int i, j;
	printf("\033[1;1H");
	for(i=0; i<rain.numberOfDrops; i++)//按雨滴绘制，优化性能，减少果冻感
	{
		SetColor(rain.drop[i].color);
		for(j=0; j<rain.drop[i].length; j++)
		{
			if(rain.drop[i].y-j >= 0 && rain.drop[i].y-j < SkyHeight)
			{
				printf("\033[%d;%dH", rain.drop[i].y-j+1, rain.drop[i].x+1);
				printf("%c", rain.drop[i].s[j]);
			}
		}
		printf("\033[0m");
		if(rain.drop[i].y-j >= 0 && rain.drop[i].y-j < SkyHeight)//j为length
		{
			printf("\033[%d;%dH", rain.drop[i].y-j+1, rain.drop[i].x+1);
			printf(" ");//尾部空格覆写
		}
	}
}

int main()
{
	printf("\033[?25l");//隐藏光标
	//srand(time(0));//并不必要，反正看不出(doge)
	rain.drop = NULL;
	rain.numberOfDrops = 0;
	while(1)
	{
		//FreshSky();
		FreshRain();
		PrintRain();
		//Sleep(RainFreshCycle);
		usleep(1000*RainFreshCycle);
	}
	return 0;
}
