///////////////////////////// YIMAENCHEAD.H ////////////////////////////////////
/***************** Release Version 3.0.2 ***********************/
/***************** Release Data 2006-07-04 *********************/ 
#ifndef __YIMAENC_HEAD_H__
  #define __YIMAENC_HEAD_H__
 
#include <fstream>
#include <QTextCodec>
#include "time.h"

using namespace std;
   
#define NULL 0

#define UNI_GEO_COOR_MULTI_FACTOR 10000000  
#define  PI 3.141592653589793
#define MAX_EMULATE_OTHER_VESSEL_COUNT 5000
 
#ifndef VCHAR
#ifdef _UNICODE
#define VCHAR WCHAR
#else
#define VCHAR CHAR
#endif
#endif

#ifndef UINT
typedef unsigned int UINT ;
#endif

#ifndef INT
typedef int INT ;
#endif
typedef int ID_INT;

#ifndef __int64
#define __int64 long long
#endif

typedef enum FIELD_COMPARE_OPTION
{
    EQUAL = 0,
    UNEQUAL = 1,
    GREATER = 2,//记录数据 大于 SEL_FIELD_CON 的 fieldVal
    GREATER_OR_EQUAL=3,
    SMALLER = 4,
    SMALLER_OR_EQUAL=5,
    CONTAIN = 6,  //这两种用于字符串类型, CONTAIN表示记录字段包含条件制定的子字符串
    CONTAINED = 7, //暂时用不上，因为要求记录字段是条件字符串的某个子字符串没有实际意义
    NOT_CONTAIN = 8
}FIELD_COMPARE_OPTION;


class DATA_STRUCT_CONVERT
{
    public:
    static bool QString2pChar(QString str,char* pChar)
    {
        /*
        QByteArray qBa = str.toLocal8Bit();
        pChar = qBa.data();
        return true;
        */
        return false;
    };

    static QString PChar2QString(char* pChar, char* code = (char*)"GBK")
    {
        QString str;
        QTextCodec *codec = QTextCodec::codecForName(code);//指定QString的编码方式
        str = codec->toUnicode(pChar);
        return str;
    };
};



typedef enum M_BASIC_DATA_TYPE
{
	DT_UINT = 0x01,
	DT_INT = 0x20,
	DT_BOOL = 0x02,
	DT_CHAR = 0x04,
	DT_FLOAT = 0x08,
	DT_DOUBLE = 0x40,
	DT_STRING = 0x10
}M_BASIC_DATA_TYPE;

typedef enum ERROR_TYPE
{	
	ERR_IN_PARAM_INVALID = 0,
//	ERR_MEMORY_
}ERROR_TYPE;

typedef struct M_ERROR_INFO
{
	ERROR_TYPE errorType;
	int errorParam;
	char errorMessage[255];
}M_ERROR_INFO;  
 

/*  航速单位:米/秒,节 */
typedef enum EMULATE_SPEED_UNIT
{
	KNOT_UNIT = 0,
	METRE_UNIT = 1
}EMULATE_SPEED_UNIT; 
 
///////////////////////////////////////////////////////////////////


/////////////////////////////////////basic things:color//////////////////////////////////////////
typedef struct M_COLOR
{
	unsigned char r;
	unsigned char g;
	unsigned char b;	
	unsigned char reserve; //保留,并且为了使M_COLOR是4字节对齐的

	M_COLOR()
	{
		r = g = b = reserve = 0;
	};

	M_COLOR(unsigned char R, unsigned char G, unsigned char B)
	{
		r = R;
		g = G;
		b = B;
		reserve = 0;
	};

	M_COLOR(unsigned char R, unsigned char G, unsigned char B, unsigned char rsv)
	{
		r = R;
		g = G;
		b = B;
		reserve = rsv;
	}; 
}M_COLOR;

typedef int INT_COLOR;

inline bool operator==(const M_COLOR left, const M_COLOR right)
{		
	return( left.r==right.r && left.g==right.g && left.b==right.b);		
};

inline bool operator!=(const M_COLOR left, const M_COLOR right)
{		
	return !( left==right);		
};
 
