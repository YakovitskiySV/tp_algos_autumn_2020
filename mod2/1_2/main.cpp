#include <iostream>
#include <string>
#include <vector>
#include <cassert>


class StringHasher {
public:
    StringHasher() : hashConstant(1) {}
    explicit StringHasher(const int& constant) : hashConstant(constant) {}
    StringHasher(const StringHasher&) = default;
    StringHasher& operator=(const StringHasher&) = default;
    ~StringHasher() = default;

    size_t operator()(const std::string& str) const {
        size_t hash = 0;
        for (size_t i = 0; i < str.size(); ++i) {
            hash = hash * hashConstant + str[i];
        }
        return hash;
  }
private:
    int hashConstant;
};

template <class Key, class Hasher, class HasherIterator>
class HashTable {
public:
    explicit HashTable(const Hasher& _hasher,
                       const HasherIterator& _hasherIterator);
    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;
    HashTable(HashTable&&) = delete;
    HashTable& operator=(HashTable&&) = delete;
    ~HashTable() = default;

    bool Add(const Key& key);
    bool Find(const Key& key) const;
    bool Delete(const Key& key);

private:
    void Grow();

    enum CellState {
        TAKEN,
        DELETED,
        EMPTY,
    };

    struct HashTableNode {
        HashTableNode() : state(EMPTY) {}
        HashTableNode(const Key& _data) : data(_data), state(TAKEN) {}
        void Set(const Key& _data) {
            data = _data;
            state = TAKEN;
        }
        void Delete() {
            state = DELETED;
        }
        Key data;
        CellState state;
    };

    std::vector<HashTableNode> table;
    Hasher hasher;
    HasherIterator hasherIterator;
    size_t keysCount;
    size_t tableSize;
};

template <class Key, class Hasher, class HasherIterator>
HashTable<Key, Hasher, HasherIterator>::HashTable(const Hasher& _hasher,
                                                  const HasherIterator& _hasherIterator) :
    table(0),
    hasher(_hasher),
    hasherIterator(_hasherIterator),
    keysCount(0),
    tableSize(0)
  {}


template <class Key, class Hasher, class HasherIterator>
void HashTable<Key, Hasher, HasherIterator>::Grow() {
    if (tableSize == 0) {
        tableSize = 8;
        table.resize(tableSize);
    }
    HashTable<Key, Hasher, HasherIterator> newHashTable(hasher, hasherIterator);
    newHashTable.table.resize(tableSize * 2);
    newHashTable.tableSize = tableSize * 2;
    for (size_t i = 0 ; i < table.size() ; ++i) {
        if (table[i].state == TAKEN) {
            newHashTable.Add(table[i].data);
        }
    }
    tableSize = std::move(newHashTable.tableSize);
    table = std::move(newHashTable.table);
    keysCount = std::move(newHashTable.keysCount);
}

template <class Key, class Hasher, class HasherIterator>
bool HashTable<Key, Hasher, HasherIterator>::Add(const Key& key) {
    if (keysCount >= tableSize * 0.75) {
        Grow();
    }
    size_t pos = hasher(key) % tableSize;
    bool delFound = false;
    size_t firstDelPos = 0;
    size_t step = (2 * hasherIterator(key) + 1) % tableSize;
    for (size_t i = 0 ; i < tableSize ; ++i) {
        switch(table[pos].state) {
            case EMPTY:
                if (delFound) {
                    table[firstDelPos].Set(key);
                } else {
                    table[pos].Set(key);
                }
                keysCount++;
                return true;
                break;

            case DELETED:
                if (!delFound) {
                    delFound = true;
                    firstDelPos = pos;
                }
                break;

            case TAKEN:
                if (table[pos].data == key) {
                    return false;
                }
                break;
        }
        pos = (pos + step) % tableSize;
    }
    assert(delFound);
    table[firstDelPos].Set(key);
    keysCount++;
    return true;
}

template <class Key, class Hasher, class HasherIterator>
bool HashTable<Key, Hasher, HasherIterator>::Delete(const Key& key) {
    size_t pos = hasher(key) % tableSize;
    if (table[pos].data == key) {
        if (table[pos].state == TAKEN) {
            table[pos].Delete();
            keysCount--;
            return true;
        } else {
            return false;
        }
    }
    size_t step = (2 * hasherIterator(key) + 1) % tableSize;
    for (size_t i = 0 ; i < tableSize ; ++i) {
        pos = (pos + step) % tableSize;
        switch (table[pos].state) {
        case EMPTY:
            return false;
            break;

        case DELETED:
            if (table[pos].data == key) {
                return false;
            }
            break;

        case TAKEN:
            if (table[pos].data == key) {
                table[pos].Delete();
                keysCount--;
                return true;
            }
            break;
        }
    }
    return false;
}

template <class Key, class Hasher, class HasherIterator>
bool HashTable<Key, Hasher, HasherIterator>::Find(const Key& key) const {
    size_t pos = hasher(key) % tableSize;
    if (table[pos].data == key) {
        if (table[pos].state == TAKEN) {
            return true;
        } else {
            return false;
        }
    }
    size_t step = (2 * hasherIterator(key) + 1) % tableSize;
    for (size_t i = 0 ; i < tableSize ; ++i) {
        pos = (pos + step) % tableSize;
        switch (table[pos].state)
        {
        case DELETED:
            if (table[pos].data == key) {
                return false;
            }
            break;

        case TAKEN:
            if (table[pos].data == key) {
                return true;
            }
            break;

        case EMPTY:
        return false;
        break;
        }
    }
    return false;
}

int main() {
    char command = '\0';
    std::string word = "";
    StringHasher hasher(73);
    StringHasher hasherIterator(37);
    HashTable<std::string, StringHasher, StringHasher> table(hasher, hasherIterator);
    while (std::cin >> command >> word) {
        switch (command) {
        case '+':
            std::cout << (table.Add(word) ? "OK" : "FAIL") << std::endl;
            break;
        
        case '-':
            std::cout << (table.Delete(word) ? "OK" : "FAIL") << std::endl;
            break;

        case '?':
            std::cout << (table.Find(word) ? "OK" : "FAIL") << std::endl;
            break;
        }
    }
    return 0;
}
