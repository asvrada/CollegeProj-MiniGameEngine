#ifndef RASTERIZATIONCPP_DECLARATIONS_H
#define RASTERIZATIONCPP_DECLARATIONS_H

/********************************
整个项目规则定义：
坐标系：左手坐标系

世界坐标定义：
x轴：向右
y轴：向上
z轴：向屏幕里

旋转坐标定义：向主轴相反方向望去的顺时针方向为 正
x : Pitch
y : Yaw
z : Roll

物体：
三角形顶点缠绕方向：顺时针 为正面向视点

齐次剪裁空间：
范围 X,Y,Z ~ [-1, 1]
**********************/
/////////
//宏定义//
/////////

//求绝对值（Absolute）
#define ABS(x) (((x) >= 0)?(x):-(x))
//错误信息
//#define ERROR 0
#define OK 1

#endif