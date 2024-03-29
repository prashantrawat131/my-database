#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Maximum number of entries in the db
const size_t DB_SIZE = 1000;

class DB
{
private:
    fstream file;
    size_t hashFunc(const string &);

public:
    DB()
    {
        file.open("db.txt", ios::in | ios::out | ios::app);
    }
    int put(const vector<string> &);
    int get(const vector<string> &);
    int del(const vector<string> &);
};

size_t DB::hashFunc(const string &key)
{
    size_t hash = 0;
    for (char c : key)
    {
        hash = hash * DB_SIZE + c;
    }
    return hash;
}

int DB::put(const vector<string> &params)
{
    string key = params[0];
    string value = params[1];
    size_t hash = hashFunc(key);
    file.seekp(0);
    for (int i = 0; i < hash; i++)
    {
        string line;
        getline(file, line);
    }

    file << hash << ";" << key << ";" << value<<"\n";
    return 0;
}

int DB::get(const vector<string> &params)
{
    return 0;
}

int DB::del(const vector<string> &params)
{
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: db <command> <params>" << endl;
        return 1;
    }

    DB db;

    string command = argv[1];
    vector<string> params(argv + 2, argv + argc);

    if (command == "put")
    {
        db.put(params);
    }
    else if (command == "get")
    {
        db.get(params);
    }
    else if (command == "delete")
    {
        db.del(params);
    }
}