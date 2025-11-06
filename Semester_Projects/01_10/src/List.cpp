#include "List.h"
#include <iostream>

using namespace std;

VariableNode* create_new_variable_node(const char* name, int value, int m) {
    VariableNode* new_node = new VariableNode;
  
    if (strlen(name) > m) {
         cerr << "Err: Identifier name too long for m = " << m << endl;
         delete new_node;
         return nullptr;
    }
    
    strncpy(new_node->name, name, m);
    new_node->name[m] = '\0';
    new_node->value = value;
    new_node->next = nullptr;
    return new_node;
}

VariableNode* find_variable(VariableNode* top, const char* ident_str) {
    VariableNode* current = top;
    while (current != nullptr) {
        if (strcmp(current->name, ident_str) == 0) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

void set_variable_value(VariableNode*& top, const char* ident_str, int value, int m) {
    VariableNode* existing_node = find_variable(top, ident_str);
    
    if (existing_node != nullptr) {
        existing_node->value = value;
        return;
    }

    VariableNode* new_node = create_new_variable_node(ident_str, value, m);
    
    if (new_node != nullptr) {
    if (top == nullptr) {
        // 1. Список пуст: new_node становится головой списка
        top = new_node;
    } else {
        // 2. Список не пуст: ищем последний узел
        VariableNode* cur = top;
        // Цикл находит последний узел, у которого next == nullptr
        for (; cur->next != nullptr; cur = cur->next) {} 
        // Присоединяем новый узел к концу
        cur->next = new_node;
    }
}
}

int get_variable_value(VariableNode*& top, const char* ident_str, int m) {
    VariableNode* existing_node = find_variable(top, ident_str);

    if (existing_node != nullptr) {
        return existing_node->value;
    }

    int new_value;
    cout << "Variable " << ident_str << " not defined. Enter its value: ";
    
    if (!(cin >> new_value)) {
        cerr << "Input for " << ident_str << " failed. Defaulting to 0." << endl;
        cin.clear();
        while (cin.get() != '\n'); 
        new_value = 0;
    }
    set_variable_value(top, ident_str, new_value, m); 
    
    return new_value;
}

void clear_variable_list(VariableNode* top) {
    VariableNode* current = top;
    VariableNode* next_node = nullptr;

    while (current != nullptr) {
        next_node = current->next;
        delete current;
        current = next_node; 
    }
}