#include "libs/hash_table.h"

void hash_table(string fileName, string actions){
    stringstream words(actions);
    string elem;
    int countWords = 0;
    while (words >> elem){
        countWords ++;
    }
    if (countWords > 3){
        cout << "1:ERROR: A lot of arguments" << endl;
        return;
    }

    stringstream ss(actions);
    string word, command, temp1, temp2;
    ss >> word;
    bool firSymbol = false;
    for (char symbol : word){
        if (firSymbol){
            command += symbol;
            continue;
        }
        firSymbol = true;
    }
    ss >> temp1;
    ss >> temp2;

    HashTable ht;

    if (fs::exists(fileName)){
        ht.loadFromFile(fileName);
    }else{
        ofstream file(fileName);
        file.close();
    }

    if (command == "PUSH" && countWords == 3){
        ht.push(temp1, temp2);
    }else if (emptyFile(fileName)){
        cout << "2:ERROR: Empty file" << endl;
        fs::remove(fileName);
        return;
    }else if (command == "GET" && countWords == 2){
        string result;
        if (ht.get(temp1, result)) {
            cout << result << endl;
        }else{
            cout << "3:ERROR: Key not found." << endl;
        }
    }else if (command == "DEL" && countWords == 2){
        if (!ht.del(temp1)) cout << "4:ERROR: Key not found." << endl;
    }else if (command == "PRINT" && countWords == 1){
        ifstream file(fileName);
        string result;
        while (getline(file, result)){
            cout << result << endl;
        }
        file.close();
    }else{
        cout << "5:ERROR: Unknown command." << endl;
        return;
    }

    ht.saveToFile(fileName);

    if (emptyFile(fileName)) fs::remove(fileName);
}