#ifndef CPPCACHE_HPP_INCLUDED
#define CPPCACHE_HPP_INCLUDED

#include <mutex>
#include <unordered_map>
#include <list>
#include <cstddef>
#include <stdexcept>
#include <iostream>

/* This is the main LRU cache file which holds the templated class of CPPcache
*  All the source code is inside a single header file because, the template definitions
*  need to be availaible at runtime.
*  P.S. - Feels like the templating was overkill, and took a lot of unecessary time to get it compiled properly
*/  
namespace my_cache
{

template <typename key = int, typename value = std::string>
class CPPCache
{
public:
  CPPCache(int);
  // To handle a a miss on the LRU cache
  bool WriteToCache(const key &my_key, const value &my_value);
  // To get item from cache and update queue
  const value &ReadFromCacheAndUpdate(const key &my_key);
  // For dirty cache
  bool CacheUpdate(const key &my_key, const value &my_value);
  //To check if item exists in cache
  bool ExistsInCache(const key &my_key) const;
  size_t cache_size;

private:
  // store keys of cache
  std::list<key> key_list;
  // store references of key in cache
  typedef typename std::list<key>::iterator list_it;
  typedef std::pair<list_it, value> key_pos_val;
  std::unordered_map<key, key_pos_val> cache_key_value_map;
  std::mutex cache_mutex;
};

// Constructor
template <typename key, typename value>
CPPCache<key, value>::CPPCache(int n)
{
  this->cache_size = n;
}

// In case of a miss, write recent used value to cache
template <typename key, typename value>
bool CPPCache<key, value>::WriteToCache(const key &my_key, const value &my_value)
{
  // Better than just a lock as it's RAII so it automatically manages the lock in lifetime of this resource
  const std::lock_guard<std::mutex> lock(cache_mutex);

  try
  {
    this->key_list.push_front(my_key);
    this->cache_key_value_map[my_key] = std::make_pair(this->key_list.begin(), my_value);
    if (this->key_list.size() > this->cache_size)
    {
      auto last_key = this->key_list.back();
      cache_key_value_map.erase(last_key);
      this->key_list.pop_back();
    }
  }
  catch (std::exception &e)
  {
    std::cerr << "Standard exception in Exception in WriteToCache : " << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }
  catch (const char *msg)
  {
    std::cerr << "Exception in WriteToCache : " << msg << std::endl;
    exit(EXIT_FAILURE);
  }
  catch (...)
  {
    std::cerr << "Exception in WriteToCache." << std::endl;
    exit(EXIT_FAILURE);
  }
  return true;
}
//Make it safer by checking the inpout first before doing invalid access
template <typename key, typename value>
const value &CPPCache<key, value>::ReadFromCacheAndUpdate(const key &my_key)
{
  const std::lock_guard<std::mutex> lock(cache_mutex);
  try
  {
    // Just to FULLY avoid a segmentation fault in any case, have to do a exists in cache check here also
    if (!this->ExistsInCache(my_key))
    {
      throw "Key not found while reading from cache";
    }
    this->key_list.erase(this->cache_key_value_map[my_key].first);
    this->key_list.push_front(my_key);
    auto val_to_return = this->cache_key_value_map[my_key].second;
    this->cache_key_value_map[my_key] = std::make_pair(this->key_list.begin(), val_to_return);
    return this->cache_key_value_map[my_key].second;
  }
  catch (std::exception &e)
  {
    std::cerr << "Standard exception in ReadFromCacheAndUpdate: " << e.what() << std::endl;
    exit(EXIT_FAILURE);
  }
  catch (const char *msg)
  {
    std::cerr << "Exception in ReadFromCacheAndUpdate : " << msg << std::endl;
    exit(EXIT_FAILURE);
  }
  catch (...)
  {
    std::cerr << "Exception in ReadFromCacheAndUpdate" << std::endl;
    exit(EXIT_FAILURE);
  }
}

template <typename key, typename value>
bool CPPCache<key, value>::CacheUpdate(const key &my_key, const value &my_value)
{
  const std::lock_guard<std::mutex> lock(cache_mutex);
  try
  {
    // Just to FULLY avoid a segmentation fault in any case, have to do a exists in cache check here also
    if (!this->ExistsInCache(my_key))
    {
      throw "Key not found while updating cache";
    }
    this->cache_key_value_map[my_key] = std::make_pair(this->key_list.begin(), my_value);
    return true;
  }
  catch (const char *msg)
  {
    std::cerr << "Exception in CacheUpdate : " << msg << std::endl;
    exit(EXIT_FAILURE);
  }
  catch (...)
  {
    std::cerr << "Exception in CacheUpdate." << std::endl;
    exit(EXIT_FAILURE);
  }
  return true;
}

template <typename key, typename value>
bool CPPCache<key, value>::ExistsInCache(const key &my_key) const
{
  try
  {
    return (this->cache_key_value_map.find(my_key) != this->cache_key_value_map.end());
  }
  catch (const char *msg)
  {
    std::cerr << "Exception in ExistsInCache : " << msg << std::endl;
    exit(EXIT_FAILURE);
  }
  catch (...)
  {
    std::cerr << "Exception in ExistsInCache" << std::endl;
    exit(EXIT_FAILURE);
  }
}
} // namespace my_cache
#endif