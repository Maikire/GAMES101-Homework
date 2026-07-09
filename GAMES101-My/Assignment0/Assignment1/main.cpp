#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <Eigen/Core>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 0, 1, 0, -eye_pos[1], 0, 0, 1,
        -eye_pos[2], 0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.

    float rad = rotation_angle * MY_PI / 180.0;

    model << cos(rad), -sin(rad), 0, 0,
             sin(rad), cos(rad), 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1;

    return model;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    // Students will implement this function

    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the projection matrix for the given parameters.
    // Then return it.

    float rad = eye_fov * MY_PI / 180.0f;

    float t = tan(rad / 2) * abs(zNear);
    float r = t * aspect_ratio;
    float l = -r;
    float b = -t;

    // 透视投影矩阵
    // GAMES101 和 OpenGL 中，相机的观察方向是 -z
    // 如果最后一行是 [0, 0, 1, 0]，那么变换后的 w 就等于原坐标的 z，所以 w 也是负值
    // 当 x 和 y 同时除以一个负数 w 时，它们在 NDC 空间中的符号都会反转
    // x 反转 = 左右镜像；y 反转 = 上下颠倒
    // 上下左右同时翻转 = 旋转 180 度
    // 所以用 [0, 0, -1, 0] 修正
    // 但是这样做会让 -z 反转到 +z，这会导致深度反转，所以将第三行整行 * -1 以抵消反转
    Eigen::Matrix4f persp;
    persp << zNear, 0, 0, 0,
             0, zNear, 0, 0,
             0, 0, -(zNear + zFar), zNear* zFar,
             0, 0, -1, 0;

    // 平移
    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -(r + l) / 2,
                 0, 1, 0, -(t + b) / 2,
                 0, 0, 1, -(zNear + zFar) / 2,
                 0, 0, 0, 1;

    // 缩放
    Eigen::Matrix4f scale;
    scale << 2 / (r - l), 0, 0, 0,
             0, 2 / (t - b), 0, 0,
             0, 0, 2 / (zNear - zFar), 0,
             0, 0, 0, 1;

    // 先透视，再正交
    projection = scale * translate * persp;

    return projection;
}

int main(int argc, const char** argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc >= 3) {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        }
        else
            return 0;
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a') {
            angle += 10;
        }
        else if (key == 'd') {
            angle -= 10;
        }
    }

    return 0;
}
