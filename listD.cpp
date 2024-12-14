#include "libs/listD.h"

void listD(string fileName, string actions){
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
    int symbols = 0;
    for (char symbol : word){
        if (symbols == 2){
            command += symbol;
            continue;
        }
        symbols++;
    }
    ss >> temp;

    ListD list;

    if (fs::exists(fileName)){
        ifstream file (fileName);
        string line;
        getline(file, line);
        file.close();

        stringstream ss(line);
        string item;

        while (getline(ss, item, ';')){
            list.pusht(item);
        }
    }else{
        ofstream file(fileName);
        file.close();
    }

    if (command == "PUSHT" && countWords == 2){
        list.pusht(temp);
    }else if (command == "PUSHH" && countWords == 2){
        list.pushh(temp);
    }else if (emptyFile(fileName)){
        cout << "2:ERROR: Empty file" << endl;
        fs::remove(fileName);
        return;
    }else if (command == "DELH" && countWords == 1){
        list.delh();
    }else if (command == "DELT" && countWords == 1){
        list.delt();
    }else if (command == "DELV" && countWords == 2){
        list.delv(temp);
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

    list.saveToFile(fileName);

    if (emptyFile(fileName)) fs::remove(fileName);
}