#ifndef RASTERIZER_RENDERCLASS_H
#define RASTERIZER_RENDERCLASS_H
/*
渲染类
进行数学计算并将生成的图案输出至Canvas

包含类:
画布类
TODO 摄像机类
*/

////////////
// 包含类 //
////////////

#include "Declarations.h"
#include "MathCollection.h"
#include "CanvasClass.h"

class RenderClass
{
private:
	//画布类
	CanvasClass *m_Canvas;

public:
	RenderClass();
	~RenderClass();

	void Initialize(RECT,HDC);
	void Shutdown();

	//更改画布类的设置
	void UpdateCanvasSettings(RECT newRect);

	//渲染一帧
	void RenderAFrame();
};

#endif