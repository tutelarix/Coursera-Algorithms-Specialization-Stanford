#ifndef LIST_HPP
#define LIST_HPP

#include"hash_node.hpp"

template<typename Key, typename Value>
class List {
public:
    void insert(const Key &key, const Value &value) {
        auto *item = new HashNode<Key, Value>(key, value);

        if(m_Count == 0) {
            m_Start = item;
            m_End = item;
        } else {
            m_End->m_Next = item;
            item->m_Previous = m_End;
            m_End = item;
        }
        m_Count++;
    }

    bool remove(const Key &key){
        auto item = find(key);

        if(item != nullptr){
            if(item->m_Previous != nullptr ) {
                item->m_Previous->m_Next = item->m_Next;
            }

            if(item->m_Next != nullptr) {
                item->m_Next->m_Previous = item->m_Previous;
            }

            if(item == m_Start) {
                m_Start = item->m_Next;
            }

            if(item == m_End) {
                m_End = item->m_Previous;
            }

            m_Count--;
            delete item;

            return true;
        }
        return false;
    }

    HashNode<Key, Value>* find(const Key &key) const {
        if(m_Start ==nullptr) return nullptr;

        auto item = m_Start;
        while(item!=nullptr && item->getKey() != key) {
            item = item->m_Next;
        }

        return item;
    }

    unsigned int getCount() {
        return m_Count;
    }


private:
    HashNode<Key, Value> *m_Start = nullptr;
    HashNode<Key, Value> *m_End = nullptr;
    unsigned int m_Count = 0;
};


#endif // LIST_HPP

