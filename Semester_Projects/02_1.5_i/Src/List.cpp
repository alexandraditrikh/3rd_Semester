#include "List.h"

Path::Path(int initial_id) : top(nullptr), tail(nullptr) {
    Node* initialNode = new Node(initial_id);

    top = tail = initialNode;
    
    initialNode->next = initialNode;
    initialNode->prev = initialNode;
}

void Path::append_node(int new_id) {
    if (!top) {
        Node* initialNode = new Node(new_id);
        top = tail = initialNode;
        initialNode->next = initialNode;
        initialNode->prev = initialNode;
        return;
    }

    Node* newNode = new Node(new_id, tail, top); 

    tail->next = newNode;
    top->prev = newNode;

    tail = newNode;
}

void Path::insert_node_after (int m_id, int l_id) {
    if (!top) return;

    Node* new_node = new Node(l_id);
    Node* cur = top;
    do {
        if (cur->id == m_id) {
            new_node->prev = cur;
            new_node->next = cur->next;

            cur->next->prev = new_node;
            cur->next = new_node;

            if (cur == tail) {
                tail = new_node; 
            }

            return;
        }
        cur = cur->next;
    } while (cur != top);
}

Path::~Path() {
    if (!top) return;

    Node* current = top->next;
    Node* temp;

    tail->next = nullptr; 
    top->prev = nullptr;

    while (current != nullptr) {
        temp = current;
        current = current->next;
        delete temp;
    }
    delete top;
    top = nullptr;
    tail = nullptr;
}