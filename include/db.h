#ifndef DB_H
#define DB_H

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <codes.h>

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
    const char NULL_CHAR = '\0';
    long long int getNumberOfRecords();
    string parseKey(const char *);
    string parseValue(const char *);

public:
    DB(string, const int &, const int &);
    ~DB();
    int put(string, string);
    int get(const string &, string &);
    int del(const string &);
    void printAll();
    void printIndex();
    void rebuildIndex();
    int rebuildDatabase();
    void printDbMetaData();
};

#endif