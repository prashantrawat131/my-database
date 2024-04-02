#pragma once
// Todo: Learn to use #ifndef, #define, #endif

#include<fstream>
#include<string>
#include<vector>
#include<map>

using namespace std;

class DB
{
private:
    fstream dbFile, indexFile;
    // size_t hashFunc(const string &);
    bool fileExists(string);
    map<string, streampos> index;
    const size_t KEY_SIZE = 30;
    const size_t VALUE_SIZE = 30;
    const size_t RECORD_SIZE = KEY_SIZE + VALUE_SIZE;
    const char NULL_RECORD[60] = {'\0'};

public:
    DB(string);
    ~DB();
    int put(const vector<string> &);
    int get(const vector<string> &, string &);
    int del(const vector<string> &);
    void printAll();
    void printIndex();
};