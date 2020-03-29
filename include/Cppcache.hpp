#ifndef CPPCACHE_HPP_INCLUDED
#define CPPCACHE_HPP_INCLUDED

#include <unordered_map>
#include <list>
#include <cstddef>
#include <stdexcept>

namespace my_cache {

template <typename key=int, typename value=int> 
class CPPCache {
  public:
    CPPCache(int);
    // To handle a a miss on the LRU cache
    bool WriteToCache(const key& my_key, const value& my_value);
    // To get item from cache and update queue
    const value& GetFromCacheAndUpdate(const key& my_key);
    //To check if item exists in cache
    bool ExistsInCache(const key& my_key) const;

  public:
    // store keys of cache
    std::list<key> key_list;
    // store references of key in cache
    typedef typename std::list<key>::iterator list_it;
    typedef std::pair < list_it, value > key_pos_val;
    std::unordered_map<key, key_pos_val> cache_key_value_map;
    size_t cache_size;
};

}
#endif