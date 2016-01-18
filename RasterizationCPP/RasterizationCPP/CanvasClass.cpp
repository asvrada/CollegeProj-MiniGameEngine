#include "CanvasClass.h"

CanvasClass::CanvasClass() {
	m_hdcBuffer = m_hdcScreen = NULL;
	m_brushBackground = NULL;
}

CanvasClass::~CanvasClass() {
}

void CanvasClass::Initialize(RECT rectWindow, HDC hdcScreen) {
	m_hdcScreen = hdcScreen;

	UpdateSettings(rectWindow);
}

void CanvasClass::Shutdown() {
	//TODO
	//m_Render->Shutdown();
	DeleteResources();
}

void CanvasClass::DeleteResources() {
	DeleteDC(m_hdcBuffer);
	DeleteObject(m_brushBackground);
}

void CanvasClass::UpdateSettings(RECT rectNew) {
	m_rectRenderArea = rectNew;

	if (m_hdcBuffer) {
		DeleteResources();
	}

	//此时的buffer没有大小
	m_hdcBuffer = CreateCompatibleDC(m_hdcScreen);

	//载入本地背景图片
	HBITMAP bmpBackgroundImage = (HBITMAP)LoadImage(NULL, TEXT("ImageResources\\Materials\\Background.bmp"), IMAGE_BITMAP, m_rectRenderArea.right, m_rectRenderArea.bottom, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	//如果载入失败
	//创建灰色背景
	if (bmpBackgroundImage == NULL) {
		bmpBackgroundImage = CreateCompatibleBitmap(m_hdcScreen, m_rectRenderArea.right, m_rectRenderArea.bottom);

		SelectObject(m_hdcBuffer, bmpBackgroundImage);
		FillRect(m_hdcBuffer, &m_rectRenderArea, (HBRUSH)GetStockObject(GRAY_BRUSH));
	}
	else {
		//选入物体
		SelectObject(m_hdcBuffer, bmpBackgroundImage);
	}

	m_brushBackground = CreatePatternBrush(bmpBackgroundImage);

	//选入后 bmp 就可以删了
	DeleteObject(bmpBackgroundImage);

	SetBkMode(m_hdcBuffer, TRANSPARENT);
}

void CanvasClass::Draw(int x , int y, COLORREF color) {
	SetPixel(m_hdcBuffer, x, y, color);
}

void CanvasClass::SwapBufferToScreen() {
	BitBlt(m_hdcScreen, 0, 0, m_rectRenderArea.right, m_rectRenderArea.bottom, m_hdcBuffer, 0, 0, SRCCOPY);
}

void CanvasClass::ClearCanvas()
{
	//用背景画刷填充背景
	//FillRect(hdcBuffer, &rectRender, brushBackground);
	FillRect(m_hdcBuffer, &m_rectRenderArea, m_brushBackground);
}

void CanvasClass::OutputText(wchar_t text[],int line) {
	TextOut(m_hdcBuffer, 0, 20*line, text, wcslen(text));
}