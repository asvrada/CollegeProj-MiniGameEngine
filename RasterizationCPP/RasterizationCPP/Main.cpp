/*
最基本的光栅化渲染器代码
项目开始时间
2016/1/3

--Jeff
*/

#include "WindowFrameClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreinstance, LPSTR lpCmd, int nShowCmd) {
	WindowFrame *mainWindow = new WindowFrame(RENDER_MODE_OUTLINE);

	//初始化
	mainWindow->Initialize(800, 600);
	//注册窗口
	if (ERROR == mainWindow->RegisterCreateWindow(hInstance, hPreinstance, lpCmd, nShowCmd)) {
		return ERROR;
	}

	//进行主循环
	mainWindow->Run();

	//关闭
	mainWindow->Shutdown();

	//善后处理
	delete mainWindow;
	mainWindow = NULL;

	return OK;
}