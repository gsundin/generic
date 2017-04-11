#include "matrix.h"
#include "regex.h"
#include "regex.cpp"
#include "scanner.cpp"
#include <iostream>
#include <fstream>

using namespace std;

Matrix::Matrix(int x, int y){
	rows = x;
	cols = y;
	data = new float * [x];
	for(int i=0; i < x; i++){
		data[x] = new float[i];
	}

}