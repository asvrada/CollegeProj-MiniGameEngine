#include "RenderClass.h"

RenderClass::RenderClass() {
	m_rectRenderArea.bottom = m_rectRenderArea.left = m_rectRenderArea.right = m_rectRenderArea.top = 0;
	
	m_hdcBuffer = m_hdcScreen = nullptr;
	m_bmpBackgroundImage = nullptr;
	m_brushBackgroundImage = nullptr;
}

RenderClass::RenderClass(RECT rectWindow) {
	m_rectRenderArea = rectWindow;

	m_hdcBuffer = m_hdcScreen = nullptr;
	m_bmpBackgroundImage = nullptr;
	m_brushBackgroundImage = nullptr;
}


RenderClass::~RenderClass() {
}

void RenderClass::Initialize(RECT rectWindow, HDC hdcScreen) {
	m_rectRenderArea = rectWindow;
	m_hdcScreen = hdcScreen;

	m_UpdateSettings();
}

void RenderClass::ShutDown() {
	//TODO
	DeleteResources();
}

void RenderClass::DeleteResources() {
	DeleteDC(m_hdcBuffer);
	DeleteObject(m_bmpBackgroundImage);
	DeleteObject(m_brushBackgroundImage);
}

void RenderClass::UpdateSettings(RECT rectNew) {
	m_rectRenderArea = rectNew;

	m_UpdateSettings();
}
void RenderClass::UpdateSettings(int rectRight, int rectBottom) {
	m_rectRenderArea.top = m_rectRenderArea.left = 0;
	m_rectRenderArea.right = rectRight;
	m_rectRenderArea.bottom = rectBottom;

	m_UpdateSettings();
}

void RenderClass::RenderAFrame() {
	//TODO






	//最后交换缓存
	SwapBufferToScreen();
}

void RenderClass::SwapBufferToScreen() {
	BitBlt(m_hdcScreen, 0, 0, m_rectRenderArea.right, m_rectRenderArea.bottom, m_hdcBuffer, 0, 0, SRCCOPY);
}

void RenderClass::m_UpdateSettings() {
	DeleteResources();

	//载入本地图片
	m_bmpBackgroundImage = (HBITMAP)LoadImage(NULL, TEXT("ImageResources\\Background.bmp"), IMAGE_BITMAP, m_rectRenderArea.right, m_rectRenderArea.bottom, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	m_brushBackgroundImage = CreatePatternBrush(m_bmpBackgroundImage);

	m_hdcBuffer = CreateCompatibleDC(m_hdcScreen);
	SelectObject(m_hdcBuffer, m_bmpBackgroundImage);
}