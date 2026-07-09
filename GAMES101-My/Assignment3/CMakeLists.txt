# 可执行程序
add_executable (Assignment3 main.cpp rasterizer.hpp rasterizer.cpp global.hpp Triangle.hpp Triangle.cpp Texture.hpp Texture.cpp Shader.hpp OBJ_Loader.h)

# 头文件
target_include_directories(Assignment3 PRIVATE
    ${EIGEN3_INCLUDE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

# 链接 OpenCV
target_link_libraries(Assignment3 PRIVATE
    ${OpenCV_LIBS}
)

# 添加 dll
add_custom_command(TARGET Assignment3 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/../Plugins/OpenCV/x64/vc16/bin/opencv_world4120$<$<CONFIG:Debug>:d>.dll"
        $<TARGET_FILE_DIR:Assignment3>
)

# 添加模型
add_custom_command(TARGET Assignment3 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/../Assignment3/models"
        # 目标路径：末尾必须加 /models，才能把整个文件夹复制过去
        $<TARGET_FILE_DIR:Assignment3>/models
)
