#pragma  once
#if !defined(CARDS_API)
	#define CARDS_API extern "C" __declspec(dllimport)
#endif

/*
initializes the cards.dll library. 
	pWidth - 返回扑克的宽度
	pHeight - 返回扑克的高度
As you can see the method takes two arguments which are stored as the width and height of the card in pixels. 
The return value is either TRUE for when the cards.dll has been initialized, or FALSE if an error has occurred. 
*/
CARDS_API BOOL WINAPI cdtInit( int* pWidth, int* pHeight );

/*
cleans up the card resources
*/
CARDS_API void WINAPI cdtTerm();

/*
draws a card
Parameters:
	hDC - 设备句柄 
	x - 左上角的位置的横坐标
	y - 左上角的位置的纵坐标 
	uID - 指出绘制哪一张扑克，或哪一个背景 
	uMode - 绘制方式
	crBk - 绘制背景时的背景颜色
The return value is TRUE if the draw was successful, and FALSE otherwise.
*/
CARDS_API BOOL WINAPI cdtDraw( HDC hDC, int x, int y, UINT uID, UINT uMode, COLORREF crBk );

/*
stretch draws a card
Parameters:
	hDC - 设备句柄 
	x - 左上角的位置的横坐标
	y - 左上角的位置的纵坐标 
	dx - 宽度
	dy - 高度
	uID - 指出绘制哪一张扑克，或哪一个背景 
	uMode - 绘制方式
	crBk - 绘制背景时的背景颜色
The return value is TRUE if the draw was successful, and FALSE otherwise.
*/
CARDS_API BOOL WINAPI cdtDrawExt( HDC hDC, int x, int y, int dx, int dy, UINT uID, UINT uMode, COLORREF crBk );

/*
animates the backs of certain cards. In windows 2000 and previous the following cards animate. 
	ROBOT - Meter moves over 4 frames 
	CASTLE - Bats flapping around castle over 2 frames 
	ISLAND - Sun sticks tongue out over 4 frames 
	CARDHAND - Cards running up and down sleeve over 4 frames 
Call cdtAnimate every 250 ms for proper animation speed. The return value is TRUE if the animation was successful, and FALSE when the animation has finished.
*/
CARDS_API BOOL WINAPI cdtAnimate(HDC hDC, UINT uID, int x, int y, int frame);

enum
{
	mdFace,				// 绘制各种花色扑克
	mdBackground,		// 绘制各种背景
	mdHilite,			// 反色绘制各种花色扑克，不支持反色背景
	mdGhost,			// 绘制网格图
	mdRemove,			// 采用背景颜色绘制一个扑克大小的矩形
	mdInvisibleGhost,	// 绘制网格图
	mdDeckX,			// 绘制 X 图
	mdDeckO				// 绘制 O 图
};

enum
{
	CROSSHATCH = 53, /* XP = CROSSHATCH */ 
	WEAVE1     = 54, /* XP = SKY */
	WEAVE2     = 55, /* XP = MINERAL */
	ROBOT      = 56, /* XP = FISH */
	FLOWERS    = 57, /* XP = FROG */
	VINE1      = 58, /* XP = MOONFLOWER */
	VINE2      = 59, /* XP = ISLAND */
	FISH1      = 60, /* XP = SQUARES */
	FISH2      = 61, /* XP = MAGENTA */
	SHELLS     = 62, /* XP = SANDDUNES */
	CASTLE     = 63, /* XP = SPACE */
	ISLAND     = 64, /* XP = LINES */
	CARDHAND   = 65, /* XP = TOYCARS */
	UNUSED     = 66, /* XP = UNUSED */
	THE_X      = 67, /* XP = THE_X */
	THE_O      = 68  /* XP = THE_0 */
};
