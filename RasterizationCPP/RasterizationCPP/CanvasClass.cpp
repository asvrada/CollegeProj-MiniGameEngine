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
	HBITMAP bmpBackgroundImage = (HBITMAP)LoadImage(NULL, TEXT("ImageResources\\Materials\\white.bmp"), IMAGE_BITMAP,
		m_rectRenderArea.right, m_rectRenderArea.bottom, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
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

void CanvasClass::Draw(int x, int y, COLORREF color) {
	SetPixel(m_hdcBuffer, x, y, color);
}

void CanvasClass::DrawTriangle(const Vector4 set[], COLORREF color) {
	Vector2 vertex[3];
	for (int lop = 0; lop < 3; lop++) {
		vertex[lop].x = (set[lop].x + 1.0f) * m_rectRenderArea.right / 2.0f;
		vertex[lop].y = (set[lop].y + 1.0f) * m_rectRenderArea.bottom / 2.0f;
	}
	DrawLine(vertex[0], vertex[1], color);
	DrawLine(vertex[1], vertex[2], color);
	DrawLine(vertex[2], vertex[0], color);
}

void CanvasClass::DrawLine(Vector2 p0, Vector2 p1, COLORREF color) {
	//直线斜率是否大于1
	BOOL steep = ABS(p1.y - p0.y) > ABS(p1.x - p0.x);
	//如果大于1
	//将直线沿 y=x 翻转输出
	if (steep) {
		swap<float>(p0.x, p0.y);
		swap<float>(p1.x, p1.y);
	}
	if (p0.x > p1.x) {
		swap<float>(p0.x, p1.x);
		swap<float>(p0.y, p1.y);
	}
	int dx = (int)(p1.x - p0.x);
	int dy = (int)ABS(p1.y - p0.y);

	int err = dx / 2;

	//y的增量
	int ystep = (p0.y < p1.y) ? 1 : -1;
	//用于绘画的 y 数值
	int painter_y = (int)p0.y;

	for (int i = (int)p0.x; i <= p1.x; i++) {
		if (steep) {
			Draw(painter_y, m_rectRenderArea.bottom - i, color);
		}
		else {
			Draw(i, m_rectRenderArea.bottom - painter_y, color);
		}
		err -= dy;
		if (err < 0) {
			painter_y += ystep;
			err += dx;
		}
	}
}

void CanvasClass::SwapBufferToScreen() {
	BitBlt(m_hdcScreen, 0, 0, m_rectRenderArea.right, m_rectRenderArea.bottom, m_hdcBuffer, 0, 0, SRCCOPY);
}

void CanvasClass::ClearCanvas() {
	//用背景画刷填充背景
	FillRect(m_hdcBuffer, &m_rectRenderArea, m_brushBackground);
}

void CanvasClass::OutputText(const wstring& text, int line) {
	TextOut(m_hdcBuffer, 0, 20 * line, text.data(), text.size());
}