#define M_RGB(r,g,b) r+(g<<8)+(b<<16)
#define M_COLOR_TO_INT(color) *((int*)&color)
#define M_INT_TO_COLOR(iColor) *((M_COLOR*)&iColor) 
#define M_GET_COLOR(r,g,b) M_INT_TO_COLOR(int(M_RGB(r,g,b)))

inline void M_SET_COLOR_BY_INT( M_COLOR& color, INT_COLOR intColor)
{	
	color = M_INT_TO_COLOR(intColor); 
};

inline void M_SET_COLOR_BY_RGB( M_COLOR& color, int r, int g, int b)
{	
	color.r=r;
	color.g=g;
	color.b=b;
	color.reserve=0;            
};
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////M_POINT/////////////////////////////////////////////////
typedef struct M_POINT
{
	int x;
	int y;	

	M_POINT()
	{
		x = y = 0;
	};

	M_POINT(int poX, int poY)
	{
		x = poX;
		y = poY;
	};

	M_POINT& operator+=(const M_POINT& right)
	{
		x = x + right.x;
		y = y + right.y;

		return *this;
	};

	M_POINT& operator-=(const M_POINT& right)
	{
		x = x - right.x;
		y = y - right.y;

		return *this;
	};

	M_POINT& operator=(const M_POINT& right)
	{
		x = right.x;
		y = right.y;

		return *this;
	};
}M_POINT;

inline bool operator==(const M_POINT left, const M_POINT right)
{	
	return( left.x==right.x && left.y==right.y);		
};

inline M_POINT operator-(const M_POINT left, const M_POINT right)
{	
	M_POINT po;
	po.x = left.x - right.x;
	po.y = left.y - right.y;
	return po;		
};

inline M_POINT operator+(const M_POINT left, const M_POINT right)
{	
	M_POINT po;
	po.x = left.x + right.x;
	po.y = left.y + right.y;
	return po;		
}; 

typedef struct MRECT
{
	int leftTopPointX;
	int leftTopPointY;
	int rightDownPointX;
	int rightDownPointY;

	MRECT()
	{
		leftTopPointX = leftTopPointY = 
			rightDownPointX = rightDownPointY = 0 ;
	};

	MRECT(M_POINT leftTopPo, int width, int height)
	{
		int leftX = leftTopPo.x;
		int topY = leftTopPo.y;
		leftTopPointX = leftX;
		leftTopPointY = topY;
		rightDownPointX = leftX + width;
		rightDownPointY = topY + height;
	};

	MRECT(int leftX, int topY, int rightX, int downY)
	{
		leftTopPointX = leftX;
		leftTopPointY = topY;
		rightDownPointX = rightX;
		rightDownPointY = downY;
	};

	MRECT(M_POINT leftTopPo, M_POINT rightBottomPo)
	{
		leftTopPointX = leftTopPo.x;
		leftTopPointY = leftTopPo.y;
		rightDownPointX = rightBottomPo.x;
		rightDownPointY = rightBottomPo.y;
	};

	MRECT& operator=(const MRECT& right)
	{
		leftTopPointX = right.leftTopPointX;
		leftTopPointY = right.leftTopPointY;
		rightDownPointX = right.rightDownPointX;
		rightDownPointY = right.rightDownPointY;

		return *this;
	};
}MRECT;

////////////////////////// MEM_GEO_OBJ_POS ////////////////////////////////
typedef struct MEM_GEO_OBJ_POS //geo object position in memory-maps
{
	int memMapPos;
	int layerPos;
	int innerLayerPos;

	MEM_GEO_OBJ_POS()
	{
		memMapPos = layerPos = innerLayerPos = -1; 
	};

	MEM_GEO_OBJ_POS(int mapPos, int lyrPos, int objPos)
	{
		memMapPos = mapPos;
		layerPos = lyrPos;
		innerLayerPos = objPos;
	};
}MAP_GEO_OBJ_POS;

////////////////////////// M_GEO_OBJ_POS ////////////////////////////////
typedef struct M_GEO_OBJ_POS //geo object position in a specific map
{
	int layerPos;
	int innerLayerObjectPos;

	M_GEO_OBJ_POS()
	{
		layerPos = -1;
		innerLayerObjectPos = -1;
	};

	M_GEO_OBJ_POS(int lyrPos, int inLyrPos)
	{
		layerPos = lyrPos;
		innerLayerObjectPos = inLyrPos;
	};
}M_GEO_OBJ_POS;

