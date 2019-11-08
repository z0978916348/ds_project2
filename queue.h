#include <iostream>

using namespace std;

template <typename T>
class queue {
public:
    struct node {
        T data;
        node *next;
        node(): data(0), next(nullptr) {}
        node(T const& data, node* next): data(data), next(next) {}
    };
    ~queue();
    bool empty() const;
    int size() const;
    T front() const;
    void push(T const& data);
    void pop();

private:
    int elements = 0;
    node *head = nullptr;
    node *tail = nullptr;
};

template <typename T>
queue<T>::~queue() {
    node *curr = new node();
    while(head) {
        curr = head;
        head = head->next;
        delete curr;
    }
    delete tail;
}

template <typename T>
bool queue<T>::empty() const {
    return elements == 0;
}

template <typename T>
int queue<T>::size() const {
    return elements;
}

template <typename T>
T queue<T>::front() const {
    if(elements == 0) {
        cout << "Queue is empty" << endl;
        return 0;
    }
    return head->data;
}

template <typename T>
void  queue<T>::push(T const& data) {
    node *newNode = new node(data, nullptr);
    if(!elements) head = newNode;
    else tail->next = newNode;
    tail = newNode;
    ++elements;
}

template <typename T>
void  queue<T>::pop() {
    if(elements == 0) {
        cout << "Queue is empty" << endl;
        return;
    }
    node *tmp = new node();
    if(elements != 0) tmp = head;
    head = head->next;
    --elements;
    delete tmp;
}