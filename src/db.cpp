#include "db.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

DB::DB(string dbName, const int &keySize, const int &valueSize) : dbFile(), index({}), keyMaxSize(keySize), valueMaxSize(valueSize)
{
    string dbFilePath = "./" + dbName;
    if (!fileExists(dbFilePath))
    {
        // just for file creation
        dbFile.open(dbFilePath, ios::out);
        dbFile.close();
    }
    dbFile.open(dbFilePath, ios::in | ios::out | ios::binary);
    /*
    The rebuildIndex function might take a long time when database grows larger.
    It is better to call this function in a separate thread.
    But for now let's call it when the database is created or opened some other time.
    */
    rebuildIndex();
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

int DB::put(const vector<string> &params)
{
    string key = params[0];

    if (key.size() > keyMaxSize)
    {
        return 103;
    }

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

    if (value.size() > valueMaxSize)
    {
        return 103;
    }

    dbFile.seekp(0, ios::end);

    key.append(keyMaxSize - key.size(), '\0');
    const char *keyData = key.data();
    dbFile.write(keyData, key.size());

    value.append(valueMaxSize - value.size(), '\0');
    const char *valueData = value.data();
    dbFile.write(valueData, value.size());

    return 1;
}

int DB::get(const vector<string> &params, string &value)
{
    string requiredKey = params[0];
    if (requiredKey.size() > keyMaxSize)
    {
        return 103;
    }

    streampos pos = getKeyPos(requiredKey);

    dbFile.seekg(pos, ios::beg);

    char *keyData = new char[keyMaxSize];
    char *valueData = new char[valueMaxSize];

    while (dbFile.read(keyData, keyMaxSize) && dbFile.read(valueData, valueMaxSize))
    {
        if (keyData == NULL || valueData == NULL)
        {
            continue;
        }

        string key(keyData);
        key = key.substr(0, key.find_first_of('\0'));

        if (key == requiredKey)
        {
            value.assign(valueData);
            value = value.substr(0, value.find_first_of('\0'));
            return 1;
        }
    }

    return 101;
}

int DB::del(const vector<string> &params)
{
    string requiredKey = params[0];

    if (requiredKey.size() > keyMaxSize)
    {
        return 103;
    }

    streampos pos = getKeyPos(requiredKey);
    dbFile.seekg(pos, ios::beg);
    while (!dbFile.eof())
    {
        char *keyData = new char[keyMaxSize];
        dbFile.read(keyData, keyMaxSize);

        string currKey(keyData);
        currKey = currKey.substr(0, currKey.find_first_of('\0'));

        if (currKey == requiredKey)
        {
            streampos pos = dbFile.tellg() - streampos(keyMaxSize);
            dbFile.seekp(static_cast<streampos>(pos));
            const int recordSize(keyMaxSize + valueMaxSize);
            char *nullData = new char[keyMaxSize + valueMaxSize];
            for (int i = 0; i < recordSize; i++)
            {
                nullData[i] = '\0';
            }
            dbFile.write(nullData, recordSize);
            return 1;
        }
    }
    return 101;
}

void DB::printAll()
{
    cout << "Printing all the data\n";

    string key = "", value = "";
    char *keyData = new char[keyMaxSize], *valueData = new char[valueMaxSize];

    dbFile.seekg(0, ios::beg);

    while (dbFile.read(keyData, keyMaxSize) && dbFile.read(valueData, valueMaxSize))
    {
        key.assign(keyData);
        value.assign(valueData);

        // cout<<key.find_first_of('\0')<<endl;

        // if (key.find_first_of('\0') != string::npos)
        // {
        // cout<<"Parsing key\n";
        key = key.substr(0, key.find_first_of('\0'));
        // }

        // if (value.find_first_of('\0') != string::npos)
        // {
        value = value.substr(0, value.find_first_of('\0'));
        // }

        cout << "Key = " << key << "\tValue = " << value << endl;
    }

    delete[] keyData;
    delete[] valueData;
}

void DB::printIndex()
{
    cout << "Printing index\n";
    for (const pair<string, streampos> &p : index)
    {
        cout << "Key = " << p.first << " Position = " << p.second << endl;
    }
}

streampos DB::getKeyPos(const string &key)
{
    streampos pos = index[key];
    return pos == NULL ? streampos(0) : pos;
}

void DB::rebuildIndex()
{
    dbFile.seekg(0, ios::beg);
    char *keyData = new char[keyMaxSize];
    char *valueData = new char[valueMaxSize];

    while (dbFile.read(keyData, keyMaxSize) && dbFile.read(valueData, valueMaxSize))
    {
        if (keyData != NULL)
        {
            string key(keyData);
            key = key.substr(0, key.find_first_of('\0'));

            streampos pos = dbFile.tellg() - streampos(keyMaxSize + valueMaxSize);
            index.insert({key, pos});
        }
    }

    delete[] keyData;
    delete[] valueData;

    dbFile.clear();
    dbFile.seekg(0, ios::beg);
}