#include "XMLLoader.h"

using namespace jeff_XML;

XMLLoader::XMLLoader() {
	is_open = false;
	root.type = TYPE_EMPTY;
}

XMLLoader::XMLLoader(string _file) :XMLLoader() {
	fileName = _file;
	if (!loadFile(fileName)) {
		std::cerr << "Load file failed" << endl;
		fileStream.close();
		fileName = "";
		is_open = false;
	}
	else {
		is_open = true;
	}
}

XMLLoader::~XMLLoader() {
	if (is_open) {
		fileStream.close();
	}
}

bool XMLLoader::loadFile(string fileName) {
	fileStream.open(fileName);
	is_open = true;

	if (!fileStream.is_open()) {
		is_open = false;
		return false;
	}
	analyse();
	return true;
}

void XMLLoader::print(Object root, int indent) {
	string str_indent;
	for (int lop = 0; lop < indent; lop++) {
		str_indent += "    ";
	}

	//叶子结点
	if (root.getStoreType() == TYPE_VALUE) {
		cout << str_indent << "<" << root.tag << ">" << "\n";
		cout << str_indent << "    " << root.getValue() << "\n";
		cout << str_indent << "</" << root.tag << ">" << endl;
	}
	//下面还有结点
	else {
		cout << str_indent << "<" << root.tag << ">" << "\n";
		for (auto &item : root.getList()) {
			print(item, indent + 1);
		}
		cout << str_indent << "</" << root.tag << ">" << "\n";
	}
}

XMLLoader& XMLLoader::print() {
	if (!is_open) {
		std::cerr << "File not open" << endl;
		return *this;
	}

	if (root.type == TYPE_EMPTY) {
		std::cerr << "File is empty ?" << endl;
	}

	//树的遍历
	print(root, 0);
	return *this;
}

const Object & XMLLoader::getRoot() {
	if (root.getType() == TYPE_EMPTY) {
		std::cerr << "Root is empty !" << endl;
	}
	return root;
}

//搜索
//输入标签的内容
vector<Object> XMLLoader::operator[](string index) {
	vector<Object> result;
	queue<Object> q;
	q.push(root);

	while (!q.empty()) {
		auto top = q.front();
		q.pop();

		//找到了
		if (top.tag == index) {
			result.push_back(top);
		}

		//如果不是叶子结点
		if (top.getStoreType() == TYPE_OBJECT) {
			auto tmp = top.getList();
			for (auto &each : tmp) {
				q.push(each);
			}
		}
	}

	return result;
}

void XMLLoader::clearSpace(string &str) {
	if (str.size() == 0) {
		return;
	}

	//开头和结尾都没空格
	if (str[0] != ' ' && str[str.size() - 1] != ' ') {
		return;
	}

	//清除开头的空格
	auto cur = str.begin();
	while (cur != str.end() && *cur == ' ') {
		cur++;
	}
	str.erase(str.begin(), cur);

	//如果清完了
	if (str.size() == 0) {
		return;
	}

	//清除结尾的空格
	cur = str.end() - 1;
	while (*cur == ' ') {
		cur--;
	}
	cur++;
	str.erase(cur, str.end());
}

//是尖括号括起来的 开头 返回1 结尾 返回0
//数据
ANALYSE_RESULT XMLLoader::analyseString(string str) {
	clearSpace(str);

	ANALYSE_RESULT result;
	double possibleNumber = 0;

	if (str.size() == 0) {
		result.type = ST_EMPTY;
		return result;
	}

	//是标签
	if (str[0] == '<') {
		//是结束标签
		if (str[1] == '/') {
			result.type = ST_END;
		}
		else {
			result.type = ST_START;
		}
	}
	//是数据的话
	else {
		//todo
		//匹配小数
		regex r("((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?");
		if (regex_match(str, r)) {
			result.type = ST_VALUE_NUMBER;
			stringstream ss(str);
			ss >> possibleNumber;
		}
		else {
			result.type = ST_VALUE_STRING;
		}
	}

	switch (result.type) {
		//</xxx>
	case ST_END:
		result.tag = str.substr(2, str.size() - 3);
		break;
		//<xxx>
	case ST_START:
		result.tag = str.substr(1, str.size() - 2);
		break;
	case ST_VALUE_STRING:
		result.value.set(str);
		break;
	case ST_VALUE_NUMBER:
		result.value.set(possibleNumber);
		break;
	default:
		break;
	}

	return result;
}

void XMLLoader::analyse() {
	//跳过文件的第一行
	bool skipFirstLine = false;

	//用于储存数据的堆栈
	stack<Object> s;

	Object lastPop;
	lastPop.type = TYPE_EMPTY;

	//读取到文件结束
	while (!fileStream.eof()) {
		string currentLine;
		getline(fileStream, currentLine);
		if (!skipFirstLine) {
			skipFirstLine = true;
			continue;
		}

		//分析字符串
		auto result = analyseString(currentLine);
		//cout << result << endl;

		//栈操作
		switch (result.type) {
		case ST_START:
			s.push(Object(result.tag));
			break;
		case ST_END:
			lastPop = s.top();
			s.pop();
			//为空说明已经分析完了
			if (!s.empty()) {
				s.top().set(lastPop);
			}
			break;
		case ST_VALUE_NUMBER:
		case ST_VALUE_STRING:
			s.top().set(result.value);
			break;
		case ST_EMPTY:
			break;
		default:
			break;
		}
	}
	root = lastPop;
	assert(s.empty());
}
