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

/////////////////////
// System Header //
/////////////////////
#include "resource.h"
#include <Windows.h>

//////////////////////////////
// STL and other Libraries //
/////////////////////////////
#include <queue>
using std::queue;
#include <vector>
using std::vector;

#include <string>
using std::wstring;
#include <sstream>
using std::wstringstream;

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
#endif