cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(UserManager VERSION 1.0)

# 添加库
add_library(${PROJECT_NAME} STATIC UserManager.cpp)

# 设置头文件安装路径
install(FILES UserManager.h DESTINATION include)

# 设置库文件安装路径
install(TARGETS ${PROJECT_NAME} DESTINATION lib)