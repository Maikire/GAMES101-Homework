# 可执行程序
add_executable (Assignment0 main.cpp)

# 头文件
target_include_directories(Assignment0 PRIVATE
    ${EIGEN3_INCLUDE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

# 链接 OpenCV
target_link_libraries(Assignment0 PRIVATE
    ${OpenCV_LIBS}
)
