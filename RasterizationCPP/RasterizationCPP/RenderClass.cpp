#include "RenderClass.h"

void RenderClass::Draw() {
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
			transformedVertexes[lop].w = 1;
		}

		//Draw
		//m_Canvas->Draw(transformedVertexes[lop].x, transformedVertexes[lop].y, RGB(0, 0, 0));
		m_Canvas->DrawTriangle(transformedVertexes, RGB(0, 0, 0));
	}
}

RenderClass::RenderClass() {
	m_Canvas = NULL;
	m_Camera = NULL;
}


RenderClass::~RenderClass() {
	delete m_Canvas;
	delete m_Camera;
}

void RenderClass::Initialize(RECT rectWindow, HDC hdcScreen) {
	m_rectRenderArea = rectWindow;
	m_Canvas = new CanvasClass();
	m_Canvas->Initialize(rectWindow, hdcScreen);

	m_Camera = new CameraClass((float)(rectWindow.right / rectWindow.bottom), 70.0f);
	m_Camera->Update();

	//初始化物体
	RenderObjects.push_back(Object());
	RenderObjects.push_back(Object());
	RenderObjects[1].vertex[2].x = 100.0f;
	RenderObjects[1].vertex[2].y = 100.0f;
	m_Camera->Rotation.y = 30.0f;
	m_Camera->Position.x = -30.0f;
	m_Camera->Position.y = 50.0f;
	m_Camera->Update();
}

void RenderClass::Shutdown() {
	m_Canvas->Shutdown();
	delete m_Canvas;
	m_Canvas = NULL;
}

void RenderClass::UpdateCanvasSettings(RECT newRect) {
	m_rectRenderArea = newRect;
	m_Canvas->UpdateSettings(newRect);
	m_Camera->Update((float)newRect.right / (float)newRect.bottom, 0.0f);
}

void RenderClass::RenderAFrame() {
	fps.computeTime();
	m_Canvas->OutputText(fps.getFPSwstring(), 0);

	wstringstream ws;
	ws << "[ " << m_rectRenderArea.right << ", " << m_rectRenderArea.bottom << " ]";
	m_Canvas->OutputText(ws.str(), 1);

	WorldToView = m_Camera->GetWorldToViewMatrix4();
	Draw();


	//将后台缓存交换至屏幕
	m_Canvas->SwapBufferToScreen();
	m_Canvas->ClearCanvas();
}