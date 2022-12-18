#include <stdio.h>
#include <stdlib.h>
#include <easyx.h>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include<math.h>

#define Width 40*23
#define Height 800

struct Player {
	int x,y;//
	IMAGE img;//
	int dirx,diry;//
	unsigned long t1,t2,dt;//
};//
Player player;
Player enemy;
int x_1,x_2,y_1,y_2,nextx1,nextx2,nexty1,nexty2,score;
int t,wait_time,signal=0;
IMAGE img[4];
//define the map
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

int start(){
	setbkcolor(WHITE);
	cleardevice();
	setfillcolor(GREEN);
	solidrectangle(410, 365, 510, 435);
	settextcolor(BLACK);
	RECT r = { 410, 365, 510, 435 };
	drawtext(_T("START"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	ExMessage mouse;
	while (1){
		mouse = getmessage(EX_MOUSE | EX_KEY);
		switch (mouse.message) {
		case WM_LBUTTONDOWN: {
			//if the coordinate of mouse is in the reactangle,set signal to 1
			if (mouse.x > 410 && mouse.x < 510 && mouse.y > 365 && mouse.y < 435){
				signal = 1;
				return 0;}
			break;}
		case WM_KEYDOWN:{
			if (mouse.vkcode == VK_ESCAPE) {
				return 0;}
			break;}
		default: break;
		}
	}
}

int end(){
	solidrectangle(410, 365, 510, 435);
	settextcolor(BLACK);
	RECT r = { 410, 365, 510, 435 };
	drawtext(_T("game over!"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	ExMessage mouse;
	while (1){
		mouse = getmessage(EX_MOUSE | EX_KEY);
		switch (mouse.message){
		case WM_LBUTTONDOWN:{
			//如果鼠标在矩形里面，就把signal设置为true
			if (mouse.x > 410 && mouse.x < 510 && mouse.y>365 && mouse.y < 435){
				signal = 1;
				return 0;}
			break;}
		case WM_KEYDOWN: {
			if (mouse.vkcode == VK_ESCAPE) {
				exit(0);}
			break;}
		default:break;
        }
	}
}

void print_score()
{
	setfillcolor(WHITE);
    solidrectangle(410,0,510,50);
    settextcolor(BLACK);
    RECT r={410,0,510,50};
    char sc[15]="";
    sprintf(sc,"score : %d",score);
    drawtext(_T(sc),&r,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void menu(int s) {
	switch (s){
		case 1:{
			start();
			break;}
		case 0:{
			end();
			break;}	  
	}
}

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
//

int x_player_way[1000]={1,2,5,4,2,6,4};
int y_player_way[1000]={1,5,4,8,9,3,4};

void initialize_way(){
    //call this function when we initialize the game
    x_player_way[0]=18;
    y_player_way[0]=15;
    for(int i=1;i<1000;i++){
        x_player_way[i]=0;
        y_player_way[i]=0;
    }
}
int index=0;
void enemy_way(){
    //call the function when we call GAMEUPDAGE().
    //according player's track,apply it to move enemy
    int x,y;
    x=player.x/40;
    y=player.y/40;
    if((x_player_way[index]!=x)||(y_player_way[index]!=y)){
        x_player_way[index+1]=x;
        y_player_way[index+1]=y;
        index++;
    }
}

int track_index=0;
void enemy_dir(){
    if(enemy.x<(x_player_way[track_index]*40)){
        enemy.dirx=1;}
    else if (enemy.x==(x_player_way[track_index]*40)){
        enemy.dirx=0;}
    else if (enemy.x>(x_player_way[track_index]*40)){
        enemy.dirx=-1;}
    if(enemy.y<(y_player_way[track_index]*40)){
        enemy.diry=1;}
    else if (enemy.y==(y_player_way[track_index]*40)){
        enemy.diry=0;}
    else if (enemy.y>(y_player_way[track_index]*40)){
        enemy.diry=-1;}
    if((enemy.x==(x_player_way[track_index]*40) && enemy.y==(y_player_way[track_index]*40))){
        track_index++;
    }
}

void GameInit() {
	//initialize the way of player.
	initialize_way(); 
	
	initgraph(Width,Height); 
	loadResource();
	//bgm
	//mciSendString("open BGM.mp3",0,0,0);
	//mciSendString("play BGM.mp3",0,0,0);
	loadimage(&player.img,"6.jpg",40,40);
	player.x=20*40;
	player.y=14*40;
	player.dirx=0;
	player.diry=0;
	player.t1=GetTickCount();
	player.dt=5;

	//initialize the enemy				
	loadimage(&enemy.img,"enemy.png",40,40);
	enemy.x=18*40;
	enemy.y=15*40;
	enemy.dirx=0; 
	enemy.diry=0;
	enemy.t1=GetTickCount();
	enemy.dt=5;
	
	t=GetTickCount();
	menu(1);
}

void GameDraw() {
	BeginBatchDraw();
	drawMap();
	putimage(player.x,player.y,&player.img);
	putimage(enemy.x,enemy.y,&enemy.img);
	print_score();
	EndBatchDraw();
}

int collision(int x1,int x2,int x3,int x4,int y1,int y2,int y3,int y4)
{
	/*get two rectangles' coordinates
	collide ? return 1,or return 0;
	*/
	int cond = ((x1 >= x3 && x1 <= x4) && (y1 >= y3 && y1 <= y4)) ||
			   ((x2 >= x3 && x2 <= x4) && (y2 >= y3 && y2 <= y4)) ||
			   ((x1 + 40 >= x3 && x1 + 40 <= x4) && (y1 >= y3 && y1 <= y4)) ||
			   ((x1 >= x3 && x1 <= x4) && (y1 + 40 >= y3 && y1 + 40 <= y4));
	if (cond) return 1;
	return 0;
}

void GameUpdate() {
	//get key message 
	char key;
	if(_kbhit()){
		key=_getch();
		x_1=player.x, x_2=x_1+40, y_1=player.y, y_2=y_1+40;
		nextx1=x_1/40, nexty1=y_1/40, nextx2=x_2/40, nexty2=y_2/40;
		switch(key) {
			case 72: {
				player.dirx=0,player.diry=-1;
				break;}
			case 80: {
				player.dirx=0,player.diry=1;
				break;}
			case 75: {
				player.dirx=-1,player.diry=0;
				break;}
			case 77: {
				player.dirx=1,player.diry=0;
				break;}
		}
	}

	x_1=player.x, x_2=x_1+40, y_1=player.y, y_2=y_1+40;
	nextx1=x_1/40, nexty1=y_1/40, nextx2=x_2/40, nexty2=y_2/40;
	//
	switch (player.dirx) {
		case 1: {//
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

		case -1 : {//
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
		case 1 : { //
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


		case -1 : { //
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

	//
	player.t2=GetTickCount();
	if(player.t2-player.t1>=player.dt) {
		player.x+=2*player.dirx;
		player.y+=2*player.diry;
		player.t1=player.t2;
	}
	//
	enemy_way();
	enemy_dir();
	enemy.t2=GetTickCount();
	wait_time=GetTickCount();
	if(enemy.t2-enemy.t1>=enemy.dt && wait_time-t>=3000) {
		enemy.x+=1*enemy.dirx;
		enemy.y+=1*enemy.diry;
		enemy.t1=enemy.t2;
	}
	//detect whether a collision occurs,1 means a collision occurs.
	int detect_col=collision(player.x,player.x+40,enemy.x,enemy.x+40,player.y,player.y+40,enemy.y,enemy.y+40);
	//detect whether player reaches the destination
	int detect_des=((player.x==680) && (player.y==680));
	if(detect_col || detect_des){
		signal=0;
		menu(0);
	}
}


int main() {
	GameInit();
	while(signal) {
		GameDraw();
		GameUpdate();
	}
	return 0;
}