inline bool operator==(const M_GEO_OBJ_POS left, const M_GEO_OBJ_POS right)
{		
	return (left.layerPos==right.layerPos
		&& left.innerLayerObjectPos==right.innerLayerObjectPos );		
};

inline bool operator!=(const M_GEO_OBJ_POS left, const M_GEO_OBJ_POS right)
{		
	return (left.layerPos != right.layerPos
		|| left.innerLayerObjectPos != right.innerLayerObjectPos );		
}; 
///////////////////////////////////////////////////////////////////////////////// 
#define ID_INVALID_VAL 0 
typedef struct M_GEO_OBJ_ID
{
	int layerID;
	int innerLayerObjectID;

	M_GEO_OBJ_ID()
	{
		layerID = ID_INVALID_VAL;
		innerLayerObjectID = ID_INVALID_VAL; 
	};

	M_GEO_OBJ_ID(int layerId, int innerLayerObjectId)
	{
		layerID = layerId;
		innerLayerObjectID = innerLayerObjectId;
	};

	bool IsValid()
	{
		return (layerID != ID_INVALID_VAL) && (innerLayerObjectID != ID_INVALID_VAL);
	};
}M_GEO_OBJ_ID;
inline bool operator==(const M_GEO_OBJ_ID left, const M_GEO_OBJ_ID right)
{
    return (left.layerID==right.layerID
        && left.innerLayerObjectID==right.innerLayerObjectID );
};

inline bool operator!=(const M_GEO_OBJ_ID left, const M_GEO_OBJ_ID right)
{
    return (left.layerID != right.layerID
        || left.innerLayerObjectID != right.innerLayerObjectID );
};

/////////////////////////////////////////////////////////////////////////////////

typedef enum SPECIAL_LINE_TYPE
{
    NO_SPECIAL_TYPE = 0,        //常规，适用线、面
    RECTANGLE_LN_TYPE = 10,     //矩形，适用线、面
    CIRCLE_LN_TYPE = 20,        //圆，适用线、面
    ELLIPSE_LN_TYPE = 30,       //椭圆，适用线、面
    ARC_LN_TYPE = 40,           //弧形，适用线、面（面：边框无半径的扇形）
    PIE_LN_TYPE = 41,           //扇形，适用线、面
    BOW_LN_TYPE = 42,           //弓形，适用线、面
    SECTOR_LN_TYPE = 50,        //扇区，适用线
    CURVE_LN_TYPE = 60,         //圆滑曲线，适用线
    CURVE_LN_TYPE_WITH_HEAD_ARROW = 61, //单箭头曲线
    CURVE_LN_TYPE_WITH_HAED_TAIL_ARROW = 62, //双箭头曲线
    SINGLE_ARROW_LN_TYPE = 80,  //单箭头（双行线）
}SPECIAL_LINE_TYPE;
/////////////////////////////////////////////////////////////////////////////////

typedef enum UNDO_OPERATION_TYPE
{ 
	ADD_GEO_OBJ = 3,
	DEL_GEO_OBJ = 4,
	MOD_GEO_OBJ = 5
}UNDO_OPERATION_TYPE;

/////////////////////////////////////////////////////////////////////////////////

typedef enum ENC_COLOR_GROUP
{
	DAY_BRIGHT = 1,
	DAY_WHITEBACK = 2,
	DAY_BLACKBACK = 3,
	DUSK = 4,
	NIGHT = 5
}ENC_COLOR_GROUP;

typedef enum LAYER_GEO_TYPE
{ 
	LAYER_GEO_TYPE_NULL = 0,
	ALL_POINT = 1,
	ALL_LINE = 2,
	ALL_FACE = 3,
	MULTIPLE_GEO_TYPE = 5
}LAYER_GEO_TYPE;

typedef enum M_GEO_TYPE //物标的几何属性
{	
	TYPE_NULL = -1,
	TYPE_POINT = 0,
	TYPE_LINE = 2,          
	TYPE_FACE = 3,          
	TYPE_COMBINED_OBJECT = 10
}M_GEO_TYPE;

