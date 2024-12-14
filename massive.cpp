#include "libs/massive.h"

void massive(string fileName, string actions){
    stringstream words(actions);
    string elem;
    int countWords = 0;
    while (words >> elem){
        countWords ++;
    }
    if (countWords > 3){
        cout << "13:ERROR: A lot of arguments" << endl;
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

    StrArray array;

    if (fs::exists(fileName)){
        ifstream file (fileName);
        string line;
        getline(file, line);
        file.close();

        stringstream ss(line);
        string item;

        while (getline(ss, item, ';')){
            array.push(item);
        }
    }else{
        ofstream file(fileName);
        file.close();
    }

    if (command == "PUSH" && countWords == 2){
        array.push(temp1);
    }else if (emptyFile(fileName)){
        cout << "12:ERROR: Empty file" << endl;
        fs::remove(fileName);
        return;
    }else if (command == "PUSHI" && countWords == 3){
        int index;
        if (!toNum(temp1, index)){
            return;
        }
        array.pushi(index, temp2);
    }else if (command == "GET" && countWords == 2){
        int index;
        if (!toNum(temp1, index)){
            return;
        }
        string result;
        if (!array.get(index, result)){
            return;
        }
        cout << result << endl;
    }else if (command == "DEL" && countWords == 2){
        int index;
        if (!toNum(temp1, index)){
            return;
        }
        array.del(index);
    }else if (command == "REPLACE" && countWords == 3){
        int index;
        if (!toNum(temp1, index)){
            return;
        }
        array.replace(index, temp2);
    }else if (command == "SIZE" && countWords == 1){
        cout << array.sizeM() << endl;
    }else if (command == "PRINT" && countWords == 1){
        ifstream file(fileName);
        string result;
        getline(file, result);
        cout << result << endl;
        file.close();
    }else{
        cout << "5:ERROR: Unknown command." << endl;
        return;
    }

    array.saveToFile(fileName);

    if (emptyFile(fileName)) fs::remove(fileName);
}

bool toNum(const string& temporary, int& index) {
    int result = 0;
    int startIndex = 0;

    if (temporary[0] == '-') {
        cout << "7:ERROR: Negative index." << endl;
        return false;
    }

    while (temporary[startIndex] != '\0') {
        char charc = temporary[startIndex];
        if (charc < '0' || charc > '9') {
            cout << "8:ERROR: Not a number." << endl;
            return false;
        }

        result = result * 10 + (charc - '0');
        startIndex++;
    }

    index = result;
    return true;
}

bool emptyFile (string fileName) {
    ifstream file (fileName);

    string temp;
    if(getline(file, fileName)){
        file.close();
        return false;
    }else{
        file.close();
        return true;
    }
}