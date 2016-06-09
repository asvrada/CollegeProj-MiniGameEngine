#include "XMLLoader.h"

int main() {
	XMLLoader loader;
	loader.loadFile("sample.xml");
	loader.print();
	return 0;
}