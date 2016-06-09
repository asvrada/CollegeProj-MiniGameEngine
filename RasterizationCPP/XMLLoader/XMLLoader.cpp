#include "XMLLoader.h"

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

void XMLLoader::read(Object root) {
	//叶子结点
	if (root.getStoreType() == TYPE_VALUE) {
		cout << "<" << root.tag << ">" << "\n";
		cout << root.getValue() << "\n";
		cout << "</" << root.tag << ">" << endl;
	}
	//下面还有结点
	else {
		cout << "<" << root.tag << ">" << "\n";
		for (auto &item : root.getList()) {
			read(item);
		}
		cout << "</" << root.tag << ">" << "\n";
	}
}

XMLLoader& XMLLoader::read() {
	if (!is_open) {
		std::cerr << "File not open" << endl;
		return *this;
	}

	if (root.type == TYPE_EMPTY) {
		std::cerr << "File is empty ?" << endl;
	}

	//树的遍历
	read(root);
	//todo
	return *this;
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
	while (*cur == ' ') {
		cur++;
	}
	str.erase(str.begin(), cur);

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
	//cout << str << "           --Current Line" << endl;

	ANALYSE_RESULT result;
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
		result.type = ST_VALUE;
		//todo
		//先假设全是字符串
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
	case ST_VALUE:
		result.value.set(str);
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

		//todo
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
		case ST_VALUE:
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
