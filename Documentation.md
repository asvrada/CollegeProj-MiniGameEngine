#项目介绍
暂无，没啥好写的，最重要的功能都还没做呢
#定义介绍
#####坐标系：  
左手坐标系，即 x 轴向右，y 轴向上，z 轴向屏幕里  
![Left Hand Coord](http://139.129.34.47/reference/lefthandcoord.jpg)  

#####旋转坐标：  
从主轴相反方向望去的顺时针方向为 ___正___  
对应的坐标轴  
x : Pitch  
y : Yaw  
z : Roll  

#####物体基本元素：三角形  
三角形顶点缠绕方向：___顺时针___ 为正面朝向，也即该面的法线方向  

#####顶点索引表
软件中的模型采用顶点索引表的方式进行储存与表示  
相关数据在文件中的储存格式如下，每行开头无缩进无空格
	
	#以井号开头为注释
	#v 开头后接一个点的 x y z 坐标
	v 0.0 0.0 0.0

	#f 开头后接一个面的顶点索引
	f 0 1 2


齐次剪裁空间 各坐标范围：  
X, Y, Z ~ [ -1, 1 ]
  
#命名规范

#####1.头文件的宏定义：
[ProjectName]\_[Directory]\_[FileName]\_[H]   
根目录下的代码不需要添加路径 

	示例:
	RASTERIZER_INPUTCLASS_H
	
#####2.类或结构体：  
每个单词首字母大写  
每个单词之间无下划线
	
	示例:
	class Input

#####3.函数名称：  
每个单词首字母大写  
每个单词之间无下划线

	示例:
	void UpdateCursorCenterPostion(const RECT&);

#####4.变量名称：  
全部小写  
单词之间有下划线  

	示例:
	bool is_center_snapped;

#####5.全局要求：
私有成员数据或者函数之前加 "m\_"  

	示例:
	KeyPressedList m_key_pressed;
	void m_Initial();

#类介绍
![All the Class](http://139.129.34.47/reference/ClassDiagram.png)