#include<iostream>
#include<string>
#include<vector>
#include "../include/db.h"

using namespace std;

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