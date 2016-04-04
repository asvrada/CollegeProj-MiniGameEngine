/*
Just Include this single file in every other header
and everything will be just fine !
*/

/************************
项目通用规则定义：
坐标系：左手坐标系

世界坐标定义：
x轴：向右
y轴：向上
z轴：向屏幕里

旋转坐标定义：向主轴相反方向望去的顺时针方向为 正
x : Pitch
y : Yaw
z : Roll

物体基本元素：
三角形顶点缠绕方向：顺时针 为正面朝向

齐次剪裁空间：
范围 
X,Y ~ [-1, 1]
Z ~ [0, 1]
**********************/


#ifndef RASTERIZER_PROJECTHEADER_H
#define RASTERIZER_PROJECTHEADER_H

//#define DEBUG
#define AFFLINE

/////////////////////
// System Header //
/////////////////////
#include "resource.h"
#include <Windows.h>
#include <assert.h>

///////////////////////////////
// STL and other Libraries //
///////////////////////////////
#include <array>
using std::array;
#include <queue>
using std::queue;
#include <vector>
using std::vector;

#include <string>
using std::wstring;
#include <sstream>
using std::wstringstream;
using std::endl;

#include <ctime>
using std::clock;
#include <cmath>
#include <memory>
using std::memset;

///////////////////
// User Header //
//////////////////
#include "Utility.h"


/////////////
// Macros //
/////////////

//角度转换
#define DEGREE(x) (0.01745f*(x))

//#define ERROR 0
#define OK 1

//Default Color
#define COLOR_BLACK (RGB(0, 0, 0))
#define COLOR_BLUE (RGB(0, 0, 255))
#define COLOR_RED (RGB(255, 0, 0))
#define COLOR_WHITE (RGB(255, 255, 255))


//////////
// Flag //
/////////
#define FRAGMENT_MASK 0x00000001
#define FRAGMENT_GOOD 0x00000000
#define FRAGMENT_DELETED 0x00000001

//32位 int
//八位16进制数表示
//x
#define RENDER_MODE_OUTLINE 0x00000000
#define RENDER_MODE_FILL 0x00000001
#define RENDER_MODE_MASK 0x00000001

#endif