//微信打飞机.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "微信打飞机.h"
#include <list>

#define MAX_LOADSTRING	   100


// 全局变量: 
HINSTANCE hInst;												// 当前实例
WCHAR szTitle[MAX_LOADSTRING] = L"微信飞机大战";                 // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING] = L"微信飞机大战";			// 主窗口类名
UINT		g_uiBmp[] = {	IDB_BACKGROUND,
							IDB_BIGPALNE,
							IDB_EXITBTN,
							IDB_GAMELOGO,
							IDB_MIDDLEPALNE,
							IDB_PLAYBTN,
							IDB_RESTARTBTN,
							IDB_SCORE,
							IDB_SMALLPLANE,
							IDB_STARTBTN
							};				//10个UIbmp资源

HBITMAP				g_hBmp[BMPCOUNT];			//Bmp句柄
std::list<CPlane*>	PlaneList;					//飞机链表
GAME			g_Game;						//游戏信息

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_  HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAME));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_GAME));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(
	   szWindowClass,
	   szTitle,
	   WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX) & (~WS_SIZEBOX),
	   CW_USEDEFAULT,
	   CW_USEDEFAULT,
	   GAMEWNDWIDTH,
	   GAMEWNDHEIGHT + 55,		//55为边框+菜单厚度
	   nullptr,
	   nullptr,
	   hInstance,
	   nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:		//窗口创建
		{
			InitGame(lParam);
		}
    case WM_COMMAND:	//菜单信息
        {
			//获取菜单句柄
			HMENU	hMenu = GetMenu(hWnd);
		
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:		//关于框
				{
					KillTimer(hWnd, GAMETIMER);
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					SetTimer(hWnd, GAMETIMER, 50, NULL);
				}
                break;
				//难度设置	
			case ID_CHU:		
				{
					CheckMenuItem(hMenu, ID_CHU, MF_CHECKED);
					CheckMenuItem(hMenu, ID_ZHONG, MF_UNCHECKED);
					CheckMenuItem(hMenu, ID_GAO, MF_UNCHECKED);
					g_Game.planenum = 10;
					g_Game.Bigplanenum = 2;
				}
					break;
			case ID_ZHONG:
				{
					CheckMenuItem(hMenu, ID_CHU, MF_UNCHECKED);
					CheckMenuItem(hMenu, ID_ZHONG, MF_CHECKED);
					CheckMenuItem(hMenu, ID_GAO, MF_UNCHECKED);
					g_Game.planenum = 15;
					g_Game.Bigplanenum = 3;
				}
					break;
			case ID_GAO:
				{
					CheckMenuItem(hMenu, ID_CHU, MF_UNCHECKED);
					CheckMenuItem(hMenu, ID_ZHONG, MF_UNCHECKED);
					CheckMenuItem(hMenu, ID_GAO, MF_CHECKED);
					g_Game.planenum = 20;
					g_Game.Bigplanenum = 4;
				}
					break;

			case IDM_SHUOMING:	//游戏说明
				{
					//因为功能比较简单，所以直接用关于框的处理函数
					KillTimer(hWnd, GAMETIMER);
					DialogBox(hInst, MAKEINTRESOURCE(IDD_SHUOMING), hWnd, About);
					SetTimer(hWnd, GAMETIMER, 50, NULL);
				}
				break;


            case IDM_EXIT:		//退出
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_TIMER:		//时间消息
		{
			TimerProc(hWnd);
		}
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
			DrawWindow(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
	case MYGAMESTART:	//游戏开始
		{
			//初始化GAME信息
			g_Game.status = GAMESATART;
			g_Game.score = 0;
			SetTimer(hWnd, GAMETIMER, 50, NULL);

			CPlane::uBigNum = g_Game.Bigplanenum;		//最多大飞机数量
			CPlane::uBigNumHave = 0;					//现有大飞机数量
			
			//初始化飞机
			for (UINT i = 0; i < g_Game.planenum; i++)
			{
				Sleep(100);		//少了这一行会出现第一个飞机打不死的bug，具体原因未知
				PlaneList.push_back(new CPlane());
			}

		}
		break;
	case WM_LBUTTONDOWN:	//鼠标点击消息
		{
			LButtonDownProc(hWnd, lParam);
		}
		break;
    case WM_DESTROY:
		DestroyGame();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


//************************************
// 函数名称:    InitGame
// 函数全称:    InitGame
// 返回类型:    VOID
// 权限:   	   public 
// Qualifier: 
// 参数: 	   LPARAM lParam
// 说明:		初始化游戏各项数据
//************************************
VOID InitGame(LPARAM lParam)
{

	//导入UI资源
	for (int i=0;i<BMPCOUNT;i++)
	{
		g_hBmp[i] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(g_uiBmp[i]));
	}

	//初始化全局变量
	ZeroMemory(&g_Game, sizeof(g_Game));
	g_Game.status = GAMEHELLO;
	g_Game.score = 0;
	g_Game.planenum = 10;
	g_Game.play = TRUE;

	PlaneList.clear();
}



//************************************
// 函数名称:    DestroyGame
// 函数全称:    DestroyGame
// 返回类型:    VOID
// 权限:   	   public 
// Qualifier: 
// 说明:		释放资源
//************************************
VOID DestroyGame()
{
	for (int i = 0; i < BMPCOUNT; i++)
	{
		DeleteObject(g_hBmp[i]);
	}
}


//************************************
// 函数名称:    DrawWindow
// 函数全称:    DrawWindow
// 返回类型:    VOID
// 权限:   	   public 
// Qualifier: 
// 参数: 	   HDC hdc
// 说明:		绘制窗口
//************************************
VOID DrawWindow(HDC hdc)
{
	HDC			hdcMem, hdcTemp;
	HBITMAP		hBmpMem;
	BITMAP		bmp;
	LOGFONT		lf;
	HFONT		hf;
	RECT		rt;					//绘制字体的区域
	TCHAR		strScore[10];		//游戏分数字符串

	//双缓冲绘制
	hBmpMem = CreateCompatibleBitmap(hdc, GAMEWNDWIDTH, GAMEWNDHEIGHT);
	hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, hBmpMem);

	hdcTemp = CreateCompatibleDC(hdc);

	//绘制背景

	SelectObject(hdcTemp, g_hBmp[BACKGROUND]);
	BitBlt(hdcMem, 0, 0, GAMEWNDWIDTH, GAMEWNDHEIGHT, hdcTemp, 0, 0, SRCCOPY);

	//判断游戏状态
	switch (g_Game.status)
	{
	case GAMEHELLO:
		{

			//绘制LOGO
			SelectObject(hdcTemp, g_hBmp[GAMELOGO]);
			GetObject(g_hBmp[GAMELOGO], sizeof(bmp), &bmp);
			TransparentBlt(hdcMem, 30, 150, bmp.bmWidth, bmp.bmHeight, hdcTemp, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));

			//绘制开始按钮
			SelectObject(hdcTemp, g_hBmp[STARTBTN]);
			GetObject(g_hBmp[STARTBTN], sizeof(bmp), &bmp);
			TransparentBlt(hdcMem, 120, 350, bmp.bmWidth, bmp.bmHeight, hdcTemp, 0, 0, bmp.bmWidth, bmp.bmHeight, RGB(255, 255, 255));
		}
		break;

	case GAMESATART:
		{
			
			//遍历PlaneList绘制飞机
			std::list<CPlane*>::iterator	Ilist;
			for (Ilist = PlaneList.begin(); Ilist != PlaneList.end(); Ilist++)
			{
				(*Ilist)->Draw(hdcMem, hdcTemp);
			}

			//绘制开始暂停按钮		同时后绘制保持在最上面
			SelectObject(hdcTemp, g_hBmp[PLAYBTN]);
			GetObject(g_hBmp[PLAYBTN], sizeof(bmp), &bmp);
			TransparentBlt(hdcMem, 0, 0, bmp.bmWidth, bmp.bmHeight / 2, hdcTemp, 0, (!g_Game.play)*bmp.bmHeight / 2, bmp.bmWidth, bmp.bmHeight / 2, RGB(255, 255, 255));

		}
		break;

	case GAMEOVER:
		{
			//绘制分数牌
			SelectObject(hdcTemp,g_hBmp[SCORE]);
			GetObject(g_hBmp[SCORE], sizeof(bmp), &bmp);
			TransparentBlt(hdcMem, (GAMEWNDWIDTH - bmp.bmWidth) / 2, (GAMEWNDHEIGHT - bmp.bmHeight) / 2, bmp.bmWidth, bmp.bmHeight,
							hdcTemp, 0, 0, bmp.bmWidth, bmp.bmHeight, 
							RGB(255, 255, 255));

			//绘制重新开始按钮
			SelectObject(hdcTemp, g_hBmp[RESTARTBTN]);
			GetObject(g_hBmp[RESTARTBTN], sizeof(bmp), &bmp);
			TransparentBlt(hdcMem, 100, 270, bmp.bmWidth, bmp.bmHeight,
							hdcTemp, 0, 0, bmp.bmWidth, bmp.bmHeight,
							RGB(255, 255, 255));

			//绘制退出按钮
			SelectObject(hdcTemp, g_hBmp[EXITBTN]);
			GetObject(g_hBmp[EXITBTN], sizeof(bmp), &bmp);
			TransparentBlt(hdcMem, 100, 310, bmp.bmWidth, bmp.bmHeight,
							hdcTemp, 0, 0, bmp.bmWidth, bmp.bmHeight,
							RGB(255, 255, 255));
			//设置字体绘制分数
			lf.lfHeight = 25;
			lf.lfWidth = 25;
			lf.lfEscapement = 0;
			lf.lfItalic = FALSE;
			lf.lfUnderline = FALSE;
			lf.lfStrikeOut = FALSE;
			lf.lfCharSet = CHINESEBIG5_CHARSET;

			hf = CreateFontIndirect(&lf);
			SelectObject(hdcMem, hf);

			/*字体区域*/
			rt.left = 150;
			rt.top = 210;
			rt.right = 300;
			rt.bottom = 260;

			_itot_s(g_Game.score, strScore, 10);
			SetBkMode(hdcMem, TRANSPARENT);
			DrawText(hdcMem, strScore, -1, &rt, DT_CENTER);

		}
		break;

	default:
		break;
	}

	BitBlt(hdc, 0, 0, GAMEWNDWIDTH, GAMEWNDHEIGHT, hdcMem, 0, 0, SRCCOPY);

	DeleteObject(hBmpMem);
	DeleteDC(hdcMem);
	DeleteDC(hdcTemp);

}

