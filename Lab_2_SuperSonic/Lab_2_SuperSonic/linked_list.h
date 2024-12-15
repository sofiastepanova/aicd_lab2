#pragma once
#include <iostream>
#include <random>
#include <stdexcept>

using namespace std;

template<typename T>
struct Node {
    T data;
    Node<T>* next, * prev;

    Node(T data) : data(data), next(nullptr), prev(nullptr) {}
};

template<typename T>
class LinkedList {
    Node<T>* _head, * _tail;
    size_t _size;

public:
    LinkedList() : _head(nullptr), _tail(nullptr), _size(0) {}

    LinkedList(const LinkedList& other) : LinkedList() {
        Node<T>* tmp = other._head;
        while (tmp)
        {
            push_tail(tmp->data);
            tmp = tmp->next;
        }
    }

    Node<T>* get_head() const {
        return _head;
    }

    Node<T>*& get_head() {
        return _head;
    }

    Node<T>* get_tail() const {
        return _tail;
    }

    Node<T>*& get_tail() {
        return _tail;
    }

    LinkedList(size_t size, T lower_bound, T upper_bound, unsigned seed) : LinkedList() {
        std::random_device rd;
        std::mt19937 gen(seed);
        if constexpr (std::is_integral_v<T>) {
            std::uniform_int_distribution<T> dist(lower_bound, upper_bound);
            for (size_t i = 0; i < size; ++i) {
                T data = dist(gen);
                push_tail(data);
            }
            _size = size;
        }
        else if constexpr (std::is_floating_point_v<T>) {
            std::uniform_real_distribution<T> dist(lower_bound, upper_bound);
            for (size_t i = 0; i < size; ++i) {
                T data = dist(gen);
                push_tail(data);
            }
            _size = size;
        }
    }

    ~LinkedList() {
        clear();
    }

    LinkedList<T>& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            Node<T>* tmp = other._head;
            while (tmp) {
                push_tail(tmp->data);
                tmp = tmp->next;
            }
        }
        return *this;
    }

    Node<T>* push_head(T data) {
        Node<T>* tmp = new Node<T>(data);
        tmp->next = _head;
        if (_head)
            _head->prev = tmp;
        if (!_tail)
            _tail = tmp;
        _head = tmp;
        _size++;
        return tmp;
    }

    void push_head(const LinkedList<T>& other) {
        Node<T>* tmp = other._tail;
        while (tmp) {
            push_head(tmp->data);
            tmp = tmp->prev;
        }
    }

    Node<T>* push_tail(T data) {
        Node<T>* tmp = new Node<T>(data);
        tmp->prev = _tail;
        if (_tail)
            _tail->next = tmp;
        if (!_head)
            _head = tmp;
        _tail = tmp;
        _size++;
        return tmp;
    }

    void push_tail(const LinkedList& other) {
        Node<T>* tmp = other._head;
        while (tmp) {
            push_tail(tmp->data);
            tmp = tmp->next;
        }
    }

    void pop_head() {
        if (!_head)
            throw std::invalid_argument("the list is empty");
        Node<T>* tmp = _head->next;
        if (tmp) tmp->prev = nullptr;
        else _tail = nullptr;
        delete _head;
        _head = tmp;
        _size--;
    }

    void pop_tail() {
        if (!_tail)
            throw std::invalid_argument("the list is empty");
        Node<T>* tmp = _tail->prev;
        if (tmp) tmp->next = nullptr;
        else _head = nullptr;
        delete _tail;
        _tail = tmp;
        _size--;
    }

    Node<T>* erase(int index) {
        Node<T>* tmp = get_at(index);
        if (!tmp)
            throw std::invalid_argument("there is no value for this index");
        if (!tmp->prev) {
            pop_head();
            return tmp;
        }

        if (!tmp->next) {
            pop_tail();
            return tmp;
        }
        Node<T>* left = tmp->prev;
        Node<T>* right = tmp->next;
        left->next = right;
        right->prev = left;
        _size--;
        delete tmp;

        return right;
    }

        void delete_node(T data) {
        Node<T>* tmp = _head;
        int index = 0;
        while (tmp != nullptr) {
            if (tmp->data == data) {
                tmp = erase(index);
                continue;
            }
            index++;
            tmp = tmp->next;
        }
    }

    Node<T>* get_at(int index) {
        if (index >= _size)
            throw std::invalid_argument("invalid index");
        Node<T>* tmp = _head;
        int n = 0;
        while (n != index) {
            if (!tmp)
                return tmp;
            tmp = tmp->next;
            n++;
        }
        return tmp;
    }

    const T& operator[](int index) const {
        return get_at(index)->data;
    }

    Node<T>* insert(int index, T data) {
        Node<T>* right = get_at(index);
        if (!right)
            return push_tail(data);
        Node<T>* left = right->prev;
        if (!left)
            return push_head(data);
        Node<T>* tmp = new Node<T>(data);
        left->next = tmp;
        right->prev = tmp;
        tmp->prev = left;
        tmp->next = right;
        _size++;
        return tmp;
    }

    T& operator[](int index) {
        return get_at(index)->data;
    }

    void clear() {
        while (_head) {
            pop_head();
        }
        _size = 0;
    }

    size_t get_size() const {
        return _size;
    }
};

template<typename T>
void reverse(LinkedList<T>& list) {
    Node<T>* current = list.get_head();
    Node<T>* tmp = nullptr;

    while (current) {
        tmp = current->prev;
        current->prev = current->next;
        current->next = tmp;

        current = current->prev;
    }

    if (tmp) {
        list.get_tail() = list.get_head();
        list.get_head() = tmp->prev;
    }
}

template<typename T>
ostream& operator<<(ostream& os, const LinkedList<T>& list) {
    Node<T>* tmp = list.get_head();
    os << "[ ";
    while (tmp) {
        os << tmp->data;
        if (tmp->next)
            os << ", ";
        tmp = tmp->next;
    }
    os << " ]";
    return os;
}
