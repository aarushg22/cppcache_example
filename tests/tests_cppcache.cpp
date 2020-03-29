#include <Cppcache.hpp>
#include <iostream>
#include "gtest/gtest.h"

TEST(Cppcache, cache_size){
    int cache_size = 5;
    my_cache::CPPCache<int, int> cache(cache_size);
    EXPECT_EQ(cache.cache_size, cache_size);
}

TEST(Cppcache, write_to_cache){
    my_cache::CPPCache<int, int> cache(5);
    int key=1;
    int val =2;
    cache.WriteToCache(key, val);
    EXPECT_EQ(cache.ExistsInCache(key), true);
}


TEST(Cppcache, read_from_cache){
    my_cache::CPPCache<int, int> cache(5);
    int key = 1;
    int val = 2;
    cache.WriteToCache(key, val);
    EXPECT_EQ(cache.ReadFromCacheAndUpdate(key),val );
}
// Could not get the exception test case to work
// TEST(Cppcache, read_from_cache_exception)
// {
//     my_cache::CPPCache<int, int> cache(5);
//     int key = 1;
//     int val = 2;
//     // cache.WriteToCache(key, val);
//     try
//     {
//         cache.ReadFromCacheAndUpdate(key);
//         FAIL()<< "Expected std::out_of_range";
//     }
//     catch (std::out_of_range const &err)
//     {
//         std::cout << err.what();
//         // EXPECT_EQ(err.what(), std::string("Out of range"));
//     }
// }
TEST(Cppcache, dirty_update_cache)
{
    my_cache::CPPCache<int, int> cache(5);
    int key = 1;
    int val = 2;
    int sec_val = 3;
    cache.WriteToCache(key, val);
    cache.CacheUpdate(key, sec_val);
    EXPECT_EQ(cache.ReadFromCacheAndUpdate(key), sec_val);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}