//************************************
// 函数名称:    UpdatePlane
// 函数全称:    UpdatePlane
// 返回类型:    VOID
// 权限:   	   public 
// Qualifier: 
// 参数: 	   POINT pos
// 说明:			更新飞信信息，将鼠标消息下发至每个飞机对象
//************************************
VOID UpdatePlane(POINT pos)
{
	std::list<CPlane*>::iterator	Ilist;

	for (Ilist = PlaneList.begin(); Ilist != PlaneList.end(); Ilist++)
	{
		if ((*Ilist)->BeHit(pos))
		{
			break;	//保证只有一个飞机被点击到
		}
	}
}




VOID TimerProc(HWND hWnd)
{
	std::list<CPlane*>::iterator Ilist;

	for (Ilist = PlaneList.begin(); Ilist != PlaneList.end(); Ilist++)
	{
		//向下移动飞机
		if ((*Ilist)->Move())
		{
			//如果有飞机到达终点
			KillTimer(hWnd, GAMETIMER);
			g_Game.status = GAMEOVER;
			//释放飞机内存
			for (Ilist = PlaneList.begin(); Ilist != PlaneList.end(); Ilist++)
			{
				delete *Ilist;
			}
			PlaneList.clear();
			break;
		}

		if ((*Ilist)->IsCrashed())				//飞机被击落
		{
			if ((*Ilist)->IsCrashShowed())		//保证爆炸效果显示
			{
				g_Game.score += (*Ilist)->Getpoint();		//分数增加	

				//删除飞机对象，并重新创建
				delete	*Ilist;
				*Ilist = new CPlane();
			}
		}
	}
	//重绘窗口
	InvalidateRect(hWnd, NULL, FALSE);
}

