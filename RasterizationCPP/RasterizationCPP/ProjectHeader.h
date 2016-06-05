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
#include <map>
using std::map;

#include <fstream>
using std::ifstream;
#include <string>
using std::wstring;
using std::string;
#include <sstream>
using std::wstringstream;
using std::endl;

#include <ctime>
using std::clock;
#include <cmath>
#include <memory>
using std::memset;
using std::make_shared;
using std::shared_ptr;

///////////////////
// User Header //
//////////////////
#include "Utility.h"


/////////////
// Macros //
/////////////

//按键映射
#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39

#define VK_A 0x41
#define VK_B 0x42
#define VK_C 0x43
#define VK_D 0x44
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48
#define VK_I 0x49
#define VK_J 0x4A
#define VK_K 0x4B
#define VK_L 0x4C
#define VK_M 0x4D
#define VK_N 0x4E
#define VK_O 0x4F
#define VK_P 0x50
#define VK_Q 0x51
#define VK_R 0x52
#define VK_S 0x53
#define VK_T 0x54
#define VK_U 0x55
#define VK_V 0x56
#define VK_W 0x57
#define VK_X 0x58
#define VK_Y 0x59
#define VK_Z 0x5A

//角度转换
#define DEGREE(x) (0.01745f*(x))

//动态多态
#define PTR_CONVERT(shared_ptr, type) (dynamic_cast<type*>(shared_ptr.get()))

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