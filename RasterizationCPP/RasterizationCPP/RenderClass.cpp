#include "RenderClass.h"

//为了用 static
#include "WindowFrameClass.h"

#include "GameObjectClass.h"

void Render::m_DrawObjects() {
	auto &vector_objects = m_ptr_manager->objects_all;
	if (vector_objects.size() == 0) {
		return;
	}
	auto camera = PTR_CONVERT(vector_objects[0], Camera);
	//根据摄像机位置创建世界到齐次剪裁空间的变换矩阵
	Matrix4 WorldToHomo = camera->world_to_view * camera->view_to_homo;

	//最终三角形渲染列表
	vector<Fragment> render_list;

	//对每个将要渲染的模型进行逐个操作
	for (auto meta_object : vector_objects) {
		//进行类型转换
		auto object = PTR_CONVERT(meta_object, GameObject);
		//转换失败
		if (object == NULL) { continue; }
		//转换成功，读取模型
#ifdef DEBUG
		assert(object->model.empty());
#endif
		auto &model = m_ptr_manager->map_models[object->model];
		if ((int)model.vertices.size() == 0) { continue; }

		//新建本地到齐次剪裁空间的矩阵，用于下面的计算
		//依次乘以该模型的缩放参数、旋转参数、平移参数
		Matrix4 LocalToHomo = Matrix4('A', object->rotation) * Matrix4(object->position);
		LocalToHomo = LocalToHomo * WorldToHomo;

		//用于暂时保存该模型变换后的顶点
		vector<Vector4> trans_ver;
		//对该模型的每一个点进行本地到齐次剪裁空间的变换
		for (auto item : model.vertices) {
			trans_ver.push_back(item * LocalToHomo);
		}

		//将每个物体拆分成三角形的列表
		for (auto lop = model.indices.begin(); lop != model.indices.end(); lop += 3) {
			render_list.push_back(Fragment(FRAGMENT_GOOD, &model.hdc_texture));
			auto cur = render_list.end() - 1;
			cur->uvList[0] = model.uv[lop->y] * 511.0f;
			cur->uvList[1] = model.uv[(lop + 1)->y] * 511.0f;
			cur->uvList[2] = model.uv[(lop + 2)->y] * 511.0f;

			//在此直接复制变换好的顶点
			cur->trans_vList[0] = trans_ver[lop->x];
			cur->trans_vList[1] = trans_ver[(lop + 1)->x];
			cur->trans_vList[2] = trans_ver[(lop + 2)->x];
		}
	}
	//进行背面剔除及剪裁
	ClippingAndBackCull(render_list);

	//进行透视除法
	for (auto &item : render_list) {
		if (item.state & FRAGMENT_DELETED) {
			continue;
		}
		for (int lop = 0; lop < 3; lop++) {
			const float &z_depth = item.trans_vList[lop].w;
			item.trans_vList[lop].x /= z_depth;
			item.trans_vList[lop].y /= z_depth;
			item.trans_vList[lop].z /= z_depth;
		}
	}

	//选择渲染模式
	if ((WindowFrame::STYLE_CHECKER & RENDER_MODE_MASK) == RENDER_MODE_OUTLINE) {
		//线框
		DrawTriangles(render_list);
	}
	else {
		//填充
		FillTriangles(render_list);
	}
}

Render::Render() {
	m_ptr_manager = nullptr;
	m_z_depth_buffer = nullptr;
}

Render::~Render() {
	Shutdown();
}

void Render::Initialize(HWND *hWndScreen) {
	m_ptr_hwnd = hWndScreen;

	m_hdc_screen = GetDC(*m_ptr_hwnd);
}

void Render::DeleteResources() {
	DeleteDC(m_hdc_buffer);
	DeleteObject(m_brush_background);
}

void Render::Shutdown() {
	DeleteResources();
	if (m_z_depth_buffer) {
		delete m_z_depth_buffer;
		m_z_depth_buffer = nullptr;
	}
}

void Render::RenderAFrame() {
	////////////////
	// >每帧必做 //
	////////////////
	m_DrawObjects();
	////////////////
	// <每帧必做 //
	////////////////
	OutputText(Time::GetFPSwstring(), 0);

	wstringstream ws;
	ws << "Resolution : " << WindowFrame::rect_client.right << " * " << WindowFrame::rect_client.bottom;
	OutputText(ws.str(), 1);

	ws.str(L"");
	auto ptr_camera = PTR_CONVERT(m_ptr_manager->objects_all[0], Camera);
	ws << ptr_camera->position << ptr_camera->rotation;
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
	//HBITMAP bmpBackgroundImage = (HBITMAP)LoadImage(NULL, TEXT("Resources\\Materials\\Background.bmp"), IMAGE_BITMAP, WindowFrame::rect_client.right, WindowFrame::rect_client.bottom, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	HBITMAP bmpBackgroundImage = NULL;

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
	PTR_CONVERT(m_ptr_manager->objects_all[0], Camera)->ChangeConfig((float)WindowFrame::rect_client.right / (float)WindowFrame::rect_client.bottom, 0);
}

