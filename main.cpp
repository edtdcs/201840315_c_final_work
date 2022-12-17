#include <stdio.h>
#include <stdlib.h>
#include <easyx.h>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include<math.h>
//定义
#define Width 40*23
#define Height 800

struct Player {
	int x,y;//坐标
	IMAGE img;//图片
	int dirx,diry;//方向
	unsigned long t1,t2,dt;//时间
};//玩家
Player player;
Player enemy;
int x_1,x_2,y_1,y_2,nextx1,nextx2,nexty1,nexty2,score;
int t;
IMAGE img[4];
//1红 2墙 3白
int map[20][23]= {
	4,2,2,2,2,2,2,2,2,2,2,2,2,2,4,4,4,4,4,4,4,4,4,
	2,2,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,
	2,2,3,3,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,4,
	2,2,3,3,2,2,3,3,3,2,2,2,2,2,2,3,2,3,3,3,3,2,4,
	2,2,3,3,3,3,2,2,3,3,3,3,3,3,3,3,2,3,2,3,3,2,2,
	2,2,3,3,2,2,3,3,2,3,2,2,2,2,2,3,2,3,3,3,3,2,2,
	2,2,3,3,2,2,3,3,2,3,3,2,3,3,3,3,2,2,2,2,2,2,2,
	2,2,3,2,3,2,3,3,3,2,3,2,3,2,2,3,2,2,3,3,3,2,2,
	2,2,3,3,3,2,3,2,3,3,3,2,3,3,3,3,3,3,3,3,3,2,4,
	2,2,3,3,3,2,3,2,2,3,2,2,2,2,2,2,2,2,2,2,3,2,4,
	4,2,3,3,2,2,3,3,2,3,3,3,3,3,2,3,3,3,3,3,3,2,4,
	4,2,2,3,2,2,2,2,2,2,3,3,2,2,2,2,2,2,2,3,2,2,2,
	2,4,2,3,2,3,2,3,3,2,3,3,3,3,3,3,3,3,2,3,2,2,2,
	2,2,2,3,2,3,3,3,3,2,2,2,3,3,3,3,3,2,2,4,4,2,4,
	2,2,3,3,2,3,3,2,3,3,3,3,3,2,2,3,3,2,4,4,4,2,4,
	2,2,3,3,2,3,2,3,3,2,3,2,3,2,3,3,3,2,4,4,4,2,4,
	2,3,3,3,3,3,2,2,2,2,3,3,3,3,3,2,3,2,2,2,2,2,4,
	2,3,3,3,2,2,2,3,3,2,2,2,2,2,2,2,2,1,2,2,4,4,4,
	2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,2,4,4,4,
	4,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,4,4,4
};
int imgInDex[4]= {1,2,3,4};
void drawMap() {
	int x,y;
	for(int i=0; i<20; i++) {
		for(int j=0; j<23; j++) {
			x=40*j;
			y=40*i;
			switch(map[i][j]) {
				case 1:
					putimage(x,y,img+0);
					break;
				case 2:
					putimage(x,y,img+1);
					break;
				case 3:
					putimage(x,y,img+2);
					break;
				case 4:
					putimage(x,y,img+3);
					break;

			}
		}
	}
}
void loadResource() {
	for(int i=0; i<4; i++) {
		char filename[20]="";
		sprintf(filename,"%d.png",imgInDex[i]);
		loadimage(img+i,filename,40,40);
	}
}
//初始化
void GameInit() {
	//窗口
	initgraph(Width,Height);
	//背景
	loadResource();
	//bgm
//	mciSendString("open BGM.mp3",0,0,0);
//	mciSendString("play BGM.mp3",0,0,0);
	//玩家
	loadimage(&player.img,"6.jpg",40,40);
	player.x=20*40;
	player.y=14*40;
	player.dirx=0;
	player.diry=0;
	player.t1=GetTickCount();
	player.dt=5;

	//敌人
	loadimage(&enemy.img,"enemy.png",40,40);
	enemy.x=18*40;
	enemy.y=15*40;
	enemy.dirx=0;
	enemy.diry=0;
	enemy.t1=GetTickCount();
	enemy.dt=5;


}
//显示
void GameDraw() {
	BeginBatchDraw();
	//背景
	drawMap();
	//玩家
	putimage(player.x,player.y,&player.img);
	//敌人
	putimage(enemy.x,enemy.y,&enemy.img);
	EndBatchDraw();
}

