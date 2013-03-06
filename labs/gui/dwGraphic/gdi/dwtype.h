// 颜色过滤操作
#define FILTER_NONE       (uint8_t)0x00   // 无过滤
#define FILTER_ALPHA      (uint8_t)0x01   // Alpha过滤
#define FILTER_KEYCOLOR   (uint8_t)0x02   // 关键色过滤
#define FILTER_MULTIPLY   (uint8_t)0x03   // 各通道乘法

typedef union tagHGOBJ
{
	DWORD handle;			
	VOID* lpvoid;
	tagHGOBJ()
	{
		handle = NULL;
		lpvoid = NULL;
	}
}HGTEXTURE,HGCANVAS;



typedef struct tagFILTERMODE
{
	uint8_t blue;
	uint8_t green;             
	uint8_t red;               
	uint8_t filter;   // 过滤模式

	tagFILTERMODE()
	{
		blue = 0;
		red  = 0;
		green = 0;
		filter = FILTER_NONE;
	}
}FILTERMODE, * LPFILTERMODE;


// 绘制模式
#define DRAWMODE_STRETCH	0x00000000
#define DRAWMODE_9GRID		0x00000001
#define DRAWMODE_TILE		0x00000002
#define DRAWMODE_CENTER		0x00000003
#define DRAWMODE_9GRIDFILL	0x00000004

typedef struct tagDRAWMODE
{
	int	dwMode;			// 绘制模式
	int		dwParam1;		// 参数1，用于九宫的Left
	int		dwParam2;		// 参数2，用于九宫的Top
	int		dwParam3;		// 参数3，用于九宫的Right
	int		dwParam4;		// 参数4，用于九宫的Bottom

	tagDRAWMODE()
	{
		dwMode = DRAWMODE_STRETCH;
		dwParam1 = 0;
		dwParam2 = 0;
		dwParam3 = 0;
		dwParam4 = 0;
	}
}DRAWMODE, * LPDRAWMODE;





