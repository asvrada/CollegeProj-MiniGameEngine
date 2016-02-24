#include "RenderClass.h"

void RenderClass::DrawObjects() {
	WorldToView = m_Camera->GetWorldToViewMatrix4();
	Matrix4 LocalToView;
	Vector4 transformedVertexes[3];
	for (Object object : RenderObjects) {
		LocalToView = Matrix4('A', object.Rotation) * Matrix4(object.Position);
		LocalToView = LocalToView * WorldToView;

		for (int lop = 0; lop < 3; lop++) {
			//Local To View transformation
			transformedVertexes[lop] = object.vertex[lop] * LocalToView;

			//View To Homo transformation
			transformedVertexes[lop] = transformedVertexes[lop] * m_Camera->ViewToHomo;

			float z_depth = transformedVertexes[lop].w;
			transformedVertexes[lop].x /= z_depth;
			transformedVertexes[lop].y /= z_depth;
			transformedVertexes[lop].z /= z_depth;
		}

		//Draw
		DrawTriangle(transformedVertexes, COLOR_BLACK);
	}
}

RenderClass::RenderClass(InputClass *input) {
	m_ptr_Input = input;
	m_Camera = NULL;
}


RenderClass::~RenderClass() {
	delete m_Camera;
}

void RenderClass::Initialize(RECT *rectWindow, HWND *hWndScreen) {
	m_ptr_rectRenderArea = rectWindow;
	m_ptr_hwnd = hWndScreen;

	m_hdcScreen = GetDC(*m_ptr_hwnd);

	m_Camera = new CameraClass((float)(rectWindow->right / rectWindow->bottom), 70.0f, m_ptr_Input, &fps);
	m_Camera->Update();

	//初始化物体
	RenderObjects.push_back(Object());
	RenderObjects.push_back(Object());
	RenderObjects[1].vertex[2].x = 100.0f;
	RenderObjects[1].vertex[2].y = 100.0f;
}

void RenderClass::DeleteResources() {
	DeleteDC(m_hdcBuffer);
	DeleteObject(m_brushBackground);
}

void RenderClass::Shutdown() {
	DeleteResources();
	delete m_Camera;
	m_Camera = nullptr;
}

void RenderClass::RenderAFrame() {
	////////////////
	// >每帧必做 //
	////////////////
	fps.computeTime();
	GetCursorPos(&m_ptr_Input->rectCursor);
	m_Camera->CameraControl();
	////////////////
	// <每帧必做 //
	////////////////

	//if (m_ptr_Input->IsKeyPressed('Z')) {
	//}


	OutputText(fps.getFPSwstring(), 0);

	//
	wstringstream ws;
	ws <<"Resolution : " << m_ptr_rectRenderArea->right << " * " << m_ptr_rectRenderArea->bottom;
	OutputText(ws.str(), 1);

	ws.str(L"");
	ws << "Use W, A, S, D ,Q ,E and Arrow keys to move around";
	OutputText(ws.str(), 2);

	
	////////////////
	// >每帧必做 //
	////////////////

	DrawObjects();

	m_ptr_Input->clearFlag();
	SwapBufferToScreen();
	ClearCanvas();
	////////////////
	// <每帧必做 //
	////////////////
}

void RenderClass::UpdateSettings()
{
	//不为空时删除原有数据
	if (m_hdcBuffer) {
		DeleteResources();
	}
	//此时的buffer没有大小
	m_hdcBuffer = CreateCompatibleDC(m_hdcScreen);

	//载入本地背景图片
	HBITMAP bmpBackgroundImage = (HBITMAP)LoadImage(NULL, TEXT("ImageResources\\Materials\\Background.bmp"), IMAGE_BITMAP,
		m_ptr_rectRenderArea->right, m_ptr_rectRenderArea->bottom, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	//如果载入失败
	//创建灰色背景
	if (bmpBackgroundImage == NULL) {
		bmpBackgroundImage = CreateCompatibleBitmap(m_hdcScreen, m_ptr_rectRenderArea->right, m_ptr_rectRenderArea->bottom);

		SelectObject(m_hdcBuffer, bmpBackgroundImage);
		FillRect(m_hdcBuffer, m_ptr_rectRenderArea, (HBRUSH)GetStockObject(GRAY_BRUSH));
	}
	else {
		//选入物体
		SelectObject(m_hdcBuffer, bmpBackgroundImage);
	}
	//根据产生的图案创建画刷
	m_brushBackground = CreatePatternBrush(bmpBackgroundImage);
	//之后 bmp 就可以删了
	//以后用画刷就行了
	DeleteObject(bmpBackgroundImage);
	//填充模式设为透明
	SetBkMode(m_hdcBuffer, TRANSPARENT);

	//升级摄像机的数据
	m_Camera->Update((float)m_ptr_rectRenderArea->right / (float)m_ptr_rectRenderArea->bottom, m_Camera->FOV);
}

void RenderClass::SwapBufferToScreen() {
	BitBlt(m_hdcScreen, 0, 0, m_ptr_rectRenderArea->right, m_ptr_rectRenderArea->bottom, m_hdcBuffer, 0, 0, SRCCOPY);
}

void RenderClass::ClearCanvas() {
	//用背景画刷填充背景
	FillRect(m_hdcBuffer, m_ptr_rectRenderArea, m_brushBackground);
}

void RenderClass::OutputText(const wstring & text, int line) {
	TextOut(m_hdcBuffer, 0, 20 * line, text.data(), text.size());
}

void RenderClass::DrawPixel(int x, int y, COLORREF color)
{
	SetPixel(m_hdcBuffer, x, y, color);
}

void RenderClass::DrawTriangle(const Vector4 set[], COLORREF color)
{
	if (triangleBackcull(set)) {
		return;
	}

	Vector2 vertex[3];
	for (int lop = 0; lop < 3; lop++) {
		vertex[lop].x = (set[lop].x + 1.0f) * m_ptr_rectRenderArea->right / 2.0f;
		vertex[lop].y = (set[lop].y + 1.0f) * m_ptr_rectRenderArea->bottom / 2.0f;
	}
	DrawLine(vertex[0], vertex[1], color);
	DrawLine(vertex[1], vertex[2], color);
	DrawLine(vertex[2], vertex[0], color);
}

void RenderClass::DrawLine(Vector2 p0, Vector2 p1, COLORREF color)
{
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
			DrawPixel(painter_y, m_ptr_rectRenderArea->bottom - i, color);
		}
		else {
			DrawPixel(i, m_ptr_rectRenderArea->bottom - painter_y, color);
		}
		err -= dy;
		if (err < 0) {
			painter_y += ystep;
			err += dx;
		}
	}
}
