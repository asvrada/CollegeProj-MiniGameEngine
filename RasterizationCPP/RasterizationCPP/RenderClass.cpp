#include "RenderClass.h"

RenderClass::RenderClass() {
	m_rectRenderArea.bottom = m_rectRenderArea.left = m_rectRenderArea.right = m_rectRenderArea.top = 0;
	
	m_hdcBuffer = m_hdcScreen = nullptr;
	m_bmpBackgroundImage = nullptr;
}

RenderClass::RenderClass(RECT rectWindow) {
	m_rectRenderArea = rectWindow;

	m_hdcBuffer = m_hdcScreen = nullptr;
	m_bmpBackgroundImage = nullptr;
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


	//输出文字信息
	OutputText(L"Debug Infomation Here");
	//最后交换缓存
	SwapBufferToScreen();
}

void RenderClass::SwapBufferToScreen() {
	BitBlt(m_hdcScreen, 0, 0, m_rectRenderArea.right, m_rectRenderArea.bottom, m_hdcBuffer, 0, 0, SRCCOPY);
}

void RenderClass::OutputText(wchar_t text[]) {
	TextOut(m_hdcBuffer, 0, 0, text, wcslen(text));
}

void RenderClass::m_UpdateSettings() {
	if (m_hdcBuffer) {
		DeleteResources();
	}

	//载入本地图片
	m_bmpBackgroundImage = (HBITMAP)LoadImage(NULL, TEXT("ImageResources\\Materials\\Background.bmp"), IMAGE_BITMAP, m_rectRenderArea.right, m_rectRenderArea.bottom, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);

	m_hdcBuffer = CreateCompatibleDC(m_hdcScreen);
	SelectObject(m_hdcBuffer, m_bmpBackgroundImage);

	SetBkMode(m_hdcBuffer, TRANSPARENT);
}