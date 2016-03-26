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
范围 X,Y,Z ~ [-1, 1]
**********************/


#ifndef RASTERIZER_PROJECTHEADER_H
#define RASTERIZER_PROJECTHEADER_H

//#define DEBUG

/////////////////////
// System Header //
/////////////////////
#include "resource.h"
#include <Windows.h>
#include <assert.h>

//////////////////////////////
// STL and other Libraries //
/////////////////////////////
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
#include "MathComplement.h"


/////////////
// Macros //
/////////////

//角度转换
#define DEGREE(x) (0.01745f*(x))

//求绝对值（Absolute）
#define ABS(x) (((x) >= 0)?(x):-(x))

//#define ERROR 0
#define OK 1

//Default Color
#define COLOR_BLACK (RGB(0, 0, 0))
#define COLOR_WHITE (RGB(255, 255, 255))


//////////
// Flag //
/////////

//x
#define COORD_LEFT_HAND 0x00000000L
#define COORD_RIGHT_HAND 0x00000001L
#define COORD_MASK 0x00000001L

//x0
#define CULL_ANTICLOCKWISE 0x000000000L
#define CULL_CLOCKWISE 0x00000002L
#define CULL_MASK 0x00000002L

//x00
#define RENDER_MODE_OUTLINE 0x00000000L
#define RENDER_MODE_FILL 0x00000004L
#define RENDER_MODE_MASK 0x00000004L
#endif