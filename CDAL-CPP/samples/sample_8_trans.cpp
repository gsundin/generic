#include <iostream>
#include "Matrix.h"
#include <math.h>
using namespace std;
int main ( ) {
    matrix m ( matrix::matrixRead ("../samples/sample_8.data") ) ;
    cout << m ;
}

