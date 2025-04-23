cmake_minimum_required(VERSION 3.5)

project(ZLToolKitParody)

# set c++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCES
    tools/variant.cpp
    tools/utils.cpp
)


# 测试文件列表
set(TEST_LIST
    tests/test_variant.cpp
    tests/test_ini.cpp
)

# 为每个测试文件生成可执行文件
foreach(TEST_FILE ${TEST_LIST})
    # 获取名称，NAME_WE获取不带后缀的文件名
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

    # 创建可执行文件
    add_executable(${TEST_NAME} ${SOURCES} ${TEST_FILE})

    # 头文件路径
    target_include_directories(${TEST_NAME}
        PRIVATE 
            ${PROJECT_SOURCE_DIR}
    )

endforeach()