//************************************
// 函数名称:    LButtonDownProc
// 函数全称:    LButtonDownProc
// 返回类型:    VOID
// 权限:   	   public 
// Qualifier: 
// 参数: 	   HWND hWnd
// 参数: 	   LPARAM lParam
// 说明:			鼠标点击消息处理
//************************************
VOID LButtonDownProc(HWND hWnd, LPARAM lParam)
{
	POINT        ptMouse;

	ptMouse.x = LOWORD(lParam);
	ptMouse.y = HIWORD(lParam);
	
	switch (g_Game.status)
	{
	case GAMEHELLO:
		if (CheckStartButtonDown(ptMouse))
		{
			SendMessage(hWnd, MYGAMESTART, 0, 0);
		}
		break;
	case GAMESATART:
		if (!CheckPlayButtonDown(hWnd,ptMouse))
		{
			UpdatePlane(ptMouse);
		}
		break;
	case GAMEOVER:
		{
			if (CheckReStartButtonDown(ptMouse))
			{
				SendMessage(hWnd, MYGAMESTART, 0, 0);
			}
			if (CheckExitButtonDown(ptMouse))
			{
				SendMessage(hWnd, WM_CLOSE, 0, 0);
			}
		}
		break;
	default:
		break;
	}
}


/************************************************************************/
/*检查按钮按下                                                           */
/************************************************************************/
BOOL CheckStartButtonDown(POINT ptMouse)
{
	RECT   rc;
	BITMAP bmp;

	GetObject(g_hBmp[GAMESATART], sizeof(bmp), &bmp);

	rc.left = 120;
	rc.top = 350;
	rc.right = bmp.bmWidth + rc.left;
	rc.bottom = bmp.bmHeight + rc.top;

	return PtInRect(&rc, ptMouse);
}

