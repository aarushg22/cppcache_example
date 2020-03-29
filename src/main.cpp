#include "Cppcache.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    my_cache::CPPCache<> cache(5);
    std::cout << "Write : " << cache.WriteToCache(1, 4) << std::endl;
    std::cout << "Exists : " << cache.ExistsInCache(1) << std::endl;
    return 0;
}
