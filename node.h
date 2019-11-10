#ifndef SPARSE_MATRIX_NODE_H
#define SPARSE_MATRIX_NODE_H

template <typename T>
class Matrix;

template <typename T>
class Node {
protected:
    Node<T> *next, *down;
    T data;
    unsigned row;
    unsigned col;
public:
    explicit Node(): data{0}, row{0}, col{0}, next{nullptr}, down{nullptr} {}
//    explicit Node(T data) : data{data}, row{0}, col{0}, next{nullptr}, down{nullptr} {}
    Node(unsigned row, unsigned col, T data) : data{data}, row{row}, col{col}, next{nullptr}, down{nullptr} {}

    void killSelf(){
        if (this->next) {
            this->next->killSelf();
        }
        delete this;
    }

    ~Node() = default;

    friend class Matrix<T>;
};

#endif //SPARSE_MATRIX_NODE_H
