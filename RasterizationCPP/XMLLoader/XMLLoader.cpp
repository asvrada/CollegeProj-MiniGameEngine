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
	cout << str << "           --Current Line" << endl;

	ANALYSE_RESULT result;
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
		//regex 
	}

	return result;
}

void XMLLoader::analyse() {
	while (!fileStream.eof()) {
		string currentLine;
		getline(fileStream, currentLine);

		analyseString(currentLine);
	}
}
