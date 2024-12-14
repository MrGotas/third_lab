#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

using namespace std;
namespace fs = filesystem;

struct Queue {
    struct Node {
        string data;
        Node* next;
        Node* prev;
        Node(const string& value) : data(value), next(nullptr), prev(nullptr) {}
    };

    Node* head;
    Node* tail;

    Queue() : head(nullptr), tail(nullptr) {}

    ~Queue() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void push(const string& value) {
        Node* newNode = new Node(value);
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void del() {
        Node* temp = head;
        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        } else {
            tail = nullptr;
        }
        delete temp;
    }

    void saveToFile(const string& filename) {
        ofstream file(filename);
        Node* current = head;
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

void queue(string filename, string actions);
bool emptyFile (string fileName);

#endif // QUEUE_H_INCLUDED