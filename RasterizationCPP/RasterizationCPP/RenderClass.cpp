#include "RenderClass.h"

#include "CameraClass.h"
#include "TimeClass.h"
#include "ObjectClass.h"
#include "WindowFrameClass.h"

void Render::m_DrawObjects() {
	if (vector_objects.size() == 0) {
		return;
	}
	m_world_to_view = m_ptr_camera->GetWorldToViewMatrix4();
	Matrix4 LocalToView;
	for (auto object : vector_objects) {
		if ((int)object.vertices.size() == 0) {
			continue;
		}
		vector<Vector4> transformed_vertices;

		LocalToView = Matrix4(1.0f) * Matrix4('A', object.rotation) * Matrix4(object.position);
		LocalToView = LocalToView * m_world_to_view;

		for (int lop = 0; lop < (int)object.vertices.size(); lop++) {
			//Local To View transformation
			transformed_vertices.push_back(object.vertices[lop] * LocalToView);

			//View To Homo transformation
			transformed_vertices[lop] = transformed_vertices[lop] * m_ptr_camera->view_to_homo;

			//Perspective Divison
			float z_depth = ABS(transformed_vertices[lop].w);
			transformed_vertices[lop].x /= z_depth;
			transformed_vertices[lop].y /= z_depth;
			transformed_vertices[lop].z /= z_depth;
		}

		//Clipping
		vector<Vector2<int>> tmp_indices;
		Clipping(tmp_indices, transformed_vertices, object.indices);

		if ((WindowFrame::STYLE_CHECKER & RENDER_MODE_MASK) == RENDER_MODE_OUTLINE) {
			DrawTriangles(transformed_vertices, tmp_indices);
		}
		else {
			FillTriangles(transformed_vertices, object.uv, tmp_indices, object.hdc_texture);
		}
	}
}

Render::Render() {
	m_ptr_camera = nullptr;
	m_z_depth_buffer = nullptr;
}

Render::~Render() {
	Shutdown();
}

void Render::Initialize(HWND *hWndScreen) {
	m_ptr_hwnd = hWndScreen;

	m_hdc_screen = GetDC(*m_ptr_hwnd);

	m_ptr_camera = new Camera((float)(WindowFrame::rect_client.right / WindowFrame::rect_client.bottom), 70.0f);
	m_ptr_camera->position.z = -70.f;
	m_ptr_camera->Update();

	//初始化物体
	vector_objects.push_back(Object());
	if (vector_objects[0].Initial("Resources\\Models\\plane.obj", TEXT("Resources\\Materials\\CheckerboardTexture.bmp")) == ERROR) {
		vector_objects.erase(vector_objects.end() - 1);
	}
	vector_objects[0].rotation = Vector3(0, 180.f, 0);
}

void Render::DeleteResources() {
	DeleteDC(m_hdc_buffer);
	DeleteObject(m_brush_background);
}

void Render::Shutdown() {
	DeleteResources();
	if (m_ptr_camera) {
		delete m_ptr_camera;
		m_ptr_camera = nullptr;
	}
	if (m_z_depth_buffer) {
		delete m_z_depth_buffer;
		m_z_depth_buffer = nullptr;
	}
}

void Render::RenderAFrame() {
	////////////////
	// >每帧必做 //
	////////////////
	m_ptr_camera->CameraControl();
	m_DrawObjects();
	////////////////
	// <每帧必做 //
	////////////////


	OutputText(Time::GetFPSwstring(), 0);

	wstringstream ws;
	ws <<"Resolution : " << WindowFrame::rect_client.right << " * " << WindowFrame::rect_client.bottom;
	OutputText(ws.str(), 1);

	ws.str(L"");
	ws << m_ptr_camera->position << m_ptr_camera->rotation;
	OutputText(ws.str(), 2);

	////////////////
	// >每帧必做 //
	////////////////
	SwapBufferToScreen();
	ClearCanvas();
	////////////////
	// <每帧必做 //
	////////////////
}

