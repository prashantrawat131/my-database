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

int DB::put(string key,string value)
{
    /*
    This function always appends at the end of the db file.
     */

    if (key == "" || value == "")
    {
        return INVALID_INPUT;
    }

    if (key.size() > keyMaxSize || value.size() > valueMaxSize)
    {
        return INPUT_SIZE_EXCEEDED;
    }

    if (index.find(key) != index.end())
    {
        return KEY_ALREADY_PRESENT;
    }

    dbFile.seekp(0, ios::end);

    key.append(keyMaxSize - key.size(), NULL_CHAR);
    const char *keyData = key.data();
    dbFile.write(keyData, key.size());

    value.append(valueMaxSize - value.size(), NULL_CHAR);
    const char *valueData = value.data();
    dbFile.write(valueData, value.size());

    return SUCCESS;
}

int DB::get(const string& requiredKey,string &value)
{
    if (requiredKey.size() > keyMaxSize)
    {
        return INPUT_SIZE_EXCEEDED;
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

        string key = parseKey(keyData);

        if (key == requiredKey)
        {
            value = parseValue(valueData);
            return SUCCESS;
        }
    }

    delete[] keyData;
    delete[] valueData;

    return KEY_NOT_FOUND;
}

int DB::del(const string &requiredKey)
{
    if (requiredKey.size() > keyMaxSize)
    {
        return INPUT_SIZE_EXCEEDED;
    }

    streampos pos = getKeyPos(requiredKey);
    dbFile.seekg(pos, ios::beg);
    while (!dbFile.eof())
    {
        char *keyData = new char[keyMaxSize];
        dbFile.read(keyData, keyMaxSize);

        string currKey = parseKey(keyData);

        if (currKey == requiredKey)
        {
            streampos pos = dbFile.tellg() - streampos(keyMaxSize);
            dbFile.seekp(static_cast<streampos>(pos));
            const int recordSize(keyMaxSize + valueMaxSize);
            char *nullData = new char[keyMaxSize + valueMaxSize];
            for (int i = 0; i < recordSize; i++)
            {
                nullData[i] = NULL_CHAR;
            }
            dbFile.write(nullData, recordSize);
            index.erase(requiredKey);
            return SUCCESS;
        }
    }
    return KEY_NOT_FOUND;
}

void DB::printAll()
{
    bool isEmpty = true;

    cout << "Printing all the data:\n--------------------------------\n";

    string key = "", value = "";
    char *keyData = new char[keyMaxSize], *valueData = new char[valueMaxSize];

    dbFile.seekg(0, ios::beg);

    while (dbFile.read(keyData, keyMaxSize) && dbFile.read(valueData, valueMaxSize))
    {
        key = parseKey(keyData);
        value = parseValue(valueData);

        if (key != "" && value != "")
        {
            cout << "Key = " << key << " Value = " << value << endl;
            isEmpty = false;
        }
    }

    if (isEmpty)
    {
        cout << "Database is empty\n";
    }
    cout << "--------------------------------\n";

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
    if (index.find(key) != index.end())
    {
        return index[key];
    }

    return streampos(0);
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
            string key = parseKey(keyData);

            streampos pos = dbFile.tellg() - streampos(keyMaxSize + valueMaxSize);
            index.insert({key, pos});
        }
    }

    delete[] keyData;
    delete[] valueData;

    dbFile.clear();
    dbFile.seekg(0, ios::beg);
}

int DB::rebuildDatabase()
{
    char *keyData = new char[keyMaxSize];
    char *valueData = new char[valueMaxSize];

    char *nextKeyData = new char[keyMaxSize];
    char *nextValueData = new char[valueMaxSize];

    char *nullData = new char[keyMaxSize + valueMaxSize];
    for (int i = 0; i < keyMaxSize + valueMaxSize; i++)
    {
        nullData[i] = NULL_CHAR;
    }

    dbFile.seekg(0, ios::beg);

    streampos leftPos = streampos(-1);

    while (dbFile.peek() != EOF)
    {
        while (dbFile.read(keyData, keyMaxSize) && dbFile.read(valueData, valueMaxSize))
        {
            if (keyData[0] == NULL_CHAR && valueData[0] == NULL_CHAR && leftPos == streampos(-1))
            {
                leftPos = dbFile.tellg() - streampos(keyMaxSize + valueMaxSize);
                break;
            }
        }

        while (dbFile.read(nextKeyData, keyMaxSize) && dbFile.read(nextValueData, valueMaxSize))
        {
            if (nextKeyData[0] == NULL_CHAR && nextValueData[0] == NULL_CHAR)
            {
                continue;
            }
            else
            {
                /* cout << "LeftPos = " << leftPos << endl;
                cout << "NextKey = " << nextKeyData << endl;
                cout << "NextValue = " << nextValueData << endl; */

                dbFile.seekp(leftPos, ios::beg);
                dbFile.write(nextKeyData, keyMaxSize);
                dbFile.write(nextValueData, valueMaxSize);

                dbFile.seekp(dbFile.tellp() - streampos(keyMaxSize + valueMaxSize), ios::beg);
                dbFile.write(nullData, keyMaxSize + valueMaxSize);

                dbFile.seekg(leftPos + streampos(keyMaxSize + valueMaxSize), ios::beg);
                leftPos = streampos(-1);

                break;
            }
        }
    }

    dbFile.clear();

    rebuildIndex();

    delete[] keyData;
    delete[] valueData;
    delete[] nextKeyData;
    delete[] nextValueData;
    delete[] nullData;

    return SUCCESS;
}

void DB::printDbMetaData()
{
    cout << "KeyMaxSize = " << keyMaxSize << " ValueMaxSize = " << valueMaxSize << endl;

    cout << "Index size = " << index.size() << endl;

    dbFile.seekg(0, ios::end);
    long long int dbFileSize = dbFile.tellg();
    if (dbFileSize > 1024)
    {
        cout << "DbFile size = " << dbFileSize / 1024 << " KB" << endl;
    }
    else
    {
        cout << "DbFile size = " << dbFileSize << " bytes" << endl;
    }

    cout << "Number of records = " << getNumberOfRecords() << endl;
}

long long int DB::getNumberOfRecords()
{
    dbFile.clear();
    dbFile.seekg(0, ios::beg);
    long long int dbFileRecords = 0;
    char *keyData = new char[keyMaxSize];
    char *valueData = new char[valueMaxSize];
    while (dbFile.read(keyData, keyMaxSize) && dbFile.read(valueData, valueMaxSize))
    {
        if (keyData[0] != NULL_CHAR && valueData[0] != NULL_CHAR)
        {
            dbFileRecords++;
        }
    }

    delete[] keyData;
    delete[] valueData;

    return dbFileRecords;
}

string DB::parseKey(const char *data)
{
    string key(data);
    return key.substr(0, key.find_first_of(NULL_CHAR));
}

string DB::parseValue(const char *data)
{
    string value(data);
    return value.substr(0, value.find_first_of(NULL_CHAR));
}