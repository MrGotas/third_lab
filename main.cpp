#include "libs/massive.h"
#include "libs/listS.h"
#include "libs/listD.h"
#include "libs/queue.h"
#include "libs/stack.h"
#include "libs/hash_table.h"

string withoutQuotes (string userInput);
bool inputFormat(string userInput, string& fileName, string& actions);

int main (int argc, char* argv[]) {
    if (argc != 1){
        if (argc < 5){
            cout << "1:ERROR: Unknown command." << endl;
            argc = 1;
        }
    }

    string fileName, actions;
    if (argc >= 5){
        if (string(argv[1]) != "--file" || string(argv[3]) != "--query"){
            cout << "2:ERROR: Unknown command." << endl;
            argc = 1;
        }else{
            string temp = argv[2];
            fileName = "files/" + temp;
            for (size_t i = 4; i < argc; i ++){
                actions = actions + argv[i];
                actions += ' ';
            }
        }
    }
    
    while (true){
        if (argc == 1){
            string userInput;
            getline(cin, userInput);

            if (!inputFormat(userInput, fileName, actions)){
                cout << "3:ERROR: Unknown input." << endl;
                continue;
            }
        }

        stringstream ss(actions);
        string word;
        ss >> word;
        if (word == "EXIT"){
            return 0;
        }else if(word[0] == 'M'){
            massive(fileName, actions);
        }else if (word[0] == 'L' && word[1] == 'S'){
            listS(fileName, actions);
        }else if(word[0] == 'L' && word[1] == 'D'){
            listD(fileName, actions);
        }else if(word[0] == 'Q'){
            queue(fileName, actions);
        }else if(word[0] == 'S'){
            stack(fileName, actions);
        }else if(word[0] == 'H'){
            hash_table(fileName, actions);
        }else{
            cout << "4:ERROR: Unknown structure." << endl;
        }

        fileName, actions = "";
        argc = 1;
    }

    return 0;
}

string withoutQuotes (string userInput){
    string result;
    for (char symbol : userInput){
        if (symbol != '\''){
            result += symbol;
        }
    }

    return result;
}

bool inputFormat(string userInput, string& fileName, string& actions){
    stringstream words(userInput);
    string word;
    int countWords = 0;
    while (words >> word){
        countWords ++;
    }

    stringstream ss(userInput);
    string token;
    ss >> token;
    if (token == "EXIT" && countWords == 1){
        actions = token;
        return true;
    }else if(countWords < 4) return false;

    if (token != "--file") return false;
    
    ss >> token;
    fileName = "files/" + token;

    ss >> token;
    if (token != "--query") return false;

    string temp;
    int count = 0;
    while (ss >> token){
        if (count == 0){
            temp += token;
            count++;
            continue;
        }else{
            temp += ' ' + token;
        }
    }

    actions = withoutQuotes(temp);
    return true;
}