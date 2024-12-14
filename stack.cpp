#include "libs/stack.h"

void stack(string fileName, string actions){
    stringstream words(actions);
    string elem;
    int countWords = 0;
    while (words >> elem){
        countWords ++;
    }
    if (countWords > 2){
        cout << "1:ERROR: A lot of arguments" << endl;
        return;
    }

    stringstream ss(actions);
    string word, command, temp;
    ss >> word;
    bool firSymbol = false;
    for (char symbol : word){
        if (firSymbol){
            command += symbol;
            continue;
        }
        firSymbol = true;
    }
    ss >> temp;

    Stack stack;

    if (fs::exists(fileName)){
        ifstream file (fileName);
        string line;
        getline(file, line);
        file.close();

        stringstream ss(line);
        string item;

        StrArray array;
        while (getline(ss, item, ';')){
            array.push(item);
        }
        int index = array.sizeM();
        for (int i = index - 1; i >= 0; i--){
            string res;
            array.get(i, res);
            stack.push(res);
        }
    }else{
        ofstream file(fileName);
        file.close();
    }

    if (command == "PUSH" && countWords == 2){
        stack.push(temp);
    }else if (emptyFile(fileName)){
        cout << "2:ERROR: Empty file" << endl;
        fs::remove(fileName);
        return;
    }else if (command == "DEL" && countWords == 1){
        stack.del();
    }else if (command == "PRINT" && countWords == 1){
        ifstream file(fileName);
        string result;
        getline(file, result);
        cout << result << endl;
        file.close();
    }else{
        cout << "3:ERROR: Unknown command." << endl;
        return;
    }

    stack.saveToFile(fileName);

    if (emptyFile(fileName)) fs::remove(fileName);
}