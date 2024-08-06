#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <iostream>
#include <mutex>
#include <fstream>
#include <cstring>

#define STORE_FILE "store/dumpFile"

std::mutex mtx;                     // 访问临界资源的互斥锁
std::string delimiter = ":";        // 键值对分隔符

template <typename K, typename V>
class Node
{
public:
    Node() {}
    Node(K key, V value, int level);
    ~Node();

    K get_key() const;
    V get_value() const;
    void set_value(V value);

    Node<K, V>** forward;
    int node_level;

private:
    K key;
    V value;
};

template <typename K, typename V> 
Node<K, V>::Node(const K key, const V value, int level) 
{
    this->key = key;
    this->value = value;
    this->node_level = level; 

    // level + 1, 因为数组索引是0 - level
    this->forward = new Node<K, V>* [level + 1];
    
	// 数组元素置为空 
    memset(this->forward, 0, sizeof(Node<K, V>* ) * (level + 1));
}

template <typename K, typename V> 
Node<K, V>::~Node() 
{
    delete[] forward;
}

template <typename K, typename V> 
K Node<K, V>::get_key() const 
{
    return key;
}

template <typename K, typename V> 
V Node<K, V>::get_value() const 
{
    return value;
}

template <typename K, typename V> 
void Node<K, V>::set_value(V value) 
{
    this->value = value;
}

// ========================= SkipList 类，用于管理存储节点==========================
template <typename K, typename V> 
class SkipList 
{
public: 
    SkipList(int level);
    ~SkipList();

    int get_random_level();
    Node<K, V>* create_node(K key, V value, int level);
    int insert_element(K key, V value);
    void display_list();
    bool search_element(K key);
    void delete_element(K key);
    void dump_file();
    void load_file();
    //递归删除节点
    void clear(Node<K,V>*);
    int size();

private:
    void get_key_value_from_string(const std::string& str, std::string* key, std::string* value);
    bool is_valid_string(const std::string& str);

private:    
    // 跳表最高级索引
    int _max_level;

    // 跳表当前索引级别 
    int _skip_list_level;

    // 指向头节点的指针
    Node<K, V> *_header;

    // 文件操作
    std::ofstream _file_writer;
    std::ifstream _file_reader;

    // 跳表当前存储的节点数
    int _element_count;
};

template <typename K, typename V> 
SkipList<K, V>::SkipList(int max_level) 
{
    this->_max_level = max_level;
    this->_skip_list_level = 0;
    this->_element_count = 0;

    K key;
    V value;
    this->_header = new Node<K, V>(key, value, _max_level);
}

template <typename K, typename V> 
SkipList<K, V>::~SkipList() 
{
    if (_file_writer.is_open()) 
    {
        _file_writer.close();
    }
    if (_file_reader.is_open()) 
    {
        _file_reader.close();
    }

    //递归删除跳表链条
    if (_header->forward[0]!=nullptr)
    {
        clear(_header->forward[0]);
    }
    delete(_header);
}

template <typename K, typename V>
void SkipList<K, V>::clear(Node<K, V> * cur)
{
    if (cur->forward[0]!=nullptr)
    {
        clear(cur->forward[0]);
    }
    delete(cur);
}

template <typename K, typename V>
int SkipList<K, V>::get_random_level()
{
    int k = 1;
    while (rand() % 2) 
    {
        k ++ ;
    }
    k = (k < _max_level) ? k : _max_level;
    return k;
}

template <typename K, typename V>
Node<K, V>* SkipList<K, V>::create_node(const K key, const V value, int level) 
{
    Node<K, V>* node = new Node<K, V>(key, value, level);
    return node;
}

template <typename K, typename V>
int SkipList<K, V>::insert_element(const K key, const V value) 
{
    mtx.lock();
    Node<K, V>* current = this->_header;

    // update数组存放每一级需要更新指向的节点
    Node<K, V>* update[_max_level + 1];
    memset(update, 0, sizeof(Node<K, V>* ) * (_max_level + 1));  
 
    for (int i = _skip_list_level; i >= 0; i -- ) 
    {
        while (current->forward[i] != nullptr && current->forward[i]->get_key() < key) 
        {
            current = current->forward[i]; 
        }
        update[i] = current;
    }

    // 到达最底层链表
    current = current->forward[0];
    // 如果要插入的键值对已经存在
    if (current != nullptr && current->get_key() == key) 
    {
        std::cout << "key: " << key << ", exists" << std::endl;
        mtx.unlock();
        return 1;
    }

    // 如果要插入的键值对不存在
    if (current == nullptr || current->get_key() != key ) 
    {
        // 为该节点生成一个随机的索引层级
        int random_level = get_random_level();

        if (random_level > _skip_list_level) 
        {
            for (int i = _skip_list_level + 1; i < random_level + 1; i ++ ) 
            {
                update[i] = _header;
            }
            _skip_list_level = random_level;
        }

        // 创建新节点 
        Node<K, V>* inserted_node = create_node(key, value, random_level);
        
        // 插入节点
        for (int i = 0; i <= random_level; i ++ ) 
        {
            inserted_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = inserted_node;
        }
        std::cout << "Successfully inserted key:" << key << ", value:" << value << std::endl;
        _element_count ++ ;
    }
    mtx.unlock();
    return 0;
}

