#ifndef HASH_TABLES_HPP
#define HASH_TABLES_HPP

#include "hash_node.hpp"
#include "list.hpp"

//#define TABLE_SIZE 20021

#define TABLE_SIZE 50047

//#define TABLE_SIZE 2000029

//List<int, int> m_Array1[TABLE_SIZE];

template <typename Value>
class HashTableChaining {
public:

    void insert(long long int key, const Value & value) {
        unsigned int compKey = getCompressedKey(key);
        auto &list = m_Array1[compKey];

        if(list.getCount() == 0) {
            list.insert(key, value);
            m_Count++;
        } else {
            auto node = list.find(key);
            if(node == nullptr) {
                list.insert(key, value);
                m_Count++;
                m_CollisionsCount++;
            } else {
                node->setValue(value);
            }
        }
    }

    bool lookup(long long int key, Value & returnValue) {
        unsigned int compKey = getCompressedKey(key);
        auto &list = m_Array1[compKey];
        auto node = list.find(key);
        if(node)
            returnValue = node->getValue();
        return (node != nullptr);
    }

    void del(long long int key) {
        unsigned int compKey = getCompressedKey(key);
        auto &list = m_Array1[compKey];
        if(list.remove(key))
        {
            m_Count--;
            if(list.getCount()>0) {
                m_CollisionsCount--;
            }
        }
    }

    float getLoad() {
        return (float)m_Count/TABLE_SIZE;
    }

    int getCount() {
        return m_Count;
    }

    int getCollisionsCount(){
        return m_CollisionsCount;
    }

    int getCompressedKey(long long int key) {
        key = key>0?key:-key;
        return key % TABLE_SIZE;
    }

private:

    List<int, Value> m_Array1[TABLE_SIZE];
    int m_Count = 0;
    int m_CollisionsCount = 0;
};


//HashNode<long long int, int> m_Array2[TABLE_SIZE];

template <typename Value>
class HashTableOpenAddressing {
public:
    void insert(long long int key, const Value & value) {
        long long int convKey = getConvertedKey(key);
        unsigned int compKey = getCompressedKey(convKey);

        bool possibleCollision = false;
        if(!m_Array2[compKey].isEmpty && !m_Array2[compKey].isDeleted) {
            possibleCollision = true;
        }

        while(!m_Array2[compKey].isEmpty && !m_Array2[compKey].isDeleted) {
            if (m_Array2[compKey].getKey() == key) {
                m_Array2[compKey].setValue(value);
                break;
            }
            // Linear probing
            compKey = getCompressedKey(++convKey);
        }

        if(m_Array2[compKey].isEmpty || m_Array2[compKey].isDeleted) {
            if(m_Array2[compKey].isEmpty) {
                m_Count++;
                m_FullCount++;
                if(possibleCollision)
                    m_CollisionsCount++;
            }

            m_Array2[compKey].setKey(key);
            m_Array2[compKey].setValue(value);
            m_Array2[compKey].isEmpty=false;
            m_Array2[compKey].isDeleted=false;
        }
    }

    bool lookup(long long int key, Value & returnValue) {
        long long int convKey = getConvertedKey(key);
        unsigned int compKey = getCompressedKey(convKey);
        unsigned int start = compKey;


        do {
            if(m_Array2[compKey].isEmpty) {
                break;
            }

            if(m_Array2[compKey].isDeleted) {
                compKey = getCompressedKey(++convKey);
                continue;
            }

            if(m_Array2[compKey].getKey() == key) {
                break;
            }

            compKey = getCompressedKey(++convKey);
        } while(start!=compKey);

        if(start==compKey && m_Array2[compKey].getKey()!=key)
            return false;

        if(!m_Array2[compKey].isEmpty) {
            returnValue = m_Array2[compKey].getValue();
            return true;
        }

        return false;
    }

    void del(long long int key) {
        long long int convKey = getConvertedKey(key);
        unsigned int compKey = getCompressedKey(convKey);
        unsigned int start = compKey;

        do {
            if(m_Array2[compKey].isEmpty) {
                break;
            }

            if(m_Array2[compKey].isDeleted) {
                compKey = getCompressedKey(++convKey);
                continue;
            }

            if(m_Array2[compKey].getKey() == key) {
                break;
            }

            compKey = getCompressedKey(++convKey);
        } while(start!=compKey);

        if(start==compKey && m_Array2[compKey].getKey()!=key)
            return;

        if(!m_Array2[compKey].isEmpty) {
            m_Array2[compKey].isDeleted = true;
            m_Count--;
        }
    }

    float getLoad() {
        return (float)m_FullCount/TABLE_SIZE;
    }

    int getCount() {
        return m_Count;
    }

    int getCollisionsCount(){
        return m_CollisionsCount;
    }

    long long int getConvertedKey(long long int key) {
        key = key>0?key:-key;
        return key;
    }

    int getCompressedKey(long long int key) {
        //key = key>0?key:-key;
        return key % TABLE_SIZE;
    }

private:

    HashNode<long long int, int> m_Array2[TABLE_SIZE];
    int m_Count = 0;
    int m_FullCount = 0;
    int m_CollisionsCount = 0;
};

#endif // HASH_TABLES_HPP