typedef enum M_DATA_TYPE_ID //物标属性的基本数据类型
{
	M_NULL = 0,
	M_INT = 1,
	M_BOOL = 2,
	M_FLOAT = 3, 
	M_STRING = 4,  //with a maxinum length of 65536 byte, and a Chinese character is counted as 2 bytes 
	M_ENUM = 5,
	M_LIST = 6
}M_DATA_TYPE_ID; 

typedef enum MAP_NATIONAL_LANGUAGE
{
    LANG_ENG = 1,
    LANG_CHN = 2,
    LANG_JPN = 3,
    LANG_KOR = 4
}MAP_NATIONAL_LANGUAGE;

#define BOOL_BE_TRUE_STRING "y"
#define BOOL_BE_FALSE_STRING "n"

#define MAX_ATTR_VAL_STRING_LEN 65536
#define MAX_NAMES_LEN 255 
#define MAX_ROUTE_WAY_POINT_COUNT 1000

typedef enum MAP_FILE_TYPE
{
	S57BASE_FILE = 0,
	S57UPDATE_FILE = 1,
	YIMAP_FILE = 2
}MAP_FILE_TYPE; 

typedef UINT SHIP_ID;
#define INVALID_SHIP_ID -1

typedef UINT WAY_POINT_ID;
typedef UINT ROUTE_ID; 
typedef UINT LEG_LINE_ID;
typedef UINT LIB_MAP_ELEMENT_ID;
typedef UINT URGENT_POINT_ID;
typedef UINT TRACK_ID;

#define INVALID_ROUTE_ID -1
#define INVALID_ID -1
#define MAX_WAY_POINT_COUNT_OF_ROUTE 100000
#define MAX_LINE_EDIT_POINT_COUNT 100000
#define ROUTES_DATA_FILE "Routes//routes.dat"
#define SEL_OBJ_MAX_COUNT 3000

typedef struct M_TIME
{
	int Year;
	int Month;
	int Day;
	int Hour;
	int Minute;
	int Second;

    //M_TIME();
    //M_TIME(int year,int month,int day,int hour,int minute,int second);
    //M_TIME& operator=(const M_TIME& right);
}M_TIME;

//2015-08-26 added by zcz
typedef struct tag_TrackPoint
{
    M_POINT ptPoint;
    int iOffsetTime;
    float fSpeedOverGround;
    float fSpeedThrghWater;
    float fHeading;
    float fCourseOverGround;
    float fCourseThrghWater;
}TrackPoint;

inline void GetCurrentTimeString(char* strTime)
{
	struct tm *newtime;
	time_t ltime;
	time(&ltime);
	newtime = gmtime(&ltime);
	
	sprintf(strTime, "%d-%d-%d, %.2d:%.2d:%.2d", 1900 + newtime->tm_year, 1 + newtime->tm_mon, newtime->tm_mday,
	                 newtime->tm_hour, newtime->tm_min, newtime->tm_sec);
};

typedef enum DRAW_MAP_WITH_RADAR_MODEL
{
	DRAW_OBJ_OVER_RADAR = 1,
	DRAW_OBJ_UNDER_RADAR = 2,
	DRAW_ALL_OBJ = 3
}DRAW_MAP_WITH_RADAR_MODEL;

typedef enum DISPLAY_CATEGORY_NUM
{
	DISPLAYBASE = 0,
	STANDARD = 1,
	OTHER = 2,
	MARINERS_STANDARD = 3,
	MARINERS_OTHER = 4
}DISPLAY_CATEGORY_NUM;  

typedef enum NAV_MODE
{
	NAV_BY_NULL = 0,
	NAV_BY_ROUTE = 1,
	NAV_BY_WAYPOINT = 2,
	NAV_BY_URGENTPOINT = 3,
	NAV_BY_TRACK = 4	
}NAV_MODE;

typedef enum MAP_EFFECT
{
    EFFECT_NONE = 1,	 //无效果
    EFFECT_HOLLOW = 2,	 //空心效果
    EFFECT_DELAY = 3	 //延迟效果
}MAP_EFFECT;

