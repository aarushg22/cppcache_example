#include <bits/stdc++.h>
#include <Cppcache.hpp>


namespace my_cache {
// Constructor
template <typename key, typename value>
CPPCache<key, value>::CPPCache(int n)
{
    this->cache_size = n;
}

// In case of a miss, write recent used value to cache
template <typename key, typename value>
bool CPPCache<key, value>::WriteToCache(const key& my_key, const value& my_value)
{
    try{
        this->key_list.push_front(my_key);
        this->cache_key_value_map[my_key] = std::make_pair(this->key_list.begin(), my_value);    
        if(this->key_list.size()>this->cache_size){
            auto last_key = this->key_list.back();
            cache_key_value_map.erase(last_key);
            this->key_list.pop_back();
        }
    } catch (...) {
        std::cerr << "Exception in Write" << std::endl;
        return false;
    }
    return true;
}
template <typename key, typename value>
const value& CPPCache<key, value>::GetFromCacheAndUpdate(const key& my_key) {
    try{
        this->key_list.erase(this->cache_key_value_map[my_key].first);
        this->key_list.push_front(my_key);
        auto val_to_return = this->cache_key_value_map[my_key].second;
        this->cache_key_value_map[my_key] = std::make_pair(this->key_list.begin(), val_to_return);
        return val_to_return;
    } catch (...){
        std::cerr << "Exception in Get" << std::endl;
        return false;
    }
}

template <typename key, typename value>
bool CPPCache<key, value>::ExistsInCache(const key& my_key) const
{
    return (this->cache_key_value_map.find(my_key) != this->cache_key_value_map.end());
}

}