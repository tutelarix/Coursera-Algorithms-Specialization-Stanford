#ifndef HASHNODE_H
#define HASHNODE_H

template<typename Key, typename Value>
class HashNode {
public:
    HashNode(Key key = {}, Value value = {}) {
        m_Key = key;
        m_Value = value;
    }

    const Key & getKey() const {
        return m_Key;
    }

    void setKey(const Key & key) {
        m_Key = key;
    }

    const Value & getValue() const {
        return m_Value;
    }

    void setValue(const Value & value) {
        m_Value = value;
    }

private:
    Key m_Key;
    Value m_Value;

public:
    // For list, not good solution
    HashNode<Key, Value> *m_Previous = nullptr;
    HashNode<Key, Value> *m_Next = nullptr;

    // For open addressing, not a good solution
    bool isDeleted = false;
    bool isEmpty = true;
};

#endif // HASHNODE_H

