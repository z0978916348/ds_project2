#include <iostream>  
using namespace std;

template <typename T>
class node {
public:
    T data;
    node<T>* next;
};  

template <typename T>
class stack {
private:
    node<T>* top;
public:
    stack() {
        top = nullptr;
}
bool isEmpty() {
    if (top == nullptr)
        return true;
    else
        return false;
}
T Top() {
    if (top)
        return top->data;
    else
        throw "Empty";
        //cout << "stack is empty" << endl;
}
void push(const T & val) {
    if (top == nullptr) {
        top = new node<T>;
        top->next = nullptr;
        top->data = val;
    }
    else {
        node<T>* temp = new node<T>;
        temp->data = val;
        temp->next = top;
        top = temp;
    }
}
void pop() {
    if (top == nullptr) {
        cout << "stack is empty" << endl;
        return;
    }
    else {
        node<T>* temp = top;
        top = top->next;
        delete temp;
    }
}
void print() {
    node<T>* temp = top;
    while (temp != nullptr)
    {
        cout << temp->data << " ";
        temp = temp->next;
    }
   }
};  
