#include "RenderClass.h"



RenderClass::RenderClass() {
	m_Canvas = NULL;
	m_Camera = NULL;
}


RenderClass::~RenderClass() {
	delete m_Canvas;
	delete m_Camera;
}

void RenderClass::Initialize(RECT rectWindow, HDC hdcScreen) {
	m_Canvas = new CanvasClass();
	m_Canvas->Initialize(rectWindow, hdcScreen);

	m_Camera = new CameraClass(rectWindow.right / rectWindow.bottom, 70.0f);
	m_Camera->Update();
}

void RenderClass::Shutdown() {
	m_Canvas->Shutdown();
	delete m_Canvas;
	m_Canvas = NULL;
}

void RenderClass::UpdateCanvasSettings(RECT newRect) {
	m_Canvas->UpdateSettings(newRect);
	m_Camera->Update(newRect.right / newRect.bottom, 0.0f);
}

void RenderClass::RenderAFrame() {
	fps.computeTime();
	m_Canvas->OutputText(fps.getFPSInfo(), 0);

	//todo
	//WorldToView = m_Camera->GetWorldToViewMatrix4();




	
	//将后台缓存交换至屏幕
	m_Canvas->SwapBufferToScreen();
	m_Canvas->ClearCanvas();
}