# 跳表的定义

## 1、Node 类
在开发基于跳表的K-V存储引擎，设计用于实际存储数据的Node类（节点）时，需要考虑以下三个因素： 
```
1. 键值存储机制：如何存储键和对应的值 
2. 层级确定：如何确定节点存在于跳表中的哪些层级
3. 跳转机制实现：跳表的搜索操作核心在于节点间的跳转 
```
针对前两点，Node 类需要包含两个主要私有属性：key 和 value，分别用于存储键和值。此外，还需要一个公共属性 node_level，用于标识节点在跳表中的层级位置。
```
1. 当 node_level = 1 时，标识当前的节点只会出现在跳表中的第 1 层。
2. 当 node_level = 2 时，标识当前的节点会出现在跳表中的第 2 层和第 1 层。
3. 以此类推
```

此时，Node 类的结构如下：
```cpp
template <typename K, typename V>
class Node {
public:
    int node_level;
private:
    K key;
    V value;
}
```
针对第三点，节点间的跳转机制，可以分为两类：
```
1. 同一节点的多层跳转：在相同键值、不同层级节点之间的跳转
2. 不同节点的单层跳转：在相同层级、不同键值节点之间的跳转
```

此时，节点定义如下：
```cpp
template <typename K, typename V>
class Node {
public:
    Node<K, V>** forward; // 在 C++ 中，二维指针等价于指针数组 
    int node_level;
private:
    K key;
    V value;
};
```
对forward数组的说明：
```
假设一个节点在跳表中的层级为 3，那么这个节点的 forward 指针数组的大小为 3。
其 forward[0] 指向该节点在第一层的下一个节点；
forward[1] 指向该节点在第二层的下一个节点，
forward[2] 指向该节点在第三层的下一个节点。
```
节点跳转机制的实际运行：\
同一层级内节点的跳转
```cpp
/**
 * 遍历跳表的底层链表
 * current : 指向当前遍历节点的指针
 */
Node<K,V>* current = head; // 假设 head 是跳表第一层的头节点
while (current->forward[0] != nullptr) {
    // 通过迭代的方式，实现同一层内的不同节点之间的跳转
    current = current->forward[0];
}
```
不同层级内同一节点的跳转
```cpp
/**
 * 同一个节点，不同层级之间的跳转
 * node : 当前节点
 * n : 节点所在的最高层级
 */
Node<K,V>* node; // 假设 node 是当前节点
int n = node->forward.size(); // 假设 forward 是动态数组
for (int i = n - 1; i >= 0; i--) {
    // 通过变更数组下标进行同一个节点在不同层级之间的跳转
    Node<K,V>* current = node->forward[i];
}
```
定义了 Node 类的关键属性之后，还需要定义一些成员函数，例如获取/设置 key 对应的 value、构造函数的实现、析构函数的实现。
```cpp
template <typename K, typename V>
class Node {
public:
    Node() {}
    Node(K k, V v, int);
    ~Node();
    K get_key() const;
    V get_value() const;
    void set_value(V);
    Node<K, V> **forward;
    int node_level;
private:
    K key;
    V value;
};

template <typename K, typename V>
Node<K, V>::Node(const K k, const V v, int level) {
    this->key = k;
    this->value = v;
    this->node_level = level;
    this->forward = new Node<K, V> *[level + 1];
    memset(this->forward, 0, sizeof(Node<K, V> *) * (level + 1));
};

template <typename K, typename V>
Node<K, V>::~Node() {
    delete[] forward;
};

template <typename K, typename V>
K Node<K, V>::get_key() const {
    return key;
};

template <typename K, typename V>
V Node<K, V>::get_value() const {
    return value;
};

template <typename K, typename V>
void Node<K, V>::set_value(V value) {
    this->value = value;
};
```