inline void Render::SwapBufferToScreen() {
	//BitBlt(m_hdc_screen, 0, 0, WindowFrame::rect_client.right, WindowFrame::rect_client.bottom, vector_objects[0].hdc_texture, 0, 0, SRCCOPY);
	BitBlt(m_hdc_screen, 0, 0, WindowFrame::rect_client.right, WindowFrame::rect_client.bottom, m_hdc_buffer, 0, 0, SRCCOPY);
}

inline void Render::ClearCanvas() {
	std::fill(m_z_depth_buffer, m_z_depth_buffer + WindowFrame::rect_client.right  * WindowFrame::rect_client.bottom, -1.0f);
	//用背景画刷填充背景
	FillRect(m_hdc_buffer, &WindowFrame::rect_client, m_brush_background);
}

inline void Render::OutputText(const wstring & text, int line) {
	TextOut(m_hdc_buffer, 0, 20 * line, text.data(), (int)text.size());
}

///////////////////
// Fill Triangles //
///////////////////
void Render::FillTriangles(vector<Fragment> &list)
{
	for (auto &item : list) {
		if (item.state & FRAGMENT_DELETED) {
			continue;
		}

		Vector4 &a = item.trans_vList[0];
		Vector4 &b = item.trans_vList[1];
		Vector4 &c = item.trans_vList[2];

		Vector2<float> &uv_a = item.uvList[0];
		Vector2<float> &uv_b = item.uvList[1];
		Vector2<float> &uv_c = item.uvList[2];


		//按照 a.y > b.y > c.y 的顺序排好
		if (a.y > b.y && a.y > c.y) {
			//A 最上面
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


		//将齐次剪裁空间的点转换到视口空间
		HomoToScreenCoord(a);
		HomoToScreenCoord(b);
		HomoToScreenCoord(c);

		//创建剪裁出的D点
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

		//确保三角形顶点缠绕方向均为顺时针
		if (d.x < b.x) {
			FillTriangleTopFlat(d, uv_d, b, uv_b, c, uv_c, item.texture);
			FillTriangleBottomFlat(a, uv_a, b, uv_b, d, uv_d, item.texture);
		}
		else {
			FillTriangleTopFlat(b, uv_b, d, uv_d, c, uv_c, item.texture);
			FillTriangleBottomFlat(a, uv_a, d, uv_d, b, uv_b, item.texture);
		}
	}
}

///////////////////////////
// 三角形插值填充算法 //
//////////////////////////
void Render::FillTriangleTopFlat(Vector4 &p0, Vector2<float>& uv_p0, Vector4 &p1, Vector2<float> &uv_p1, Vector4& p2, Vector2<float>& uv_p2, HDC *texture) {
	//左边这条线上的变化量
	float dxdyl = (p2.x - p0.x) / (p2.y - p0.y);
	float d_one_over_z_dyl = (1 / p2.w - 1 / p0.w) / (p2.y - p0.y);
	float d_u_over_z_dyl = (uv_p2.x / p2.w - uv_p0.x / p0.w) / (p2.y - p0.y);
	float d_v_over_z_dyl = (uv_p2.y / p2.w - uv_p0.y / p0.w) / (p2.y - p0.y);

	//右边这条线上的变化量
	float dxdyr = (p2.x - p1.x) / (p2.y - p0.y);
	float d_one_over_z_dyr = (1 / p2.w - 1 / p1.w) / (p2.y - p0.y);
	float d_u_over_z_dyr = (uv_p2.x / p2.w - uv_p1.x / p1.w) / (p2.y - p0.y);
	float d_v_over_z_dyr = (uv_p2.y / p2.w - uv_p1.y / p1.w) / (p2.y - p0.y);

	//扫描线的起终点
	int ystart = (int)ceil(p0.y);
	int yend = (int)ceil(p2.y) - 1;

	//初始值
	float x_left = p0.x + (ystart - p0.y) * dxdyl;
	float one_over_z_left = 1 / p0.w;
	float u_over_z_left = uv_p0.x / p0.w;
	float v_over_z_left = uv_p0.y / p0.w;
	float x_right = p1.x + (ystart - p0.y) * dxdyr;
	float one_over_z_right = 1 / p1.w;
	float u_over_z_right = uv_p1.x / p1.w;
	float v_over_z_right = uv_p1.y / p1.w;

	for (int y = ystart; y <= yend; y++) {
		int xstart = (int)ceil(x_left);
		int xend = (int)ceil(x_right) - 1;

		float ddz_step = (one_over_z_right - one_over_z_left) / (float)(xend - xstart);
		float ddu_step = (u_over_z_right - u_over_z_left) / (float)(xend - xstart);
		float ddv_step = (v_over_z_right - v_over_z_left) / (float)(xend - xstart);
		float ddz = one_over_z_left;
		float ddu = u_over_z_left;
		float ddv = v_over_z_left;

		for (int x = xstart; x <= xend; x++) {
			float u = ddu / ddz;
			float v = ddv / ddz;

			int _index = (y - 1) * WindowFrame::rect_client.right + x;

			if (_index < WindowFrame::rect_client.right * WindowFrame::rect_client.bottom && _index >= 0) {
				float &_z = m_z_depth_buffer[_index];
				if (ddz > _z) {
					DrawPixel(x, y, GetPixel(*texture, (int)u, (int)v));
					_z = ddz;
				}
			}
			ddz += ddz_step;
			ddu += ddu_step;
			ddv += ddv_step;
		}
		x_left += dxdyl;
		one_over_z_left += d_one_over_z_dyl;
		u_over_z_left += d_u_over_z_dyl;
		v_over_z_left += d_v_over_z_dyl;

		x_right += dxdyr;
		one_over_z_right += d_one_over_z_dyr;
		u_over_z_right += d_u_over_z_dyr;
		v_over_z_right += d_v_over_z_dyr;
	}
}

void Render::FillTriangleBottomFlat(Vector4 &p0, Vector2<float>& uv_p0, Vector4 &p1, Vector2<float> &uv_p1, Vector4& p2, Vector2<float>& uv_p2, HDC *texture) {
	//左边这条线上的变化量
	//float oneOVERp2ySUBp0y = 1 / (p2.y - p0.y);
	float dxdyl = (p2.x - p0.x) / (p2.y - p0.y);
	float d_one_over_z_dyl = (1.0f / p2.w - 1.0f / p0.w) / (p2.y - p0.y);
	float d_u_over_z_dyl = (uv_p2.x / p2.w - uv_p0.x / p0.w) / (p2.y - p0.y);
	float d_v_over_z_dyl = (uv_p2.y / p2.w - uv_p0.y / p0.w) / (p2.y - p0.y);

	//右边这条线上的变化量
	float dxdyr = (p1.x - p0.x) / (p2.y - p0.y);
	float d_one_over_z_dyr = (1.0f / p1.w - 1.0f / p0.w) / (p2.y - p0.y);
	float d_u_over_z_dyr = (uv_p1.x / p1.w - uv_p0.x / p0.w) / (p2.y - p0.y);
	float d_v_over_z_dyr = (uv_p1.y / p1.w - uv_p0.y / p0.w) / (p2.y - p0.y);

	//扫描线的范围
	int ystart = (int)ceil(p0.y);
	int yend = (int)ceil(p1.y) - 1;

	//初始值
	//左边线
	float x_left = p0.x + (ystart - p0.y) * dxdyl;
	float one_over_z_left = 1.0f / p0.w;
	float u_over_z_left = uv_p0.x / p0.w;
	float v_over_z_left = uv_p0.y / p0.w;
	//右边线
	float x_right = p0.x + (ystart - p0.y) * dxdyr;
	float one_over_z_right = 1.0f / p0.w;
	float u_over_z_right = uv_p0.x / p0.w;
	float v_over_z_right = uv_p0.y / p0.w;

	//绘制三角形
	for (int y = ystart; y <= yend; y++) {
		int xstart = (int)ceil(x_left);
		int xend = (int)ceil(x_right) - 1;

		float ddz_step = (one_over_z_right - one_over_z_left) / (float)(xend - xstart);
		float ddu_step = (u_over_z_right - u_over_z_left) / (float)(xend - xstart);
		float ddv_step = (v_over_z_right - v_over_z_left) / (float)(xend - xstart);
		float ddz = one_over_z_left;
		float ddu = u_over_z_left;
		float ddv = v_over_z_left;

		//绘制一条扫描线
		for (int x = xstart; x <= xend; x++) {
			float u = ddu / ddz;
			float v = ddv / ddz;

			int _index = (y - 1) * WindowFrame::rect_client.right + x;

			if (_index < WindowFrame::rect_client.right * WindowFrame::rect_client.bottom && _index >= 0) {
				float &_z = m_z_depth_buffer[_index];
				if (ddz > _z) {
					DrawPixel(x, y, GetPixel(*texture, (int)u, (int)v));
					_z = ddz;
				}
			}
			ddz += ddz_step;
			ddu += ddu_step;
			ddv += ddv_step;
		}
		//绘制每条扫描线后，数据加上变化量
		x_left += dxdyl;
		one_over_z_left += d_one_over_z_dyl;
		u_over_z_left += d_u_over_z_dyl;
		v_over_z_left += d_v_over_z_dyl;

		x_right += dxdyr;
		one_over_z_right += d_one_over_z_dyr;
		u_over_z_right += d_u_over_z_dyr;
		v_over_z_right += d_v_over_z_dyr;
	}
}

/////////////////
// Draw Lines //
/////////////////
void Render::DrawTriangles(const vector<Fragment> &fg)
{
	//Draw every face of that object
	for (auto item : fg) {
		//如果已被删除
		if (item.state & FRAGMENT_DELETED) {
			continue;
		}
		DrawTriangle(item.trans_vList[0], item.trans_vList[1], item.trans_vList[2], COLOR_BLACK);
	}
}

void Render::DrawTriangle(const Vector4 p0, const Vector4 p1, const Vector4 p2, COLORREF color)
{
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
	BOOL steep = abs(p1.y - p0.y) > abs(p1.x - p0.x);
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
	int dy = (int)abs(p1.y - p0.y);

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
