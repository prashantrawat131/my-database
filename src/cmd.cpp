#include <iostream>
#include <string>
#include <vector>
#include "../include/db.h"
#include <../include/codes.h>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: db <command> <params>" << endl;
        return 1;
    }

    DB db("prashant123", 30, 30);

    string command = argv[1];
    vector<string> params(argv + 2, argv + argc);

    if (command == "put")
    {
        string value = "";
        for (int i = 1; i < params.size(); i++)
        {
            value += params[i];
            if (i != params.size() - 1)
            {
                value += " ";
            }
        }
        int code = db.put(params[0], value);
        if (code == KEY_ALREADY_PRESENT)
        {
            cout << "Key already exists" << endl;
        }
        else if (code == SUCCESS)
        {
            cout << "Key-Value pair inserted" << endl;
        }
        else if (code == INPUT_SIZE_EXCEEDED)
        {
            cout << "Input size exceeded" << endl;
        }
        else
        {
            cout << "Unkown reason for error\n";
        }
    }
    else if (command == "get")
    {
        string value = "";
        int rc = db.get(params[0], value);
        if (rc == KEY_NOT_FOUND)
        {
            cout << "Key not found" << endl;
        }
        else if (rc == SUCCESS)
        {
            cout << "Value: " << value << endl;
        }
        else if (rc == INPUT_SIZE_EXCEEDED)
        {
            cout << "Input size exceeded" << endl;
        }
        else
        {
            cout << "Unkown reason for error\n";
        }
    }
    else if (command == "delete")
    {
        int rc = db.del(params[0]);
        if (rc == KEY_NOT_FOUND)
        {
            cout << "Key not found" << endl;
        }
        else if (rc == SUCCESS)
        {
            cout << "Key deleted" << endl;
        }
        else if (rc == INPUT_SIZE_EXCEEDED)
        {
            cout << "Input size exceeded" << endl;
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
    else if (command == "rebuilddb")
    {
        int rc = db.rebuildDatabase();
        if (rc == SUCCESS)
        {
            cout << "Database rebuilt successfully\n";
        }
        else
        {
            cout << "Error in rebuilding database\n";
        }
    }
    else if (command == "printdbmetadata")
    {
        db.printDbMetaData();
    }
    else
    {
        cout << "Usage: db <command> <params>" << endl;
    }
}