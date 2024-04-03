#pragma once
// Todo: Learn to use #ifndef, #define, #endif

#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class DB
{
private:
    fstream dbFile, indexFile;
    bool fileExists(string);
    map<string, streampos> index;
    const size_t keyMaxSize;
    const size_t valueMaxSize;
    streampos getKeyPos(const string &key);
    void rebuildIndex();

public:
    DB(string, const int&, const int&);
    ~DB();
    int put(const vector<string> &);
    int get(const vector<string> &, string &);
    int del(const vector<string> &);
    void printAll();
    void printIndex();
};