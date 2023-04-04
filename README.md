# KV数据库
a tiny database using skiplist structure,based on C++20

轻量级键值对存储引擎，基于跳表数据结构实现

# 项目亮点
* 该项目内存配置部分完全采用智能指针，除索引部分外不使用任何原生指针，不存在内存泄漏问题
* 使用RAII手法包装的mutex_lock，自动lock和unlock保护临界区，实现skiplist的线程安全
* 对key的类型进行抽象，使用C++20标准的concept和require关键字，实现对key类型的检验，只要key类型实现了比较操作就可以作为键的类型。默认key的类型是int

# 项目文件
src 源代码目录
  * main.cpp  此处main函数做功能测试，可以在main.cpp中参考skiplist用法
  * skiplist.h  项目具体实现
  * mutex_lock.h  RAII手法包装的互斥量
  * CMakeLists.txt  cmake列表文件
  
doc 文档目录，具体实现文档和项目细节可以在此处阅读

store 数据存储目录

stress_test 测试目录

# 提供接口
insert(K, V)，插入节点

print()，打印所有节点和各层级索引

find(K)，查找键是否存在，如果存在返回指向键值对的指针，不存在返回空

erase(K)，删除键对应的节点，若不存在该键则不删除

dump_file()，数据落盘

load_file()，从文件中读取数据，存入跳表

size() ，返回跳表中元素数量

# 性能测试

插入10w条数据，15.64秒

查找10w条数据，14.81秒

删除10w条数据，10.21秒

![GM6@Y2@JPAF7G4OO2 O23{6](https://user-images.githubusercontent.com/76865160/229724737-14b58a91-c905-4a60-9817-14f91b5f5a70.jpg)

# 运行方式

若想使用本项目，只需包含skiplist.h头文件即可

```
#include "skiplist.h"
```

整个项目编译运行方式如下

拷贝项目后，在skiplist目录下

编译
```
cmake -S . -B ./build
cmake --build ./build
```

运行测试
```
./bin/skiplist
```


> 本项目基于代码随想录的Skiplist-CPP优化而来 https://github.com/youngyangyang04/Skiplist-CPP。 
