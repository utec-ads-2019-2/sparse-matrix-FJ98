#ifndef SPARSE_MATRIX_MATRIX_H
#define SPARSE_MATRIX_MATRIX_H

#include "node.h"
#include <stdexcept>

using namespace std;

#define EMPTY 0

template <typename T>
class Matrix {
private:
    Node<T> *root;
    unsigned rows, columns;

private:
    Node<T> **findNodeByRow(unsigned row, int column) {
        auto currentNode = &this->root;

        for(int i = 0; i <= static_cast<int>(row); ++i){
            currentNode = &((*currentNode)->down);
        }

        currentNode = &((*currentNode)->next);
        while(*currentNode && (*currentNode)->col < column){
            currentNode = &((*currentNode)->next);
        }

        return currentNode;
    }

    Node<T> **findNodeByColumn(unsigned row, int column) {
        auto currentNode = &this->root;

        for(int i = 0; i <= static_cast<int>(column); ++i){
            currentNode = &((*currentNode)->next);
        }

        currentNode = &((*currentNode)->down);
        while(*currentNode && (*currentNode)->row < row){
            currentNode = &((*currentNode)->down);
        }

        return currentNode;
    }

    void updateRow(unsigned row, unsigned column, T data){
        auto newNode = new Node<T>(row, column, data);
        auto rowNode = this->findNodeByRow(row, column);

        if(*rowNode && (*rowNode)->col == column){
            if(data){
                (*rowNode)->data = data;
                return;
            }
        }
        else if(data != EMPTY){
            newNode->next = *rowNode;
            *rowNode = newNode;
        }
        else{ return; }
    }

    void updateColumn(unsigned row, unsigned column, T data){
        auto newNode = new Node<T>(row, column, data);
        auto columnNode = this->findNodeByColumn(row, column);

        if(*columnNode && (*columnNode)->row == row){
            delete *columnNode;
            return;
        }
        newNode->down = *columnNode;
        *columnNode = newNode;
    }

public:
    Matrix(): rows{0}, columns{0}, root{nullptr} {}
    Matrix(unsigned rows, unsigned columns) : rows{rows}, columns{columns}, root{nullptr} {
        if (rows <= 0 || columns <= 0) { throw out_of_range("Can't build a matrix with negative or zero dimension"); }

        this->root = new Node<T>;

        // rows
        auto newNode = this->root;
        for (int i = 0; i < static_cast<int>(rows); ++i) {
            newNode->down = new Node<T>;
            newNode = newNode->down;
        }

        // columns
        newNode = this->root;
        for (int j = 0; j < static_cast<int>(columns); ++j) {
            newNode->next = new Node<T>;
            newNode = newNode->next;
        }
    }

    void set(unsigned row, unsigned column, T data) {
        if ( (row < 0 || row >= this->rows) || (column < 0 || column >= this->columns) ) {
            throw out_of_range("Position entered is out of the matrix");
        }
        this->updateRow(row, column, data);
        this->updateColumn(row, column, data);
    }


    T operator()(unsigned row, unsigned column) const {
        if ( (row < 0 || row >= this->rows) || (column < 0 || column >= this->columns) ) {
            throw out_of_range("Position is out of the matrix");
        }
        // row
        auto currentNode = this->root;
        for (int i = 0; i <= static_cast<int>(row); ++i) {
            currentNode = currentNode->down;
        }

        // column
        currentNode = currentNode->next;
        while (currentNode && currentNode->col < column) {
            currentNode = currentNode->next;
        }

        if (!currentNode || currentNode->col > column) { return EMPTY; }

        return currentNode->data;
    }


    Matrix<T> operator*(T scalar) const {
        auto resultantMatrix = Matrix<T>{this->rows, this->columns};

        for (int i = 0; i < this->rows; ++i)
        {
            for (int j = 0; j < this->columns; ++j) {
                resultantMatrix.set(i, j, (*this)(i, j) * scalar);
            }
        }

        return resultantMatrix;
    }

    Matrix<T>& operator*(const Matrix<T>& other) const {
        if (this->columns != other.rows) {
            throw out_of_range("Columns of the first matrix must be equal to rows of the second matrix");
        }

        auto resultantMatrix = new Matrix<T>{this->rows, other.columns};
        for (int i = 0; i < this->rows; ++i)
        {
            for (int j = 0; j < other.columns; ++j)
            {
                auto value = 0;
                for (int k = 0; k < this->columns; ++k) {
                    value +=  (*this)(i, k) * other(k, j);
                }
                if (value) { /* To avoid complete the matrix with zeros */
                    resultantMatrix->set(i, j, value);
                }
            }
        }
        return *resultantMatrix;
    }

    // Tus operaciones serán un poco lentas porque usas los métodos de la matriz en vez de navegar con los punteros

    Matrix<T>& operator+(const Matrix<T>& other) const {
        if (this->rows != other.rows || this->columns != other.columns) { throw out_of_range("Matrices dimensions must be equals");}

        auto resultantMatrix = new Matrix<T>{this->rows, this->columns};
        for (int i = 0; i < this->rows; ++i)
        {
            for (int j = 0; j < this->columns; ++j)
            {
                if ((*this)(i, j) + other(i, j)) { /* To avoid complete the matrix with zeros */
                    resultantMatrix->set(i, j, (*this)(i, j) + other(i, j));
                }
            }
        }
        return *resultantMatrix;
    }

    Matrix<T>& operator-(const Matrix<T>& other) const {
        auto resultantMatrix =  new Matrix<T>{this->rows, this->columns};
        for (int i = 0; i < this->rows; ++i)
        {
            for (int j = 0; j < this->columns; ++j)
            {
                if ((*this)(i, j) - other(i, j)) { /* To avoid complete the matrix with zeros */
                    resultantMatrix->set(i, j, (*this)(i, j) - other(i, j));
                }
            }
        }
        return *resultantMatrix;
    }

    Matrix<T>& transpose() const {
        auto transposeMatrix = new Matrix<T>{this->rows, this->columns};
        for (int i = 0; i < this->rows; ++i)
        {
            for (int j = 0; j < this->columns; ++j)
            {
                if ((*this)(i, j)) { /* To avoid complete th matrix with zeros */
                    transposeMatrix->set(j, i, (*this)(i, j));
                }
            }
        }
        return *transposeMatrix;
    }

    void print() const {
        for (int i = 0; i < this->rows; ++i)
        {
            for (int j = 0; j < this->columns; ++j) {
                std::cout << (*this)(i, j) << " ";
            }
            std::cout << std::endl;
        }
        cout << endl << endl;
    }

    //----------------ACCESSES-------------------
    unsigned rowsSize() { return this->rows; }
    unsigned columnsSize() { return this->columns; }
    //----------------------------------------------

    ~Matrix(){
        if (!this->root) { return; }
        this->root->killSelf();
    }
};

#endif //SPARSE_MATRIX_MATRIX_H