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
		return;
	}

	//todo
	return *this;
}

void XMLLoader::analyse() {

}
