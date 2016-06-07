#include "XMLLoader.h"

int main() {
	XMLLoader loader;
	loader.loadFile("sample.xml");
	loader.read();
	return 0;
}