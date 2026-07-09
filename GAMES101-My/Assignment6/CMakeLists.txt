# 可执行程序
add_executable(Assignment6 main.cpp Object.hpp Vector.cpp Vector.hpp Sphere.hpp
    global.hpp Triangle.hpp Scene.cpp Scene.hpp Light.hpp AreaLight.hpp BVH.cpp
    BVH.hpp Bounds3.hpp Ray.hpp Material.hpp Intersection.hpp Renderer.cpp Renderer.hpp
)

# 头文件
target_include_directories(Assignment6 PRIVATE
    ${EIGEN3_INCLUDE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

# 链接 OpenCV
target_link_libraries(Assignment6 PRIVATE
    ${OpenCV_LIBS}
)

# 添加 dll
add_custom_command(TARGET Assignment6 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/../Plugins/OpenCV/x64/vc16/bin/opencv_world4120$<$<CONFIG:Debug>:d>.dll"
        $<TARGET_FILE_DIR:Assignment6>
)

# 添加模型
add_custom_command(TARGET Assignment6 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/../Assignment6/models"
        # 目标路径：末尾必须加 /models，才能把整个文件夹复制过去
        $<TARGET_FILE_DIR:Assignment6>/models
)
