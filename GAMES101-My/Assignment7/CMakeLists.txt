# 可执行程序
add_executable(Assignment7 main.cpp Object.hpp Vector.cpp Vector.hpp Sphere.hpp
    global.hpp Triangle.hpp Scene.cpp Scene.hpp Light.hpp AreaLight.hpp BVH.cpp
    BVH.hpp Bounds3.hpp Ray.hpp Material.hpp Intersection.hpp Renderer.cpp Renderer.hpp
)

# 添加模型
add_custom_command(TARGET Assignment7 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/../Assignment7/models"
        # 目标路径：末尾必须加 /models，才能把整个文件夹复制过去
        $<TARGET_FILE_DIR:Assignment7>/models
)

