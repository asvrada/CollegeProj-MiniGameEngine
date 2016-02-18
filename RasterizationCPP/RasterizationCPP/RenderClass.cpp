#include "RenderClass.h"

/*
FLOAT3D* SingleObjectLocalToHomo(OBJECT object, MATRIX4 WTV, MATRIX4 VTH)
{
//Local -> world
LocalToView = RST(Scale(1.0f), Rotation(object.rotation), Transition(object.position));
LocalToView = MatrixMul4(LocalToView, WTV);
//将模型的点根据本地到视口矩阵进行变换
//将新产生的点储存起来
FLOAT3D *TmpVertexes = (FLOAT3D*)malloc(object.model.vertexNum*sizeof(FLOAT3D));
int lop;
for (lop = 0; lop < object.model.vertexNum; lop++)
{
VectorTransform(object.model.vertexList[lop].vertex, &TmpVertexes[lop], LocalToView);
}

//然后转换到齐次剪裁空间
float z;
for (lop = 0; lop < object.model.vertexNum; lop++)
{
//深度信息
z = TmpVertexes[lop].z;
VectorTransform(TmpVertexes[lop], &TmpVertexes[lop], VTH);

TmpVertexes[lop].x /= z;
TmpVertexes[lop].y /= z;
}

//缺少CVV剪裁
return TmpVertexes;
}
*/

/*
SingleObjectTmpVertexes = SingleObjectLocalToHomo(objectList[lop], WorldToView, ViewToHomo);

//将所有点转换到屏幕的二维坐标系
for (int lop2 = 0; lop2 < objectList[lop].model.vertexNum; lop2++)
{
SingleObjectTmpVertexes[lop2].x =
(SingleObjectTmpVertexes[lop2].x + 1.0f)* rectRender.right / 2.0f;
SingleObjectTmpVertexes[lop2].y =
(SingleObjectTmpVertexes[lop2].y + 1.0f) * rectRender.bottom / 2.0f;
}
//画出三角形
DrawModelListIndex(SingleObjectTmpVertexes, objectList[lop].model.vertexListIndex, objectList[lop].model.vertexList);

free(SingleObjectTmpVertexes);
SingleObjectTmpVertexes = NULL;
*/
void RenderClass::Draw() {
	if (RenderObjects.size() == 0) {
		RenderObjects.push_back(Object());
	}

	Matrix4 LocalToView;
	Vector4 transformedVertexes[3];
	for (Object object : RenderObjects) {
		LocalToView = Matrix4('A',object.Rotation) * Matrix4(object.Position);
		LocalToView = LocalToView * WorldToView;
		for (int lop = 0; lop < 3; lop++) {
			//Local To View transformation
			transformedVertexes[lop] = object.vertex[lop] * LocalToView;
			
			//View To Homo transformation
			float z_depth;
			//todo
		}
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
	m_Canvas->OutputText(fps.getFPSwstring(), 0);

	
	WorldToView = m_Camera->GetWorldToViewMatrix4();
	Draw();

	
	//将后台缓存交换至屏幕
	m_Canvas->SwapBufferToScreen();
	m_Canvas->ClearCanvas();
}