#ifndef XMLLOADER_H
#define XMLLOADER_H

#include <assert.h>

#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <stack>

using std::string;
using std::ifstream;
using std::cout;
using std::endl;
using std::map;
using std::vector;
using std::stack;

//储存数据
class Value {
private:
	//就两种
	//number和string
	string type;

	float valNumber;
	string valString;

public:
	Value() {}
	//构造函数
	Value(float _val) {
		type = "number";
		valNumber = _val;
	}
	Value(string _val) {
		type = "string";
		valString = _val;
	}

	string getType() {
		return type;
	}

	//取得数据
	string& getString() {
		assert(type == "string");
		return valString;
	}

	float& getNumber() {
		assert(type == "number");
		return valNumber;
	}

	//存放数据
	void store(float newData) {
		type = "number";
		valNumber = newData;
	}
	void store(string newData) {
		type = "string";
		valString = newData;
	}
};

//每一对尖括号就是一个Object
//可得一个object里面可能是
//A. 数个object
//B. 数据
class Object {
public:
	string name;

private:
	//true表明储存数据
	//false表明储存其他object
	bool isVal;
	Value val;
	vector<Object> list;

public:
	//构造函数
	Object() {
		isVal = true;
		val.store("");
	}

	//判断当前object存储的类型
	bool isVal() {
		return isVal;
	}

	//取值
	Value& getValue() {
		assert(isVal);
		return val;
	}
	vector<Object>& getList() {
		assert(!isVal);
		return list;
	}

};

class XMLLoader {
///////////
//成员变量//
//////////
public:
	string fileName;
	ifstream fileStream;
private:
	bool is_open;
	Object root;
///////////
//成员函数//
//////////
public:
	//构造函数
	XMLLoader();
	XMLLoader(string);

	~XMLLoader();

	//读取文件
	//读取成功，返回true
	//读取失败，返回false
	bool loadFile(string);

	//输出读取的文件内容
	XMLLoader& read();

private:
	//分析文件
	void analyse();
};

#endif