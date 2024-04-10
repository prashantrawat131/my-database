#include <gtest/gtest.h>
#include "../include/db.h"
#include <vector>
#include <string>
#include <fstream>
#include <codes.h>

TEST(DB, BasicTests)
{
    // delete the file if it exists to start fresh tests
    fstream file;
    file.open("test.db", ios::out | ios::binary | ios::trunc);
    file.close();

    DB db("test.db", 30, 30);
    vector<string> params = {};
    string value = "";

    params = {"", "this is one"};
    EXPECT_EQ(db.put(params), INVALID_INPUT);

    params = {"one", ""};
    EXPECT_EQ(db.put(params), INVALID_INPUT);

    params = {"one", "this is one"};
    EXPECT_EQ(db.put(params), SUCCESS);

    db.get({params[0]}, value);
    EXPECT_EQ(value, params[1]);

    params = {"two", "this is two"};
    EXPECT_EQ(db.put(params), SUCCESS);
    // EXPECT_EQ(db.put(params), KEY_ALREADY_PRESENT);
    db.get({params[0]}, value);
    EXPECT_EQ(value, params[1]);

    EXPECT_EQ(db.del({"one"}), SUCCESS);
    EXPECT_EQ(db.del({"one"}), KEY_NOT_FOUND);
    EXPECT_EQ(db.del({"three"}), KEY_NOT_FOUND);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}