# 可执行程序
add_executable (Assignment4 main.cpp)

# 头文件
target_include_directories(Assignment4 PRIVATE
    ${EIGEN3_INCLUDE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

# 链接 OpenCV
target_link_libraries(Assignment4 PRIVATE
    ${OpenCV_LIBS}
)

# 添加 dll
add_custom_command(TARGET Assignment4 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/../Plugins/OpenCV/x64/vc16/bin/opencv_world4120$<$<CONFIG:Debug>:d>.dll"
        $<TARGET_FILE_DIR:Assignment4>
)
