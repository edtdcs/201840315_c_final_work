#include <stdio.h>
#include <stdlib.h>
#include <easyx.h>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include<math.h>
//瀹氫箟
#define Width 40*23
#define Height 800

struct Player {
	int x,y;//鍧愭爣
	IMAGE img;//鍥剧墖
	int dirx,diry;//鏂瑰悜
	unsigned long t1,t2,dt;//鏃堕棿
};//鐜╁
Player player;
Player enemy;
int x_1,x_2,y_1,y_2,nextx1,nextx2,nexty1,nexty2,score;
int t;
IMAGE img[4];
//1绾?2澧?3鐧?
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
//鍒濆鍖?

int x_player_way[1000]={1,2,5,4,2,6,4};
int y_player_way[1000]={1,5,4,8,9,3,4};

void initialize_way(){
    //call this function when we initialize the game
    x_player_way[0]=20;
    y_player_way[0]=14;
    for(int i=1;i<1000;i++){
        x_player_way[i]=0;
        y_player_way[i]=0;
    }
}
int index=0;
void enemy_way(){
    //call the function when we call GAMEUPDAGE().
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
	initialize_way(); 
	//绐楀彛
	initgraph(Width,Height); 
	//鑳屾櫙
	loadResource();
	//bgm
	//mciSendString("open BGM.mp3",0,0,0);
	//mciSendString("play BGM.mp3",0,0,0);
	//鐜╁
	loadimage(&player.img,"6.jpg",40,40);
	player.x=20*40;
	player.y=14*40;
	player.dirx=0;
	player.diry=0;
	player.t1=GetTickCount();
	player.dt=5;

	//鏁屼汉					
	loadimage(&enemy.img,"enemy.png",40,40);
	enemy.x=18*40;
	enemy.y=15*40;
	enemy.dirx=0; 
	enemy.diry=0;
	enemy.t1=GetTickCount();
	enemy.dt=5;
}
//鏄剧ず
void GameDraw() {
	BeginBatchDraw();
	//鑳屾櫙
	drawMap();
	//鐜╁
	putimage(player.x,player.y,&player.img);
	//鏁屼汉
	putimage(enemy.x,enemy.y,&enemy.img);
	EndBatchDraw();
}

//鏇存柊
void GameUpdate() {
	//鐜╁
	//鎵嬪姩绉诲姩
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
	//纰板
	 
	switch (player.dirx) {
		case 1: {//鍙? 浠ユ涓轰緥璇存槑纰版挒鍒ゅ畾锛岀鎾炲繀鐒跺彂鐢熷湪杈圭晫锛屾墍浠layer.x%40==0鏃舵墠闇€鍒ゆ柇鏄惁纰版挒锛屽綋浜虹墿姝ｅソ澶勪簬涓€涓柟鍧楁椂锛屽彧闇€鍒ゅ畾鍙宠竟涓€涓牸瀛愭槸鍚︿负澧欐垨閲戝竵锛?
				//褰撲汉鐗╁浜庝袱鏂瑰潡涔嬮棿鏃讹紝鍒ゅ畾鍙充晶涓ゆ柟鍧?,璁歌繘鏀夸簬12鏈?4鏃?7:49娉ㄣ€?
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

		case -1 : {//宸?
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
		case 1 : { //涓?
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


		case -1 : { //涓?
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

	//绉诲姩鏂瑰悜锛岄€熷害
	player.t2=GetTickCount();
	if(player.t2-player.t1>=player.dt) {
		player.x+=2*player.dirx;
		player.y+=2*player.diry;
		player.t1=player.t2;
	}
	//鏁屼汉
	//绉诲姩閫熷害
	enemy_way();
	enemy_dir();
	enemy.t2=GetTickCount();
	if(enemy.t2-enemy.t1>=enemy.dt) {
		enemy.x+=1*enemy.dirx;
		enemy.y+=1*enemy.diry;
		enemy.t1=enemy.t2;
	}
	//鏂瑰悜
	/*int dx=abs(player.x-enemy.x);
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
	}*/
	
	
	//纰板
	/*int ccx=enemy.x+14;
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
	}*/
	//鍚冩帀
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
