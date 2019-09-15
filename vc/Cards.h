#pragma  once
#if !defined(CARDS_API)
	#define CARDS_API extern "C" __declspec(dllimport)
#endif

/*
initializes the cards.dll library. 
	pWidth - �����˿˵Ŀ��
	pHeight - �����˿˵ĸ߶�
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
	hDC - �豸��� 
	x - ���Ͻǵ�λ�õĺ�����
	y - ���Ͻǵ�λ�õ������� 
	uID - ָ��������һ���˿ˣ�����һ������ 
	uMode - ���Ʒ�ʽ
	crBk - ���Ʊ���ʱ�ı�����ɫ
The return value is TRUE if the draw was successful, and FALSE otherwise.
*/
CARDS_API BOOL WINAPI cdtDraw( HDC hDC, int x, int y, UINT uID, UINT uMode, COLORREF crBk );

/*
stretch draws a card
Parameters:
	hDC - �豸��� 
	x - ���Ͻǵ�λ�õĺ�����
	y - ���Ͻǵ�λ�õ������� 
	dx - ���
	dy - �߶�
	uID - ָ��������һ���˿ˣ�����һ������ 
	uMode - ���Ʒ�ʽ
	crBk - ���Ʊ���ʱ�ı�����ɫ
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
	mdFace,				// ���Ƹ��ֻ�ɫ�˿�
	mdBackground,		// ���Ƹ��ֱ���
	mdHilite,			// ��ɫ���Ƹ��ֻ�ɫ�˿ˣ���֧�ַ�ɫ����
	mdGhost,			// ��������ͼ
	mdRemove,			// ���ñ�����ɫ����һ���˿˴�С�ľ���
	mdInvisibleGhost,	// ��������ͼ
	mdDeckX,			// ���� X ͼ
	mdDeckO				// ���� O ͼ
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
