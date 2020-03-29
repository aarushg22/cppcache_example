#include "Cppcache.hpp"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>
#include <vector>
#include "fileIO.cpp"


// void WriteToItems(const int &key, const int &val, my_cache::CPPCache<int, int> &cache, std::vector<int> &items) //, std::fstream& items_file)
// {
//     const std::lock_guard<std::mutex> lock(items_write_mutex);
//     // change to write directly to file
//     items[key - 1] = val;
//     if (cache.ExistsInCache(key))
//     {
//         cache.CacheUpdate(key, val);
//     }
// }

void ReaderThread(my_cache::CPPCache<> &cache, const std::string &read_file_name, const std::string& items_file_name)//std::vector<int>& items) 
{
    //read from items file and write to reader output file  
    try {
        std::cout << "read file " << read_file_name << "  " << cache.cache_size << std::endl;
        std::string line_number;
        int key;
        std::vector< std::string> to_write;
        std::ifstream read_file(read_file_name);
        if (read_file.is_open())
        {
            while (std::getline(read_file, line_number))
            {
                key = std::stoi(line_number);
                std::cout<< "file :" << read_file_name <<" key :" << key << std::endl;
                std::string val;
                std::string read_from;
                if(cache.ExistsInCache(key)){
                    val = cache.ReadFromCacheAndUpdate(key);  
                    read_from = " cache";
                } else {
                    // change to get directly from file
                    // val = items[key-1];
                    // val = std::strtod(ReadFromItems(key,items_file_name).c_str(), NULL);
                    val = ReadFromItems(key, items_file_name);
                    read_from = " disk";
                    cache.WriteToCache(key, val);
                }
                to_write.push_back(val+ read_from);

            }
            read_file.close();
        }
        else
        {
            throw "FATAL!! Could not open read file";
        }
        std::ofstream write_file(read_file_name+".out");
        int i=0;
        if(write_file.is_open()){
            for(auto &i : to_write){
            write_file << i << std::endl;
            }
            write_file.close();
        }
        else
        {
            throw "FATAL!! Could not open readers output file";
        }
    } catch (std::exception &e) {
        std::cout<<"Error in reader thread : " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    } catch (const char* msg){
        std::cout << "Error in reader thread : " << msg << std::endl;
        exit(EXIT_FAILURE);
    } catch (...) {
        std::cout << "Error in reader thread." << std::endl;
        exit(EXIT_FAILURE);
    }
}

void WriterThread(my_cache::CPPCache<> &cache, const std::string &write_file_name, const std::string &items_file_name) //std::vector<int>& items)
{
    try {
        std::cout << "write file" << write_file_name << "  " << cache.cache_size << std::endl;
        std::string line_number_and_val;
        int key;
        std::string val;
        std::string space = " ";
        std::vector<std::string> to_write;
        std::ifstream write_file(write_file_name);
        if (write_file.is_open())
        {
            while (std::getline(write_file, line_number_and_val))
            {
                key = std::stoi(line_number_and_val.substr(0,line_number_and_val.find(space)));
                val = line_number_and_val.substr(line_number_and_val.find(space)+1, line_number_and_val.size());
                std::cout << "file :" << write_file_name << " key : " << key << " val : "<< val << std::endl;
                // WriteToItems(key, val, cache, items);
                WriteToItems(key, val, cache, items_file_name);
            }
            write_file.close();
        }
        else
        {
            throw "FATAL!! Could not open read file";
        }
    }
    catch (std::exception &e)
    {
        std::cout << "Error in writer thread : " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    catch (const char *msg)
    {
        std::cout << "Error in writer thread : " << msg << std::endl;
        exit(EXIT_FAILURE);
    }
    catch (...)
    {
        std::cout << "Error in writer thread." << std::endl;
        exit(EXIT_FAILURE);
    }
}
int main(int argc, char **argv)
{
    try{
        if(argc!=5){
            throw "Improper parameters, need to input in format :\n./<executable_name> <size_of_cache> <reader_file> <writer_file> <items_file>";
        }
        my_cache::CPPCache<int, std::string> cache(std::stoi(argv[1]));

        //my raw thread pool
        std::vector<std::thread> workers;

        // Spawn Reader Threads
        std::string read_file_name;
        std::ifstream readers_file(argv[2]);
        
        if (readers_file.is_open())
        {
            while (std::getline(readers_file, read_file_name))
            {
                workers.push_back(std::thread(ReaderThread, std::ref(cache), read_file_name, argv[4]));
            }
            readers_file.close();
        } else {
            throw "FATAL!! Could not open readers file";
        }
        
        // Spawn Writer Threads
        std::string write_file_name;
        std::ifstream writers_file(argv[3]);
        if (writers_file.is_open())
        {
            while (std::getline(writers_file, write_file_name))
            {
                workers.push_back(std::thread(WriterThread, std::ref(cache), write_file_name, argv[4]));
            }
            writers_file.close();
        } else {
            throw "FATAL!! Could not open writers file";
        }

        // Block to wait for all threads to finish
        for (auto &worker : workers) {
            worker.join();
        }
        

    } catch (const char *msg) {
        std::cerr << msg << std::endl;
        return -1;
    } catch (std::invalid_argument& e) {
        std::cerr << "Issue Occured in main with : " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Error in main"  << std::endl;
        return -1;
    }
    return 0;
}
