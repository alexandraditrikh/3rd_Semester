#pragma once

#include <stdexcept>

class Node {
public:
    int id; 
    Node* next;
    Node* prev;
    Node(int id, Node* prev_node = nullptr, Node* next_node = nullptr) 
        : id(id), prev(prev_node), next(next_node) {}
};

class Path {
public:
    Node* top; 
    Node* tail; 

    Path(int initial_id); 

    void append_node(int new_id);
    void insert_node_after (int m_id, int l_id);

    ~Path();
};