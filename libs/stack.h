#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include "massive.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

using namespace std;
namespace fs = filesystem;

struct Stack {
    // Определение узла списка
    struct Node {
        string data;
        Node* next;
        Node(const string& value) : data(value), next(nullptr) {}
    };

    Node* top;

    Stack() : top(nullptr) {}

    ~Stack() {
        while (top != nullptr) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
    }

    void push(const string& value) {
        Node* newNode = new Node(value);
        newNode->next = top;
        top = newNode;
    }

    void del() {
        if (top != nullptr) {
            Node* temp = top;
            top = top->next;
            delete temp;
        }
    }

    void saveToFile(const string& filename) {
        ofstream file(filename);
        Node* current = top;
        while (current != nullptr) {
            file << current->data;
            if (current->next != nullptr) {
                file << ";";
            }
            current = current->next;
        }
        file.close();
    }
};

void stack(string fileName, string actions);
bool emptyFile (string fileName);

#endif // STACK_H_INCLUDED