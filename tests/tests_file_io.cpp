#include "../src/fileIO.cpp"
#include <iostream>
#include "gtest/gtest.h"

// Unfortunately I wasn't able to get more tests configured and running in time, if I 
// get more time, maybe I can try and write more tests for exception cases and other scenarios

TEST(FileIO, read_test){
    int line = 1;
    std::string filename = "test_1";
    std::string val = "333";
    std::ofstream f(filename);
    if(f){
        f << val;
        f.close();
    }
    EXPECT_EQ(ReadFromItems(line, filename), val);
}
TEST(FileIO, write_test)
{
    std::string filename = "test_3";
    std::string val1 = "100";
    std::string val2 = "102";
    std::string val3 = "103";
    std::ofstream fout(filename);
    if(fout){
        fout << val1 << std::endl;
        fout << val2 << std::endl;
        fout << val3 << std::endl;
        fout.close();
    }
    int line = 2;
    std::string str_temp;
    std::string val = "101";
    my_cache::CPPCache<> cache(5);
    WriteToItems(line, val, cache, filename);
    std::ifstream fin(filename);
    if(fin){
        std::getline(fin, str_temp);
        // Since we want to get the value in the second line
        std::getline(fin, str_temp);
        EXPECT_EQ(str_temp, val);
        fin.close();
    }
    
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}