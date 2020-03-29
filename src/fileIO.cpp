
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>
#include <vector>
#include <Cppcache.hpp>

/* This file contains methods which help in abstracting the IO operation to/from files on disk
*  This took me a loooong time, turns out there is no efficient way to insert a particular line of 
*  an arbitrary size into a file at any index, without corrupting it's original data. Well actually there
*  might be, but it seemed too complex to implement.   
*/
std::mutex items_read_mutex;
std::mutex items_write_mutex;

void WriteToItems(const int &key, const std::string &val, my_cache::CPPCache<int, std::string> &cache, const std::string &items_file_name)
{
    // Better than just a lock as it's RAII so it automatically manages the lock in lifetime of this resource
    const std::lock_guard<std::mutex> lock(items_write_mutex);

    
    // Have to use a temporary file to copy all the contents and create a new
    // items file with the new value inserted at the specified line
    // Cannot just write over a particular line because the in case of overwriting
    // in files, the number of bytes need to be the same for the line being
    // overwritten, which won't true in our case
    std::ifstream filein(items_file_name);           //File to read from
    std::ofstream fileout(items_file_name + "temp"); //Temporary file
    if (!filein || !fileout)
    {
        throw "Error opening files!";
    }

    std::string str_temp;
    int index = 0;
    // while (filein >> strTemp)
    while(std::getline(filein, str_temp))
    {
        if (index == key - 1)
        {
            str_temp = val;
        }
        index += 1;
        str_temp += "\n";
        fileout << str_temp;
    }
    fileout.close();
    filein.close();

    // Copy back the file to original src
    std::ifstream src(items_file_name + "temp", std::ios::binary);
    std::ofstream dst(items_file_name, std::ios::binary);

    dst << src.rdbuf();

    if (cache.ExistsInCache(key))
    {
        cache.CacheUpdate(key, val);
    }
}


std::string ReadFromItems(const int &line, const std::string &items_file_name)
{
    std::ifstream read_file(items_file_name);
    std::string value = "";
    int index = 0;
    bool chk_flag = false;
    if (read_file.is_open())
    {
        //Have a weird bug here, sometimes the getline just exists on the first read, no idea why, yet
        while (std::getline(read_file, value))
        {
            if (index == (line - 1))
            {
                chk_flag = true;
                break;
            }
            index+=1;
        }
        if(read_file.bad()){
            throw "ReadFromItems : IOError";
        }
        read_file.close();
    }
    if (!chk_flag)
    {
        throw "ReadFromItems : line index provided is more than the lines in the file";
    }
    else if (value.compare("")==0)
    {
        throw "ReadFromItems : Incorrect input configuration for Items, read an empty value";
    }
    return value;
}