void Render::UpdateSettings()
{
	//改变z缓存的大小
	if (m_z_depth_buffer) {
		delete m_z_depth_buffer;
		m_z_depth_buffer = nullptr;
	}

	m_z_depth_buffer = new float[WindowFrame::rect_client.right  * WindowFrame::rect_client.bottom];

	//不为空时删除原有数据
	if (m_hdc_buffer) {
		DeleteResources();
	}
	//此时的buffer没有大小
	m_hdc_buffer = CreateCompatibleDC(m_hdc_screen);

	//载入本地背景图片
	HBITMAP bmpBackgroundImage = (HBITMAP)LoadImage(NULL, NULL/*TEXT("Resources\\Materials\\Background.bmp")*/, IMAGE_BITMAP,
		WindowFrame::rect_client.right, WindowFrame::rect_client.bottom, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	//如果载入失败
	//创建灰色背景
	if (bmpBackgroundImage == NULL) {
		bmpBackgroundImage = CreateCompatibleBitmap(m_hdc_screen, WindowFrame::rect_client.right, WindowFrame::rect_client.bottom);

		SelectObject(m_hdc_buffer, bmpBackgroundImage);
		FillRect(m_hdc_buffer, &WindowFrame::rect_client, (HBRUSH)GetStockObject(GRAY_BRUSH));
	}
	else {
		//选入物体
		SelectObject(m_hdc_buffer, bmpBackgroundImage);
	}
	//根据产生的图案创建画刷
	m_brush_background = CreatePatternBrush(bmpBackgroundImage);
	//之后 bmp 就可以删了
	//以后用画刷就行了
	DeleteObject(bmpBackgroundImage);
	//填充模式设为透明
	SetBkMode(m_hdc_buffer, TRANSPARENT);

	//升级摄像机的数据
	m_ptr_camera->Update((float)WindowFrame::rect_client.right / (float)WindowFrame::rect_client.bottom, m_ptr_camera->fov);
}

void Render::SwapBufferToScreen() {
	//BitBlt(m_hdc_screen, 0, 0, WindowFrame::rect_client.right, WindowFrame::rect_client.bottom, vector_objects[0].hdc_texture, 0, 0, SRCCOPY);
	BitBlt(m_hdc_screen, 0, 0, WindowFrame::rect_client.right, WindowFrame::rect_client.bottom, m_hdc_buffer, 0, 0, SRCCOPY);
}

void Render::ClearCanvas() {
	std::fill(m_z_depth_buffer, m_z_depth_buffer + WindowFrame::rect_client.right  * WindowFrame::rect_client.bottom, -1.0f);
	//用背景画刷填充背景
	FillRect(m_hdc_buffer, &WindowFrame::rect_client, m_brush_background);
}

inline void Render::OutputText(const wstring & text, int line) {
	TextOut(m_hdc_buffer, 0, 20 * line, text.data(), text.size());
}

///////////////////
// Fill Triangles //
///////////////////
void Render::FillTriangles(vector<Vector4>& vertices,vector<Vector2<float>>& UVs, vector<Vector2<int>>& indices, HDC texture)
{
	for (int lop = 0; lop < (int)indices.size(); lop += 3) {
		//一个三角形的三个顶点
		Vector4 a = vertices[indices[lop].x];
		Vector4 b = vertices[indices[lop + 1].x];
		Vector4 c = vertices[indices[lop + 2].x];

		if (TriangleBackcull(a, b, c)) {
			continue;
		}

		//一个三角形三个顶点的uv
		Vector2<float> uv_a = UVs[indices[lop].y];
		Vector2<float> uv_b = UVs[indices[lop + 1].y];
		Vector2<float> uv_c = UVs[indices[lop + 2].y];

		uv_a.x = uv_a.x * 512.0f;
		uv_a.y = uv_a.y * 512.0f;

		uv_b.x = uv_b.x * 512.0f;
		uv_b.y = uv_b.y * 512.0f;

		uv_c.x = uv_c.x * 512.0f;
		uv_c.y = uv_c.y * 512.0f;

		//将三角形三点按照从上到下ABC的顺序重新排列
		//A在最上面
		if (a.y > b.y && a.y > c.y) {
			if (b.y < c.y) {
				swap<Vector4>(b, c);
				swap<Vector2<float>>(uv_b, uv_c);
			}
		}
		else if (b.y > a.y && b.y > c.y) {
			//B 最上面
			//A B 互换
			swap<Vector4>(a, b);
			swap<Vector2<float>>(uv_a, uv_b);
			if (b.y < c.y) {
				swap<Vector4>(b, c);
				swap<Vector2<float>>(uv_b, uv_c);
			}
		}
		else if (c.y > a.y && c.y > b.y) {
			//C 最上面
			//A C 互换
			swap<Vector4>(a, c);
			swap<Vector2<float>>(uv_a, uv_c);
			if (b.y < c.y) {
				swap<Vector4>(b, c);
				swap<Vector2<float>>(uv_b, uv_c);
			}
		}

		Vector4 d;
		Vector2<float> uv_d;

		float t_BYAYCYAY = (b.y - a.y) / (c.y - a.y);
		d.x = t_BYAYCYAY * (c.x - a.x) + a.x;
		d.y = b.y;
		d.z = t_BYAYCYAY * (c.z - a.z) + a.z;
		d.w = 1 / (t_BYAYCYAY * (1 / c.w - 1 / a.w) + 1 / a.w);

		float oneoverz = t_BYAYCYAY * (1 / c.w - 1 / a.w) + 1 / a.w;
		float uvoverz = t_BYAYCYAY * (uv_c.x / c.w - uv_a.x / a.w) + uv_a.x / a.w;
		uv_d.x = uvoverz / oneoverz;
		uvoverz = t_BYAYCYAY * (uv_c.y / c.w - uv_a.y / a.w) + uv_a.y / a.w;
		uv_d.y = uvoverz / oneoverz;

		//将齐次剪裁空间的点转换到视口空间
		HomoToScreenCoord(a);
		HomoToScreenCoord(b);
		HomoToScreenCoord(c);
		HomoToScreenCoord(d);

		//如果新隔出来的D点在B点左边
		if (d.x < b.x)
		{
			FillTriangleTopFlat(d, uv_d, b, uv_b, c, uv_c, texture);
			FillTriangleBottomFlat(a, uv_a, b, uv_b, d, uv_d, texture);
		}
		else {
			FillTriangleTopFlat(b, uv_b, d, uv_d, c, uv_c, texture);
			FillTriangleBottomFlat(a, uv_a, d, uv_d, b, uv_b, texture);
		}
	}
}

void Render::FillTriangleTopFlat(Vector4 a, Vector2<float> uv_a, Vector4 b, Vector2<float> uv_b, Vector4 c, Vector2<float> uv_c, HDC texture) {
	const float &x1 = a.x;
	const float &x2 = b.x;
	const float &x3 = c.x;

	const float &y1 = a.y;
	const float &y2 = b.y;
	const float &y3 = c.y;

	//确定三角形的范围
	int miny = (int)y3;
	int maxy = (int)y1;

	//最终算出的UV坐标
	//范围 [0, 512]
	int u, v;
	//x轴左右两点
	float xLeft, xRight;


	//透视正确的插值计算
	float oneoverz_Left, oneoverz_Right;
	float oneoverz_Top, oneoverz_Bottom;
	float oneoverz, oneoverz_Step;
	//UV 与 1/z
	float uoverz_Top, uoverz_Bottom;
	float uoverz_Left, uoverz_Right;
	float voverz_Top, voverz_Bottom;
	float voverz_Left, voverz_Right;

	float uoverz, uoverz_Step;
	float voverz, voverz_Step;


	//(y - y1) / (y3 - y1)
	float t_YAYCYAY;
	// (y - y2) / (y3 - y2)
	float t_YBYCYBY;

	//双重循环
	for (int y = miny; y <= maxy; y++)
	{
		//将多次用到的数据先算出来
		t_YAYCYAY = (y - y1) / (y3 - y1);
		t_YBYCYBY = (y - y2) / (y3 - y2);

		xLeft = t_YAYCYAY * (x3 - x1) + x1;
		xRight = t_YBYCYBY* (x3 - x2) + x2;

		//透视正确的插值 值的计算
		//1 / z
		oneoverz_Top = 1 / a.w;
		oneoverz_Bottom = 1 / c.w;
		oneoverz_Left = t_YAYCYAY* (oneoverz_Bottom - oneoverz_Top) + oneoverz_Top;
		oneoverz_Top = 1 / b.w;
		oneoverz_Right = t_YBYCYBY * (oneoverz_Bottom - oneoverz_Top) + oneoverz_Top;
		oneoverz_Step = (oneoverz_Right - oneoverz_Left) / (xRight - xLeft);
		//U / z
		uoverz_Top = uv_a.x / a.w;
		uoverz_Bottom = uv_c.x / c.w;
		uoverz_Left = t_YAYCYAY* (uoverz_Bottom - uoverz_Top) + uoverz_Top;
		uoverz_Top = uv_b.x / b.w;
		uoverz_Right = t_YBYCYBY* (uoverz_Bottom - uoverz_Top) + uoverz_Top;
		uoverz_Step = (uoverz_Right - uoverz_Left) / (xRight - xLeft);
		//V / z
		voverz_Top = uv_a.y / a.w;
		voverz_Bottom = uv_c.y / c.w;
		voverz_Left = t_YAYCYAY * (voverz_Bottom - voverz_Top) + voverz_Top;
		voverz_Top = uv_b.y / b.w;
		voverz_Right = t_YBYCYBY* (voverz_Bottom - voverz_Top) + voverz_Top;
		voverz_Step = (voverz_Right - voverz_Left) / (xRight - xLeft);


		int x = 0;
		for (x = (int)xLeft, oneoverz = oneoverz_Left, uoverz = uoverz_Left, voverz = voverz_Left; x <= xRight; x++, oneoverz += oneoverz_Step, uoverz += uoverz_Step, voverz += voverz_Step) {
			u = (int)(uoverz / oneoverz);
			v = (int)(voverz / oneoverz);

			int _x = x;
			int _y = WindowFrame::rect_client.bottom - y;
			int _index = (_y - 1) * WindowFrame::rect_client.right + x;

			//没有超出缓冲区的范围
			if (_index < WindowFrame::rect_client.right * WindowFrame::rect_client.bottom && _index >= 0) {
				float &_z = m_z_depth_buffer[_index];
				if (oneoverz > _z) {
#ifdef DEBUG
					//debug
					assert(u != 512);
					assert(v != 512);
#endif
					DrawPixel(_x, _y, GetPixel(texture, u, v));
					_z = oneoverz;
				}
			}
		}
	}
}

void Render::FillTriangleBottomFlat(Vector4 a, Vector2<float> uv_a, Vector4 b, Vector2<float> uv_b, Vector4 c, Vector2<float> uv_c, HDC texture) {
	const float &x1 = a.x;
	const float &x2 = b.x;
	const float &x3 = c.x;

	const float &y1 = a.y;
	const float &y2 = b.y;
	const float &y3 = c.y;

	//确定三角形的范围
	int miny = (int)y2;
	int maxy = (int)y1;

	//用于直接从载入的纹理采样的UV坐标
	int u, v;
	//左右两点的 x 范围
	float xLeft, xRight;

	//透视正确的插值
	float oneoverz_Left, oneoverz_Right;
	float oneoverz_Top, oneoverz_Bottom;
	float oneoverz, oneoverz_Step;
	//UV 与 1/z
	float uoverz_Top, uoverz_Bottom;
	float uoverz_Left, uoverz_Right;
	float voverz_Top, voverz_Bottom;
	float voverz_Left, voverz_Right;

	float uoverz, uoverz_Step;
	float voverz, voverz_Step;

	//(y - a.y) / (c.y - a.y)
	float t_YAYCYAY;
	//(y - a.y) / (b.y - a.y)
	float t_YAYBYAY;

	//双重循环
	//一行一行的渲染
	for (int y = miny; y <= maxy; y++)
	{
		//将多次用到的数据先算出来
		t_YAYCYAY = (y - y1) / (y3 - y1);
		t_YAYBYAY = (y - y1) / (y2 - y1);

		//当前行的左右端点
		xLeft = t_YAYCYAY * (x3 - x1) + x1;
		xRight = t_YAYBYAY * (x2 - x1) + x1;

		//透视正确的插值 值的计算
		//1 / z
		oneoverz_Top = 1 / a.w;
		oneoverz_Bottom = 1 / c.w;
		oneoverz_Left = t_YAYCYAY * (oneoverz_Bottom - oneoverz_Top) + oneoverz_Top;
		oneoverz_Bottom = 1 / b.w;
		oneoverz_Right = t_YAYBYAY * (oneoverz_Bottom - oneoverz_Top) + oneoverz_Top;
		oneoverz_Step = (oneoverz_Right - oneoverz_Left) / (xRight - xLeft);
		//U / z
		uoverz_Top = uv_a.x / a.w;
		uoverz_Bottom = uv_c.x / c.w;
		uoverz_Left = t_YAYCYAY * (uoverz_Bottom - uoverz_Top) + uoverz_Top;
		uoverz_Bottom = uv_b.x / b.w;
		uoverz_Right = t_YAYBYAY * (uoverz_Bottom - uoverz_Top) + uoverz_Top;
		uoverz_Step = (uoverz_Right - uoverz_Left) / (xRight - xLeft);
		//V / z
		voverz_Top = uv_a.y / a.w;
		voverz_Bottom = uv_c.y / c.w;
		voverz_Left = t_YAYCYAY * (voverz_Bottom - voverz_Top) + voverz_Top;
		voverz_Bottom = uv_b.y / b.w;
		voverz_Right = t_YAYBYAY * (voverz_Bottom - voverz_Top) + voverz_Top;
		voverz_Step = (voverz_Right - voverz_Left) / (xRight - xLeft);


		int x = 0;
		for (x = (int)xLeft, oneoverz = oneoverz_Left, uoverz = uoverz_Left, voverz = voverz_Left; x < xRight; x++, oneoverz += oneoverz_Step, uoverz += uoverz_Step, voverz += voverz_Step) {
			u = (int)(uoverz / oneoverz);
			v = (int)(voverz / oneoverz);

			int _x = x;
			int _y = WindowFrame::rect_client.bottom - y;
			int _index = (_y - 1) * WindowFrame::rect_client.right + x;

			if (_index < WindowFrame::rect_client.right * WindowFrame::rect_client.bottom && _index >= 0) {
				float &_z = m_z_depth_buffer[_index];
				if (oneoverz > _z) {
#ifdef DEBUG
					//debug
					assert(u != 512);
					assert(v != 512);
#endif
					DrawPixel(_x, _y, GetPixel(texture, u, v));
					_z = oneoverz;
				}
			}
		}
	}
}

/////////////////
// Draw Lines //
/////////////////

void Render::DrawTriangles(vector<Vector4>& vertices, vector<Vector2<int>>& indices)
{
	//Draw every face of that object
	int indice_count = (int)indices.size();
	for (int lop = 0; lop < indice_count; lop += 3) {
		DrawTriangle(
			vertices[(int)indices[lop].x],
			vertices[(int)indices[lop + 1].x],
			vertices[(int)indices[lop + 2].x],
			COLOR_BLACK);
	}
}

void Render::DrawTriangle(const Vector4 p0, const Vector4 p1, const Vector4 p2, COLORREF color)
{
	if (TriangleBackcull(p0, p1, p2)) {
		return;
	}

	Vector2<float> vertex[3];
	vertex[0].x = (p0.x + 1.0f) * WindowFrame::rect_client.right / 2.0f;
	vertex[0].y = (p0.y + 1.0f) * WindowFrame::rect_client.bottom / 2.0f;

	vertex[1].x = (p1.x + 1.0f) * WindowFrame::rect_client.right / 2.0f;
	vertex[1].y = (p1.y + 1.0f) * WindowFrame::rect_client.bottom / 2.0f;

	vertex[2].x = (p2.x + 1.0f) * WindowFrame::rect_client.right / 2.0f;
	vertex[2].y = (p2.y + 1.0f) * WindowFrame::rect_client.bottom / 2.0f;

	DrawLine(vertex[0], vertex[1], color);
	DrawLine(vertex[1], vertex[2], color);
	DrawLine(vertex[2], vertex[0], color);
}

void Render::DrawLine(Vector2<float> p0, Vector2<float> p1, COLORREF color)
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
			DrawPixel(painter_y, WindowFrame::rect_client.bottom - i, color);
		}
		else {
			DrawPixel(i, WindowFrame::rect_client.bottom - painter_y, color);
		}
		err -= dy;
		if (err < 0) {
			painter_y += ystep;
			err += dx;
		}
	}
}

inline void Render::DrawPixel(int x, int y, COLORREF color)
{
	SetPixel(m_hdc_buffer, x, y, color);
}
