#pragma once

#include "resource.h"
#include <mmsystem.h>
#include <comdef.h>
#include "CPlane.h"

#pragma comment(lib,"msimg32.lib")
#pragma comment(lib,"winmm.lib")

#define GAMEWNDWIDTH       380						//窗口宽度
#define GAMEWNDHEIGHT      550						//窗口高度
#define BMPCOUNT			10						//UI数目

#define MYGAMESTART			(WM_USER+1)				//游戏开始消息
#define GAMETIMER			1						//游戏计时器


/*游戏状态*/
enum GAMESTATUS
{
	GAMEHELLO,		//游戏欢迎界面
	GAMESATART,		//游戏开始
	GAMEOVER		//游戏结束
};

/*游戏信息*/
struct GAME
{
	GAMESTATUS	status;			//游戏当前状态
	DWORD		score;			//游戏得分
	UINT		planenum;		//飞机数
	BOOL		play;			//游戏开始还是暂停
	UINT		Bigplanenum;	//大飞机数量（难度设置）
};


/*函数声明*/

VOID		InitGame(LPARAM lParam);		//初始化游戏	
VOID		DestroyGame();					//游戏摧毁
VOID        DrawWindow(HDC hdc);			//桌面绘制
VOID		UpdatePlane(POINT pos);	//飞机信息更新
VOID		TimerProc(HWND hWnd);			//处理时间信息
VOID		LButtonDownProc(HWND hWnd, LPARAM lParam);
BOOL		CheckStartButtonDown(POINT ptMouse);
BOOL		CheckReStartButtonDown(POINT ptMouse);
BOOL		CheckExitButtonDown(POINT ptMouse);
BOOL		CheckPlayButtonDown(HWND DhWnd, POINT ptMouse);