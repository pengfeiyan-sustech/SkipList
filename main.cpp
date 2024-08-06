#include <iostream>
#include "skipList.h"
#define FILE_PATH "./store/dumpFile"

int main() {

    // 键值中的key用int型，如果用其他类型，需要自定义比较函数
    // 而且如果修改key的类型，同时需要修改skipList.load_file函数
    SkipList<int, std::string> skipList(6);
	skipList.insert_element(1, "你好"); 
	skipList.insert_element(3, "欢迎"); 
	skipList.insert_element(7, "来到"); 
	skipList.insert_element(8, "阿飞的"); 
	skipList.insert_element(9, "CPP"); 
	skipList.insert_element(19, "开源项目"); 
	skipList.insert_element(19, "赶快关注吧！"); 

    std::cout << "skipList size:" << skipList.size() << std::endl;

    skipList.dump_file();

    // skipList.load_file();

    skipList.search_element(9);
    skipList.search_element(18);


    skipList.display_list();

    skipList.delete_element(3);
    skipList.delete_element(7);

    std::cout << "skipList size:" << skipList.size() << std::endl;

    skipList.display_list();
}