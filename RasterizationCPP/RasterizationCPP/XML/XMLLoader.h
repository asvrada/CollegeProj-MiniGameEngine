#ifndef XMLLOADER_H
#define XMLLOADER_H

#include "../ProjectHeader.h"

#include <regex>
using std::regex;
using std::regex_match;

#include <stack>
using std::stack;

namespace jeff_XML {

	//结点会有几种类型
	enum TYPE {
		TYPE_EMPTY,
		TYPE_BASE,
		TYPE_NUMBER,
		TYPE_STRING,
		TYPE_VALUE,
		TYPE_OBJECT
	};

	//用于分析当前行里是什么
	enum STRINGTYPE {
		ST_EMPTY,
		ST_START,
		ST_END,
		ST_VALUE_STRING,
		ST_VALUE_NUMBER
	};

	//基类
	//方便传递数据
	class Base {
	public:
		TYPE type;

		Base() {
			type = TYPE_BASE;
		}

		virtual TYPE getType() {
			return TYPE_BASE;
		}

		//虚函数
		//重载输出流
		friend ostream& operator << (ostream& os, Base& t) {
			os << "TYPE_BASE";
			return os;
		}
	};

	//储存数字
	class Number :public Base {
	public:
		double val;

		Number() {
			type = TYPE_NUMBER;
			val = 0;
		}
		//构造函数
		Number(double _val) :Number() {
			val = _val;
		}

		TYPE getType() {
			return type;
		}

		//重载输出流
		friend ostream& operator << (ostream& os, Number& t) {
			os << "[TYPE_NUMBER] " << t.val;
			return os;
		}
	};

	//储存字符串
	class String :public Base {
	public:
		string val;

		String() {
			type = TYPE_STRING;
		}
		//构造函数
		String(string _val) :String() {
			val = _val;
		}

		TYPE getType() {
			return type;
		}

		//重载输出流
		friend ostream& operator << (ostream& os, String& t) {
			os << "[TYPE_STRING] " << t.val;
			return os;
		}

	};

	//储存一个Number或者String
	class Value : public Base {
	private:
		shared_ptr<Base> val;

	public:
		Value() {
			type = TYPE_EMPTY;
		}

		Value(double _val) :Value() {
			type = TYPE_NUMBER;
			val = make_shared<Number>(_val);
		}

		Value(string _val) :Value() {
			type = TYPE_STRING;
			val = make_shared<String>(_val);
		}

		TYPE getType() {
			return type;
		}

		//get set
		//获取数据
		Number* const getNumber() {
			assert(type == TYPE_NUMBER);
			return PTR_CONVERT(val, Number);
		}

		String* const getString() {
			assert(type == TYPE_STRING);
			return PTR_CONVERT(val, String);
		}

		void set(double _val) {
			if (type != TYPE_NUMBER) {
				type = TYPE_NUMBER;
				val = make_shared<Number>(_val);
			}
			else {
				PTR_CONVERT(val, Number)->val = _val;
			}
		}

		void set(string _val) {
			if (type != TYPE_STRING) {
				type = TYPE_STRING;
				val = make_shared<String>(_val);
			}
			else {
				PTR_CONVERT(val, String)->val = _val;
			}
		}

		//重载输出流
		friend ostream& operator << (ostream& os, Value& t) {
			switch (t.type) {
			case TYPE_NUMBER:
				os << *t.getNumber();
				break;
			case TYPE_STRING:
				os << *t.getString();
				break;
			case TYPE_EMPTY:
				os << "Nothing stored yet";
				break;
			default:
				break;
			}
			return os;
		}
	};

	//每一对尖括号就是一个Object
	//可得一个object里面可能是
	//A. 数个object
	//B. 数据
	class Object : public Base {
	public:
		string tag;

	private:
		//true表明储存数据
		//false表明储存其他object
		TYPE storedType;
		Value val;
		vector<Object> list;

	public:
		//构造函数
		Object() {
			type = TYPE_OBJECT;
			storedType = TYPE_EMPTY;
		}

		Object(string _name) :Object() {
			tag = _name;
		}

		//判断当前object存储的类型
		TYPE getType() {
			return type;
		}

		TYPE getStoreType() {
			return storedType;
		}

		//存值
		void set(Value& t) {
			storedType = TYPE_VALUE;
			val = t;
		}
		void set(Object& t) {
			//如果原先储存的不是object
			if (storedType != TYPE_OBJECT) {
				storedType = TYPE_OBJECT;
				list.clear();
			}
			list.push_back(t);
		}

		//取值
		Value& getValue() {
			assert(storedType == TYPE_VALUE);
			return val;
		}
		vector<Object>& getList() {
			assert(storedType == TYPE_OBJECT);
			return list;
		}

		//递归查找tag
		int findTag(string target_tag, jeff_XML::Object &out) {
			if (!(storedType == TYPE_OBJECT || storedType == TYPE_VALUE)) {
				return ERROR;
			}

			if (tag == target_tag) {
				out = *this;
				return OK;
			}

			//递归查找
			for (auto &each : list) {
				if (each.findTag(target_tag, out) == OK) {
					return OK;
				}
			}

			return ERROR;
		}
	};

	//当前行要么是标签（用字符串返回
	//要么是数据
	struct ANALYSE_RESULT {
		STRINGTYPE type;

		string tag;
		Value value;
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

		//分析得到的树根
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
		XMLLoader& print();

		//返回根
		const Object& getRoot();

		vector<Object> operator [] (string index);

	//辅助函数
	private:
		//树的遍历
		void print(Object root, int indent);

		//清除字符串开头和结尾的空格
		void clearSpace(string&);

		//分析读取的当前行是什么
		ANALYSE_RESULT analyseString(string);

		//主要函数
		//分析文件
		void analyse();
	};
}

#endif