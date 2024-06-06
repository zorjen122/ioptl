# ioptl

ioptl 是一个由 C++17 编写的部分 STL 库，支持截至到 C++17 的标准容器，以及一些常见的数据结构（例如 string、AVL 树等），并提供一些元组件。

## 构建方法

1. 克隆项目：

   ```bash
   git clone https://github.com/zorjen122/ioptl.git
   ```

2. 进入项目目录：

   ```bash
   cd ioptl
   ```

3. 使用 `make` 命令构建项目：

   ```bash
   make && make run
   ```

## 使用方法

您可以通过手动导入、环境变量或添加包含路径的方式使用 ioptl。以下是使用 g++ 添加包含路径的示例：

```bash
#include "container/vectorF.h"
g++ --std=c++17 ./example.cpp -l ./ioptl/include
```

## 测试方法

您可以使用提供的测试脚本来进行测试：

```bash
./test_script_iop.py
```

## 文件结构

项目文件结构如下：

- `container/`: 包含容器相关的头文件
- `data_structure/`: 包含数据结构相关的头文件
- `utility/`: 包含工具函数和元组件的头文件

##### 文件结构

```
├── algo
│   ├── algobaseF.h
│   └── algorithmF.h
├── allocF.h
├── allocatorF.h
├── container
│   ├── arrayF.h
│   ├── dequeF.h
│   ├── forward_listF.h
│   ├── hashtableF.h
│   ├── initailzer_listF.h
│   ├── listF.h
│   ├── mapF.h
│   ├── multimapF.h
│   ├── multisetF.h
│   ├── pairF.h
│   ├── queueF.h
│   ├── rb_treeF.h
│   ├── setF.h
│   ├── stackF.h
│   ├── stringF.h
│   ├── unordered_mapF.h
│   ├── unordered_multimapF.h
│   ├── unordered_multisetF.h
│   └── unordered_setF.h
├── cppconfig.h
├── impl
│   ├── allocator_result.h
│   └── integer_sequence.h
├── iteratorF.h
├── memoryF.h
├── meta
│   ├── functionalF.h
│   └── metautilF.h
├── test
│   ├── .vscode
│   │   └── settings.json
│   ├── allocator_test
│   │   ├── a.out
│   │   ├── allocator.cc
│   │   └── demo.cc
│   ├── container_test
│   │   ├── a.out
│   │   ├── arr.cpp
│   │   ├── demo.cc
│   │   ├── deq.cpp
│   │   ├── list.cpp
│   │   ├── maps.cpp
│   │   ├── queue.cpp
│   │   ├── rbtree.cpp
│   │   ├── set.cpp
│   │   ├── stack.cpp
│   │   ├── string.cpp
│   │   └── vec.cpp
│   ├── demo.cc
│   ├── impl_test
│   │   └── integer_sq.cpp
│   ├── iterator_test
│   │   ├── a.out
│   │   ├── iter.cpp
│   │   └── iter_base.cpp
│   ├── rebind_question.cpp
│   ├── test_.h
│   └── util_test
│       ├── a.out
│       ├── func.cpp
│       └── iter.cpp
├── traits
│   ├── allocator_traitsF.h
│   └── type_traitsF.h
├── util
│   ├── exceptionF.h
│   ├── iterator_baseF.h
│   ├── iterator_utilF.h
│   └── utilityF.h
└── vec_iter.h
```

主要参考资料
---

- SGI STL (C++11 before)
- Folly STL (C++11 ~ 17)
- MSVC STL (C++17 ~ 20)



---

- 该项目是一个个体项目，bug检测和性能不足，欢迎同爱好者贡献代码和提出建议！
- 联系方式：1839916082@qq.com