#include <../src/fileIO.cpp>
#include <iostream>
#include "gtest/gtest.h"

// Unfortunately I wasn't able to get these tests configured and running in time, if I 
// get another day or so, maybe I can try and write more tests

// TEST(FileIO, read_test){
//     int line = 1;
//     std::string filename = "test";
//     std::string val = "333";
//     EXPECT_EQ(ReadFromItems(line, filename), val);
// }
// TEST(FileIO, write_test)
// {
//     int key = 1;
//     std::string filename = "test_1";
//     std::string val = "100";
//     std::string str_temp;
//     my_cache::CPPCache<> cache(5);
//     WriteToItems(key, val, cache, filename);
//     std::ifstream f(filename);
//     if(f){
//         std::getline(f,str_temp);
//         EXPECT_EQ(str_temp, val);
//         f.close();
//     }
    
// }

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}