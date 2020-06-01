# CMAKE 最低版本
cmake_minimum_required(VERSION 3.7)

# 项目名称
project(grammer)

# 设置自动生成 moc 文件
set(CMAKE_AUTOMOC ON)

# 设置 SOURCES 为源文件
set(SOURCES
    src/mainwindow.cpp
    src/body.cpp
    src/production.cpp
    src/grammer.cpp
)

# 头文件
set(HEADERS
    include/mainwindow.h
)

# add_executable() 须放在 target_link_libraries() 之前
# 向项目中添加使用指定源文件的可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} main.cpp)

add_executable(test test.cpp)

# 设置该目标的 build 命令中应包含的目录
# 相当于运行 g++ 时的 -I${PROJECT_SOURCE_DIR}/include
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# Qt5
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

if(Qt5_FOUND)
    message("-- Qt5 found")
else()
    message(FATAL ERROR "Cannot find Qt5")
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")