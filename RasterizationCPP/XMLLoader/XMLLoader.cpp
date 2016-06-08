#include "XMLLoader.h"

XMLLoader::XMLLoader() {
	is_open = false;
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

XMLLoader& XMLLoader::read() {
	if (!is_open) {
		std::cerr << "File not open" << endl;
		return *this;
	}

	//todo
	return *this;
}

void XMLLoader::clearSpace(string &str) {
	if (str[0] != ' ') {
		return;
	}
	int lop = 0;
	for (; lop < str.size(); lop++) {
		if (str[lop] != ' ') {
			break;
		}
	}
	str.erase(0, lop);
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
	//用于储存数据的堆栈
	stack<Object> s;

	//跳过文件的第一行
	bool skipFirstLine = false;

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
		cout << result << endl;


		//todo
		switch (result.type) {
		case ST_START:
			break;
		case ST_END:
			break;
		case ST_VALUE:
			break;
		case ST_EMPTY:
			break;
		default:
			break;
		}
	}
}