//更新
void GameUpdate() {
	//玩家
	//手动移动
	char key;
	if(_kbhit()) {
		key=_getch();
		x_1=player.x, x_2=x_1+40, y_1=player.y, y_2=y_1+40;
		nextx1=x_1/40, nexty1=y_1/40, nextx2=x_2/40, nexty2=y_2/40;
		switch(key) {
			case 72: {


				player.dirx=0,player.diry=-1;
				break;
			}
			case 80: {

				player.dirx=0,player.diry=1;
				break;
			}
			case 75: {

				player.dirx=-1,player.diry=0;
				break;
			}
			case 77: {

				player.dirx=1,player.diry=0;
				break;
			}
		}
	}
//}
//void collsion(){
	x_1=player.x, x_2=x_1+40, y_1=player.y, y_2=y_1+40;
	nextx1=x_1/40, nexty1=y_1/40, nextx2=x_2/40, nexty2=y_2/40;
	//碰壁
	 
	switch (player.dirx) {
		case 1: {//右  以此为例说明碰撞判定，碰撞必然发生在边界，所以player.x%40==0时才需判断是否碰撞，当人物正好处于一个方块时，只需判定右边一个格子是否为墙或金币，
				//当人物处于两方块之间时，判定右侧两方块 ,许进政于12月14日17:49注。 
			if(player.x%40==0) {
				if(player.y%40==0) {
					if(map[nexty1][nextx2]==2) {
						player.dirx=0;
						break;
					}
					if(map[nexty1][nextx2]==3) {
					score++;
					map[nexty1][nextx2]=4;
				}
				}
				if(player.y%40!=0) {
					if(map[nexty1][nextx2]==2||map[nexty2][nextx2]==2) {
						player.dirx=0;
						break;
					}
					if(map[nexty1][nextx2]==3) {
					score++;
					map[nexty1][nextx2]=4;
				}
				if(map[nexty2][nextx2]==3) {
					score++;
					map[nexty2][nextx2]=4;
				}
				}
				
			}
			break;
		}

		case -1 : {//左 
			if(player.x%40==0) {
				if(player.y%40==0) {
					if(map[nexty1][nextx1-1]==2) {
						player.dirx=0;
						break;
					}
					if(map[nexty1][nextx1-1]==3) {
					score++;
					map[nexty1][nextx1-1]=4;
				}
				}
				if(player.y%40!=0) {
					if(map[nexty1][nextx1-1]==2||map[nexty2][nextx1-1]==2) {
						player.dirx=0;
						break;
					}
					if(map[nexty1][nextx1-1]==3) {
					score++;
					map[nexty1][nextx1-1]=4;
				}
				if(map[nexty2][nextx1-1]==3) {
					score++;
					map[nexty2][nextx1-1]=4;
				}
				}
				
			}
			break;
		}
	}
	switch (player.diry) {
		case 1 : { //下
			if(player.y%40==0) {
				if(player.x%40==0) {
					if(map[nexty2][nextx1]==2) {
						player.diry=0;
						break;
					}
					if(map[nexty2][nextx1]==3) {
					score++;
					map[nexty2][nextx1]=4;
				}
				}
				if(player.x%40!=0) {
					if(map[nexty2][nextx1]==2||map[nexty2][nextx2]==2) {
						player.diry=0;
						break;
					}
					if(map[nexty2][nextx1]==3) {
					score++;
					map[nexty2][nextx1]=4;
				}
				if(map[nexty2][nextx2]==3) {
					score++;
					map[nexty2][nextx2]=4;
				}
				}
				
			}
			break;
		}


		case -1 : { //上
			if(player.y%40==0) {
				if(player.x%40==0) {
					if(map[nexty1-1][nextx1]==2) {
						player.diry=0;
						break;
					}
					if(map[nexty1-1][nextx1]==3) {
					score++;
					map[nexty1-1][nextx1]=4;
				}
				}
				if(player.x%40!=0) {
					if(map[nexty1-1][nextx1]==2||map[nexty1-1][nextx2]==2) {
						player.diry=0;
						break;
					}
					if(map[nexty1-1][nextx1]==3) {
					score++;
					map[nexty1-1][nextx1]=4;
				}
				if(map[nexty1-1][nextx2]==3) {
					score++;
					map[nexty1-1][nextx2]=4;
				}
				}
				
			}
			break;
		}

	}

	//移动方向，速度
	player.t2=GetTickCount();
	if(player.t2-player.t1>=player.dt) {
		player.x+=2*player.dirx;
		player.y+=2*player.diry;
		player.t1=player.t2;
	}
	//敌人
	//移动速度
	enemy.t2=GetTickCount();
	if(enemy.t2-enemy.t1>=enemy.dt) {
		enemy.x+=1*enemy.dirx;
		enemy.y+=1*enemy.diry;
		enemy.t1=enemy.t2;
	}
	//方向
	int dx=abs(player.x-enemy.x);
	int dy=abs(player.y-enemy.y);
	if(player.x>=enemy.x&&player.y>=enemy.y) {
		if(dx>=dy)enemy.dirx=1;
		else enemy.diry=1;
	}
	if(player.x>=enemy.x&&player.y<enemy.y) {
		if(dx>=dy)enemy.dirx=1;
		else enemy.diry=-1;
	}
	if(player.x<enemy.x&&player.y>=enemy.y) {
		if(dx>=dy)enemy.dirx=-1;
		else enemy.diry=1;
	}
	if(player.x<enemy.x&&player.y<enemy.y) {
		if(dx>=dy)enemy.dirx=-1;
		else enemy.diry=-1;
	}

	//碰壁
	int ccx=enemy.x+14;
	int ccy=enemy.y+14;

	if(enemy.dirx==1) {
		for(int yy=ccy-13; yy<=ccy+13; yy++) {
			if(getpixel(ccx+15,yy)!=WHITE)
				enemy.dirx=0;
			break;
		}
	} else if(enemy.dirx==-1) {
		for(int yy=ccy-13; yy<=ccy+13; yy++) {
			if(getpixel(ccx-15,yy)!=WHITE)
				enemy.dirx=0;
			break;
		}
	} else if(enemy.diry==1) {
		for(int xx=ccx-13; xx<=ccx+13; xx++) {
			if(getpixel(xx,ccy+15)!=WHITE)
				enemy.diry=0;
			break;
		}
	} else if(enemy.diry==-1) {
		for(int xx=ccx-13; xx<=ccx+13; xx++) {
			if(getpixel(xx,ccy-15)!=WHITE)
				enemy.diry=0;
			break;
		}
	}
	//吃掉
}


int main() {
	GameInit();
	while(1) {
		GameDraw();
		GameUpdate();
		printf("%d",score); 
//			collsion();

	}

	return 0;
}

