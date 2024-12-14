#ifndef LISTS_H_INCLUDED
#define LISTS_H_INCLUDED

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

using namespace std;
namespace fs = filesystem;

struct ListS {
    // Определение узла списка
    struct Node {
        string data;
        Node* next;
        Node(const string& value) : data(value), next(nullptr) {}
    };

    Node* head;
    Node* tail;

    ListS() : head(nullptr), tail(nullptr) {}

    ~ListS() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void pushh(const string& value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
        if (tail == nullptr) {
            tail = head;
        }
    }

    void pusht(const string& value) {
        Node* newNode = new Node(value);
        if (tail == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    void delh() {
        if (head == nullptr) {
            cout << "Список пуст." << endl;
            return;
        }
        Node* temp = head;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        }
        delete temp;
    }

    void delt() {
        if (head == nullptr) {
            cout << "Список пуст." << endl;
            return;
        }
        if (head == tail) {
            delete head;
            head = tail = nullptr;
            return;
        }
        Node* current = head;
        while (current->next != tail) {
            current = current->next;
        }
        delete tail;
        tail = current;
        tail->next = nullptr;
    }

    void delv(const string& value) {
        if (head == nullptr) {
            cout << "Список пуст." << endl;
            return;
        }
        if (head->data == value) {
            delh();
            return;
        }
        Node* current = head;
        while (current->next != nullptr && current->next->data != value) {
            current = current->next;
        }
        if (current->next == nullptr) {
            cout << "Элемент не найден." << endl;
            return;
        }
        Node* temp = current->next;
        current->next = current->next->next;
        if (current->next == nullptr) {
            tail = current;
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

void listS(string file, string actions);
bool emptyFile (string fileName);

#endif // LISTS_H_INCLUDED