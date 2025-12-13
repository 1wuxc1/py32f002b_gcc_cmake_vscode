# py32f002b_demo
这个项目是主要是学习cmake构建py32f002b的项目，并且顺便了解py32f002b的一些特性。
# 项目结构
```
py32f002b_demo/
├── CMakeLists.txt //cmake工程构建文件
├── sdk_config.cmake //py32f002b和其他sdk工程目录
├── CMakePresets.json //cmake工程预设文件(vscode使用)
├── py32f002bx5.ld //py32f002b的链接脚本(设置单片机内存配置)
├── startup_py32f002bxx.s //py32f002b的启动文件(设置单片机启动地址)
├── cmake
│   ├── gcc-arm-none-eabi.cmake //gcc-arm-none-eabi编译器配置文件
│   └── starm-clang.cmake //starm-clang编译器配置文件(没有用上)
├── README.md //项目说明
├── main.cpp //main运行文件
├── user/
│   ├── FreeRtosConfig //rtos配置文件
│   └── Hardware //硬件外设相关文件
│   └── py32_hal_conf //py32f002b的hal配置文件
│   └── CMakeLists.txt //user目录的cmake构建文件(用于构建user目录的文件)
└── .vscode/
    └── launch.json //vscode调试配置文件
```
# 构建环境 (gcc和对应工具链)
cmake //构建工具

arm-none-eabi-gcc //编译器


vscode //集成开发环境

mingw64 //链接器

openocd //调试工具(py32f002b的SDK里面的openocd)

Cortex Debug //vscode调试工具

freertos_sdk

py32f002b_sdk //py32f002b的SDK目录


# 联系
如果有什么问题或者好的建议可以联系我：
邮箱：w2302879076@163.com