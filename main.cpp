#include "tester/tester.h"
#include "matrix.h"

int main() {
    Tester::execute();
    cout << "===============" << endl;
    cout << "All test passed" << endl;
    cout << "===============" << endl;

/*-------------BEGIN-OF-TESTING-MATRIX-MULTIPLICATION----------------*/
    int r = 4, col = 4;
    Matrix<int> testMatrix1(r, col), testMatrix2(col, r);
    cout << "Matrix 1 dimensions: " << testMatrix1.rowsSize() << " " << testMatrix1.columnsSize() << endl;
    cout << "Matrix 2 dimensions: " << testMatrix2.rowsSize() << " " << testMatrix2.columnsSize() << endl << endl;
    for(int i = 0; i < r; i++){
        for(int j = 0; j < col; j++){
            testMatrix1.set(i, j, rand() % 10);
            testMatrix2.set(j, i, rand() % 10);
        }
    }

    cout<<"Matrix 1: "<<endl;
    testMatrix1.print();

    cout<<"Matrix 2:"<<endl;
    testMatrix2.print();

    Matrix<int> testMatrixMultiplication = testMatrix1 * testMatrix2;
    cout<<"Matrix3 : Matrix 1 * Matrix 2:"<<endl;
    testMatrixMultiplication.print();

    cout<<"Matrix 3 transpose:"<<endl;
    auto testMatrixTranpose = testMatrixMultiplication.transpose();
    testMatrixTranpose.print();
/*----------------END-OF-TESTING-MATRIX-MULTIPLICATION----------------*/
    return EXIT_SUCCESS;
}