//************************************
// 函数名称:    CheckReStartButtonDown
// 函数全称:    CheckReStartButtonDown
// 返回类型:    BOOL
// 权限:   	   public 
// Qualifier: 
// 参数: 	   POINT ptMouse
// 说明:			检测重新开始按钮点击
//************************************
BOOL CheckReStartButtonDown(POINT ptMouse)
{
	RECT   rc;
	BITMAP bmp;

	GetObject(g_hBmp[RESTARTBTN], sizeof(bmp), &bmp);

	rc.left = 100;
	rc.top = 270;
	rc.right = bmp.bmWidth + rc.left;
	rc.bottom = bmp.bmHeight + rc.top;

	return PtInRect(&rc, ptMouse);
}

//************************************
// 函数名称:    CheckExitButtonDown
// 函数全称:    CheckExitButtonDown
// 返回类型:    BOOL
// 权限:   	   public 
// Qualifier: 
// 参数: 	   POINT ptMouse
// 说明:			检测退出按钮点击
//************************************
BOOL CheckExitButtonDown(POINT ptMouse)
{
	RECT   rc;
	BITMAP bmp;

	GetObject(g_hBmp[EXITBTN], sizeof(bmp), &bmp);

	rc.left = 100;
	rc.top = 310;
	rc.right = bmp.bmWidth + rc.left;
	rc.bottom = bmp.bmHeight + rc.top;

	return PtInRect(&rc, ptMouse);
}

//************************************
// 函数名称:    CheckPlayButtonDown
// 函数全称:    CheckPlayButtonDown
// 返回类型:    BOOL
// 权限:   	   public 
// Qualifier: 
// 参数: 	   HWND hWnd
// 参数: 	   POINT ptMouse
// 说明:			检测是否点击到开始/暂停按钮，并开始/暂停游戏
//************************************
BOOL CheckPlayButtonDown(HWND hWnd, POINT ptMouse)
{
	RECT   rc;
	BITMAP bmp;

	GetObject(g_hBmp[PLAYBTN], sizeof(bmp), &bmp);

	rc.left = 0;
	rc.top = 0;
	rc.right = bmp.bmWidth + rc.left;
	rc.bottom = bmp.bmHeight/2 + rc.top;

	if (PtInRect(&rc, ptMouse))
	{
		if (g_Game.play)
		{
			g_Game.play = FALSE;
			KillTimer(hWnd, GAMETIMER);
		}
		else 
		{
			g_Game.play = TRUE;
			SetTimer(hWnd, GAMETIMER, 50, NULL);
		}
		InvalidateRect(hWnd, NULL, TRUE);
		return TRUE;
	}
	return FALSE;
}
