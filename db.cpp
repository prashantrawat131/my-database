#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

const size_t KEY_SIZE = 30;
const size_t VALUE_SIZE = 30;
const size_t RECORD_SIZE = KEY_SIZE + VALUE_SIZE;
const char NULL_RECORD[RECORD_SIZE] = {'\0'};

class DB
{
private:
    fstream dbFile, indexFile;
    // size_t hashFunc(const string &);
    bool fileExists(string);
    map<string, streampos> index;

public:
    DB(string);
    ~DB();
    int put(const vector<string> &);
    int get(const vector<string> &, string &);
    int del(const vector<string> &);
    void printAll();
    void printIndex();
};

DB::DB(string dbName) : dbFile(), index({})
{
    string dbFilePath = "./databases/" + dbName;
    if (!fileExists(dbFilePath))
    {
        // just for file creation
        dbFile.open(dbFilePath, ios::out);
        dbFile.close();
    }
    dbFile.open(dbFilePath, ios::in | ios::out | ios::binary);
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

/* size_t DB::hashFunc(const string &key)
{
    size_t hash = 0;
    for (char c : key)
    {
        hash = hash * DB_FILE_SIZE + c;
    }
    return hash % DB_FILE_SIZE;
} */

int DB::put(const vector<string> &params)
{
    string key = params[0];

    if (index[key] != NULL)
    {
        return 102;
    }

    string value = "";
    for (int i = 1; i < params.size(); i++)
    {
        value += params[i];
        if (i != params.size() - 1)
        {
            value += " ";
        }
    }

    dbFile.seekp(0, ios::end);

    key.append(KEY_SIZE - key.size(), '\0');
    const char *keyData = key.data();
    dbFile.write(keyData, key.size());

    value.append(VALUE_SIZE - value.size(), '\0');
    const char *valueData = value.data();
    dbFile.write(valueData, value.size());

    streampos pos = dbFile.tellp() - streampos(RECORD_SIZE);
    index.insert({key, pos});

    return 1;
}

int DB::get(const vector<string> &params, string &value)
{
    string requiredKey = params[0];

    streampos pos = index[requiredKey];
    if (pos != NULL)
    {
        cout << "Using index to find value\n";
        dbFile.seekg(pos, ios::beg);
    }
    else
    {
        dbFile.seekg(0, ios::beg);
    }

    while (!dbFile.eof())
    {
        char *keyData = new char[KEY_SIZE];
        dbFile.read(keyData, KEY_SIZE);

        string currKey(keyData);
        currKey = currKey.substr(0, currKey.find_first_of('\0'));
        if (currKey == requiredKey)
        {
            char *valueData = new char[VALUE_SIZE];
            dbFile.read(valueData, VALUE_SIZE);

            value = string(valueData);
            value = value.substr(0, value.find_first_of('\0'));

            return 1;
        }
    }

    return 101;
}

int DB::del(const vector<string> &params)
{
    string requiredKey = params[0];

    dbFile.seekg(0, ios::beg);
    while (!dbFile.eof())
    {
        char *keyData = new char[KEY_SIZE];
        dbFile.read(keyData, KEY_SIZE);

        string currKey(keyData);
        currKey = currKey.substr(0, currKey.find_first_of('\0'));

        if (currKey == requiredKey)
        {
            streampos pos = dbFile.tellg() - streampos(KEY_SIZE);
            dbFile.seekp(static_cast<streampos>(pos));
            dbFile.write(NULL_RECORD, RECORD_SIZE);
            return 1;
        }
    }
    return 101;
}

void DB::printAll()
{
    cout << "Printing all the data\n";

    string key = "", value = "";
    char *keyData = new char[KEY_SIZE], *valueData = new char[VALUE_SIZE];

    dbFile.seekg(0, ios::beg);

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

void DB::printIndex()
{
    cout << "Printing index\n";
    for (const pair<string, streampos> &p : index)
    {
        cout << "Key = " << p.first << " Value = " << p.second << endl;
    }
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
        int rc = db.put(params);
        if (rc == 102)
        {
            cout << "Key already exists" << endl;
        }
        else if (rc == 1)
        {
            cout << "Key-Value pair inserted" << endl;
        }
        else
        {
            cout << "Unkown reason for error\n";
        }
    }
    else if (command == "get")
    {
        string value = "";
        int rc = db.get(params, value);
        if (rc == 101)
        {
            cout << "Key not found" << endl;
        }
        else if (rc == 1)
        {
            cout << "Value: " << value << endl;
        }
        else
        {
            cout << "Unkown reason for error\n";
        }
    }
    else if (command == "delete")
    {
        int rc = db.del(params);
        if (rc == 101)
        {
            cout << "Key not found" << endl;
        }
        else if (rc == 1)
        {
            cout << "Key deleted" << endl;
        }
        else
        {
            cout << "Unkown reason for error\n";
        }
    }
    else if (command == "printall")
    {
        db.printAll();
    }
    else if (command == "printindex")
    {
        db.printIndex();
    }
    else
    {
        cout << "Usage: db <command> <params>" << endl;
    }
}