#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Maximum number of entries in the db
const size_t DB_FILE_SIZE = 10;
const size_t KEY_SIZE = 30;
const size_t VALUE_SIZE = 30;
const size_t RECORD_SIZE = KEY_SIZE + VALUE_SIZE;

class DB
{
private:
    fstream dbFile, indexFile;
    size_t hashFunc(const string &);
    bool fileExists(string);

public:
    DB(string);
    ~DB();
    int put(const vector<string> &);
    int get(const vector<string> &);
    int del(const vector<string> &);
    void printAll();
};

void DB::printAll()
{
    cout << "Printing all the data\n";

    string key = "", value = "";
    char *keyData = new char[KEY_SIZE], *valueData = new char[VALUE_SIZE];

    dbFile.seekg(ios::beg);

    while (!dbFile.eof())
    {
        dbFile.read(keyData, KEY_SIZE);
        key.assign(keyData);

        dbFile.read(valueData, VALUE_SIZE);
        value.assign(valueData);

        key = key.substr(0, key.find_first_of('\0'));
        value = value.substr(0, value.find_first_of('\0'));

        cout << "Key = " << key << " Value = " << value << endl;
    }

    delete[] keyData;
    delete[] valueData;
}

DB::DB(string dbName)
{
    string dbFilePath = "./databases/" + dbName;
    if (!fileExists(dbFilePath))
    {
        dbFile.open(dbFilePath, ios::out);
        // for (int i = 0; i < DB_FILE_SIZE; i++)
        // {
        //     dbFile.put('\n');
        // }
        dbFile.close();
    }
    dbFile.open(dbFilePath, ios::in | ios::out | ios::binary | ios::app);
}

DB::~DB()
{
    dbFile.close();
}

bool DB::fileExists(string filePath)
{
    ifstream in(filePath);
    return in.good();
}

size_t DB::hashFunc(const string &key)
{
    size_t hash = 0;
    for (char c : key)
    {
        hash = hash * DB_FILE_SIZE + c;
    }
    return hash % DB_FILE_SIZE;
}

int DB::put(const vector<string> &params)
{
    string key = params[0];
    string value = "";
    for (int i = 1; i < params.size(); i++)
    {
        value += params[i];
        if (i != params.size() - 1)
        {
            value += " ";
        }
    }

    // dbFile.seekp(ios::end);

    key.append(KEY_SIZE - key.size(), '\0');
    const char *keyData = key.data();
    dbFile.write(keyData, key.size());

    value.append(VALUE_SIZE - value.size(), '\0');
    const char *valueData = value.data();
    dbFile.write(valueData, value.size());

    /* size_t hash = hashFunc(key);
    cout << "Hash is " << hash << endl;
    string line;

    dbFile.seekg(0, ios::beg);
    for (int i = 0; i < hash; i++)
    {
        getline(dbFile, line);
    }
    streampos loc = dbFile.tellg();
    dbFile.seekp(loc, ios::beg);
    cout << "Loc: " << loc << endl;
    dbFile << hash << ";"
           << key << ";"
           << value << "\n"; */
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

    DB db("prashant123");

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
    else
    {
        db.printAll();
    }
}