template <typename K, typename V> 
void SkipList<K, V>::display_list() 
{
    std::cout << "\n*****Skip List*****"<<"\n"; 
    for (int i = 0; i <= _skip_list_level; i ++ ) 
    {
        Node<K, V>* node = this->_header->forward[i]; 
        std::cout << "Level " << i << ": ";
        while (node != NULL) 
        {
            std::cout << node->get_key() << ":" << node->get_value() << ";";
            node = node->forward[i];
        }
        std::cout << std::endl;
    }
}

template <typename K, typename V> 
void SkipList<K, V>::dump_file() 
{
    std::cout << "dump_file-----------------" << std::endl;
    _file_writer.open(STORE_FILE);
    Node<K, V>* node = this->_header->forward[0]; 

    while (node != nullptr) 
    {
        _file_writer << node->get_key() << ":" << node->get_value() << "\n";
        std::cout << node->get_key() << ":" << node->get_value() << ";\n";
        node = node->forward[0];
    }

    _file_writer.flush();
    _file_writer.close();
    return ;
}

template <typename K, typename V> 
void SkipList<K, V>::load_file() 
{
    _file_reader.open(STORE_FILE);
    std::cout << "load_file-----------------" << std::endl;
    std::string line;
    std::string* key = new std::string();
    std::string* value = new std::string();
    while (getline(_file_reader, line)) 
    {
        get_key_value_from_string(line, key, value);
        if (key->empty() || value->empty()) 
        {
            continue;
        }
        // 将键转化为int类型
        insert_element(stoi(*key), *value);
        std::cout << "key:" << *key << "value:" << *value << std::endl;
    }
    delete key;
    delete value;
    _file_reader.close();
}

template <typename K, typename V> 
int SkipList<K, V>::size() 
{ 
    return _element_count;
}

template <typename K, typename V>
void SkipList<K, V>::get_key_value_from_string(const std::string& str, std::string* key, std::string* value) 
{
    if (!is_valid_string(str)) 
    {
        return;
    }
    *key = str.substr(0, str.find(delimiter));
    *value = str.substr(str.find(delimiter)+1, str.length());
}

template <typename K, typename V>
bool SkipList<K, V>::is_valid_string(const std::string& str) 
{
    if (str.empty()) 
    {
        return false;
    }
    if (str.find(delimiter) == std::string::npos) 
    {
        return false;
    }
    return true;
}

template <typename K, typename V> 
void SkipList<K, V>::delete_element(K key) 
{
    mtx.lock();
    Node<K, V>* current = this->_header; 
    Node<K, V>* update[_max_level + 1];
    memset(update, 0, sizeof(Node<K, V>* ) * (_max_level + 1));

    for (int i = _skip_list_level; i >= 0; i--) 
    {
        while (current->forward[i] != nullptr && current->forward[i]->get_key() < key) 
        {
            current = current->forward[i];
        }
        update[i] = current;
    }

    current = current->forward[0];
    if (current != nullptr && current->get_key() == key) 
    {
        for (int i = 0; i <= _skip_list_level; i ++ ) 
        {
            if (update[i]->forward[i] != current) 
                break;

            update[i]->forward[i] = current->forward[i];
        }

        while (_skip_list_level > 0 && _header->forward[_skip_list_level] == 0) 
        {
            _skip_list_level --; 
        }

        std::cout << "Successfully deleted key "<< key << std::endl;
        delete current;
        _element_count --;
    }
    mtx.unlock();
    return;
}

template <typename K, typename V> 
bool SkipList<K, V>::search_element(K key) 
{
    std::cout << "search_element-----------------" << std::endl;
    Node<K, V>* current = _header;

    for (int i = _skip_list_level; i >= 0; i -- ) 
    {
        while (current->forward[i] && current->forward[i]->get_key() < key) 
        {
            current = current->forward[i];
        }
    }

    current = current->forward[0];

    if (current and current->get_key() == key) 
    {
        std::cout << "Found key: " << key << ", value: " << current->get_value() << std::endl;
        return true;
    }

    std::cout << "Not Found Key:" << key << std::endl;
    return false;
}

#endif