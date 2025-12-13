# CMakeLists for user directory

# 递归搜索目录及子目录下的所有.c文件
file(GLOB_RECURSE USER_SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

# 添加源文件到目标
target_sources(${PROJECT_NAME} PRIVATE
    ${USER_SOURCES}
)

# 收集所有 .c 和 .h 文件
file(GLOB_RECURSE USER_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/*.[ch]
    ${CMAKE_CURRENT_SOURCE_DIR}/*.[ch]pp
)

# 初始化 include 路径
set(USER_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

# 提取所有文件所在目录
foreach(USER_FILE ${USER_FILES})
    get_filename_component(FILE_DIR ${USER_FILE} DIRECTORY)
    list(APPEND USER_INCLUDE_DIRS ${FILE_DIR})
endforeach()

#  去重
list(REMOVE_DUPLICATES USER_INCLUDE_DIRS)

# 添加所有用户 include 目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${USER_INCLUDE_DIRS}
)