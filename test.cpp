#include <fstream>
#include <iostream>

using namespace std;

int main()
{
    fstream file;
    file.open("test", ios::out | ios::binary);

    string str("Hello this is some data");
    str.append(10, '\0');
    const char *binaryData = str.data();
    cout << "Binary data: " << binaryData << endl;
    file.write(binaryData, str.size());
}