typedef enum LoadMapName
{
    GoogleSatMap = 0,//谷歌卫星地图
    GoogleStrMap = 1,//谷歌街道地图

    TencentSatMap = 2,//腾讯卫星地图
    TencentStrMap = 3,//腾讯街道地图

    TianDiSatMap = 4,//天地图卫星地图
    TianDiStrMap = 5,//天地图街道地图

    //大于10为百度切割模式
    BaiduStaMap = 11, //百度卫星地图
    BaiduStrMap = 12,//百度街道地图

    //大于100的为使用经纬直投投影
    TianDiSatMapDirect = 101,//天地图卫星地图（经纬直投）
    TianDiStrMapDirect = 102,//天地图街道地图（经纬直投）
}LoadMapName;

typedef enum ProjectorType
{
    StandardMercat = 1, //标准墨卡托
    GoogleMercat = 2,   //谷歌墨卡托投影
    LonLatDirect = 3    //经纬投影
}ProjectorType;

typedef struct ShipSpeedLine
{
    int shipId;
    M_POINT startScrnPo;
    M_POINT endScrnPo;
    bool bOwnShip;
    ShipSpeedLine()
    {
        shipId = -1;
        startScrnPo = M_POINT(-100, -100);
        endScrnPo = M_POINT(-100, -100);
        bOwnShip = false;
    }
}ShipSpeedLine;
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////

inline float GetSpeedOfMsFromKnot(float speedByKnot)
{
	return speedByKnot * 1880 / 3600;
}

inline float GetSpeedOfKnotFromMs(float speedByMs)
{
	return speedByMs * 3600 / 1880;
} 


typedef enum M_TOOl_TYPE {
        TYPE_NONE = 0,
        TYPE_EBL = 1,
        TYPE_MEASURE_DIS = 2,
        TYPE_MEASURE_AREA = 3,
        TYPE_AREA_ZOOM_IN = 4
}M_TOOl_TYPE;

//操作状态
typedef enum TOperatorType
{
    NoOpertion = 1,
    AddUserObject = 2,
    DragMap = 17,
    EDITING_GEO_OBJECT = 61
}TOperatorType;

//危险物标显示样式
typedef enum ISODANGER_STYLE
{
    FORCE_DISPLAY_REDX = 1,//强制显示_红叉
    FORCE_DISPLAY_ORIGIN_SYMBOL = 2,//强制显示_原始符号
    NOT_FORCE_DISPLAY = 3,  //不强制显示 
}ISODANGER_STYLE;

//海图显示视点范围
typedef enum VIEW_POINT_SCOPE
{
    VPS_MID_WITHOUT_BNDRY = 0,
    VPS_CONTAIN_LEFT_BNDRY = 1,
    VPS_CONTAIN_RIGHT_BNDRY = 2
}VIEW_POINT_SCOPE;

//内置操作选项
typedef enum M_TOOL_OPTION
{
    MT_KryCtrl__SlowerScale = 0x01, //按住Ctrl键，（滚轮或双击的）缩放操作幅度变小,默认开启
    MT_DBLeft__InsertObjNode= 0x02, //鼠标左键双击（编辑状态的）物标边缘，新增节点，默认开启
    MT_DbLeft__ZoomIn = 0x04,       //鼠标左键双击，放大,默认关闭
    MT_DbRight__ZoomOut = 0x08,     //鼠标右键双击，缩小,默认关闭
    MT_Left__SelectShip = 0x10,     //鼠标左键抬起，选择船舶，默认关闭
    MT_ONLY_TOUCH_MODE = 0x20,      //仅触屏模式，将禁用一些操作，如：自由绘画的操作模式1（见更新文档）
    MT_FIGER_SCALE = 0x40,          //双指缩放手势，默认开启
}M_TOOL_OPTION;

//编辑修改状态
typedef enum CHANGE_TYPE
{
    CT_ADDED_NODE = 1,  //在nodePos后添加了一个节点
    CT_MOVE_NODE = 2,   //移动了一个节点
    CT_MOVE_WHOLE = 3,  //移动了整体
    CT_DELETE_NODE = 4  //删除了一个节点
}CHANGE_TYPE;

//海图文本类型
typedef enum ENC_TEXT_TYPE
{
    ETT_ImportantText = 1,  //重要文本：海域名等
    ETT_LightDescText = 2,  //灯标类描述文本
    ETT_GeographicName= 3,  //地名
    ETT_MagneticValue = 4,  //磁差值文本
    ETT_NatureOfSeabed= 5,  //海底地质
}ENC_TEXT_TYPE;

#endif
