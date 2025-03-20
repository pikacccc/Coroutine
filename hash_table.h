#pragma once
#include <string>
#include <vector>
#include <forward_list>

template <class Key, class Value, class Hash = std::hash<Key>>
class hash_table
{
public:
    explicit hash_table(size_t num_buckets = 100) : num_buckets(num_buckets), table(num_buckets)
    {
    }

    void insert_or_update(const Key& key, const Value& value);
    bool try_get_value(const Key& key, Value& out_value) const;
    bool try_remove(const Key& key, Value& value);

private:
    struct key_value
    {
        Key key;
        Value value;

        key_value(const Key& key, const Value& value) : key(key), value(value)
        {
        }
    };

    std::vector<std::forward_list<key_value>> table;

    size_t num_buckets;

    Hash hash_map;

    size_t hash(const Key& key) const;
};

template <class Key, class Value, class Hash>
inline void hash_table<Key, Value, Hash>::insert_or_update(const Key& key, const Value& value)
{
    size_t hash_value = hash(key);
    std::forward_list<key_value>& bucket = table[hash_value];
    for (key_value& item : bucket)
    {
        if (item.key == key)
        {
            item.value = value;
            return;
        }
    }
    key_value temp_key_value{key, value};
    bucket.emplace_front(temp_key_value);
}

template <class Key, class Value, class Hash>
inline bool hash_table<Key, Value, Hash>::try_get_value(const Key& key, Value& out_value) const
{
    size_t hash_value = hash(key);
    const std::forward_list<key_value>& bucket = table[hash_value];
    for (const key_value& item : bucket)
    {
        if (item.key == key)
        {
            out_value = item.value;
            return true;
        }
    }

    return false;
}

template <class Key, class Value, class Hash>
inline bool hash_table<Key, Value, Hash>::try_remove(const Key& key, Value& value)
{
    size_t hash_value = hash(key);
    std::forward_list<key_value>& bucket = table[hash_value];
    auto it = bucket.before_begin();
    for (auto next_it = bucket.begin(); next_it != bucket.end(); ++next_it)
    {
        if (next_it->key == key)
        {
            value = next_it->value;
            bucket.erase_after(it);
            return true;
        }
        it = next_it;
    }
    return false;
}

template <class Key, class Value, class Hash>
inline size_t hash_table<Key, Value, Hash>::hash(const Key& key) const
{
    return hash_map(key) % num_buckets;
}
