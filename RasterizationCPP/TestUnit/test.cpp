#include <iostream>

#include "MathComplement.h"

using namespace std;

int main() {
	Matrix4 a(2.0f), b(2.0f);
	a.Invert();
	a = a*b;
	return 0;
}