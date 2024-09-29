# 跳表项目
本项目参考 https://github.com/youngyangyang04/Skiplist-CPP \
下面为个人笔记。
## 项目笔记🙂
1. [Node类定义](https://github.com/pengfeiyan-sustech/SkipList/blob/master/%E9%A1%B9%E7%9B%AE%E7%AC%94%E8%AE%B0/1.Node%E7%B1%BB%E5%AE%9A%E4%B9%89.md#%E8%B7%B3%E8%A1%A8%E7%9A%84%E5%AE%9A%E4%B9%89)

2. [SkipList类定义](https://github.com/pengfeiyan-sustech/SkipList/blob/master/%E9%A1%B9%E7%9B%AE%E7%AC%94%E8%AE%B0/2.SkipList%E7%B1%BB%E5%AE%9A%E4%B9%89.md#skiplist%E7%B1%BB%E5%AE%9A%E4%B9%89)

3. [跳表的层级](https://github.com/pengfeiyan-sustech/SkipList/blob/master/%E9%A1%B9%E7%9B%AE%E7%AC%94%E8%AE%B0/3.%E8%B7%B3%E8%A1%A8%E7%9A%84%E5%B1%82%E7%BA%A7.md#%E8%B7%B3%E8%A1%A8%E7%9A%84%E5%B1%82%E7%BA%A7)

4. [跳表的插入](https://github.com/pengfeiyan-sustech/SkipList/blob/master/%E9%A1%B9%E7%9B%AE%E7%AC%94%E8%AE%B0/4.%E8%B7%B3%E8%A1%A8%E7%9A%84%E6%8F%92%E5%85%A5.md#%E8%B7%B3%E8%A1%A8%E7%9A%84%E6%8F%92%E5%85%A5)

5. [跳表的删除](https://github.com/pengfeiyan-sustech/SkipList/blob/master/%E9%A1%B9%E7%9B%AE%E7%AC%94%E8%AE%B0/5.%E8%B7%B3%E8%A1%A8%E7%9A%84%E5%88%A0%E9%99%A4.md#%E8%B7%B3%E8%A1%A8%E7%9A%84%E5%88%A0%E9%99%A4)

6. [跳表的展示](https://github.com/pengfeiyan-sustech/SkipList/blob/master/%E9%A1%B9%E7%9B%AE%E7%AC%94%E8%AE%B0/6.%E8%B7%B3%E8%A1%A8%E7%9A%84%E5%B1%95%E7%A4%BA.md#%E8%B7%B3%E8%A1%A8%E7%9A%84%E5%B1%95%E7%A4%BA)

7. [生成和读取持久化文件](https://github.com/pengfeiyan-sustech/SkipList/blob/master/%E9%A1%B9%E7%9B%AE%E7%AC%94%E8%AE%B0/7.%E7%94%9F%E6%88%90%E5%92%8C%E8%AF%BB%E5%8F%96%E6%8C%81%E4%B9%85%E5%8C%96%E6%96%87%E4%BB%B6.md#%E7%94%9F%E6%88%90%E5%92%8C%E8%AF%BB%E5%8F%96%E6%8C%81%E4%B9%85%E5%8C%96%E6%96%87%E4%BB%B6)

8. [添加互斥锁](https://github.com/pengfeiyan-sustech/SkipList/blob/master/%E9%A1%B9%E7%9B%AE%E7%AC%94%E8%AE%B0/8.%E6%B7%BB%E5%8A%A0%E4%BA%92%E6%96%A5%E9%94%81.md#%E6%B7%BB%E5%8A%A0%E4%BA%92%E6%96%A5%E9%94%81)

9. [压力测试](https://github.com/pengfeiyan-sustech/SkipList/blob/master/%E9%A1%B9%E7%9B%AE%E7%AC%94%E8%AE%B0/9.%E5%8E%8B%E5%8A%9B%E6%B5%8B%E8%AF%95.md#%E5%8E%8B%E5%8A%9B%E6%B5%8B%E8%AF%95)

项目运行方式
```
g++ --std=c++11 main.cpp -o app -pthread
g++ --std=c++11 stressTest.cpp -o app -pthread
```
