//
// Created by Göksu Güvendiren on 2019-05-14.
//

#include "Scene.hpp"


void Scene::buildBVH() {
    printf(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

Intersection Scene::intersect(const Ray &ray) const
{
    return this->bvh->Intersect(ray);
}

void Scene::sampleLight(Intersection &pos, float &pdf) const
{
    float emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
        }
    }
    float p = get_random_float() * emit_area_sum;
    emit_area_sum = 0;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        if (objects[k]->hasEmit()){
            emit_area_sum += objects[k]->getArea();
            if (p <= emit_area_sum){
                objects[k]->Sample(pos, pdf);
                break;
            }
        }
    }
}

bool Scene::trace(
        const Ray &ray,
        const std::vector<Object*> &objects,
        float &tNear, uint32_t &index, Object **hitObject)
{
    *hitObject = nullptr;
    for (uint32_t k = 0; k < objects.size(); ++k) {
        float tNearK = kInfinity;
        uint32_t indexK;
        Vector2f uvK;
        if (objects[k]->intersect(ray, tNearK, indexK) && tNearK < tNear) {
            *hitObject = objects[k];
            tNear = tNearK;
            index = indexK;
        }
    }


    return (*hitObject != nullptr);
}

// Implementation of Path Tracing
Vector3f Scene::castRay(const Ray& ray, int depth) const
{
    Intersection inter = intersect(ray);

    if (!inter.happened)
    {
        return Vector3f(0.0f, 0.0f, 0.0f);
    }

    // 如果光线直接打在光源上（且是直接从相机射出的光线）
    if (inter.m->hasEmission())
    {
        // 间接光照打到光源的情况已在 L_dir 中采样计算过，所以深度大于 0 时不重复计算
        // 如果有 BSDF 等材质，这样写可能会漏掉一些路径。例如 相机 -> 镜子 -> 光源
        return depth == 0 ? inter.m->getEmission() : Vector3f(0.0f, 0.0f, 0.0f);
    }

    Vector3f L_dir(0.0f, 0.0f, 0.0f);
    Vector3f L_indir(0.0f, 0.0f, 0.0f);

    // 直射光采样 (L_dir)
    Intersection light_pos;
    float pdf_light = 0.0f;
    sampleLight(light_pos, pdf_light);

    Vector3f p = inter.coords;
    Vector3f xx = light_pos.coords;
    Vector3f ws = normalize(xx - p);

    //Vector3f wo = normalize(-ray.direction);
    Vector3f wo = normalize(ray.direction);

    // 加上一个小偏移量避免自交引起的表面噪点 (Shadow acne)
    Vector3f p_shifted = p + inter.normal * 0.0005f;
    if (dotProduct(ray.direction, inter.normal) > 0)
    {
        p_shifted = p - inter.normal * 0.0005f;
    }

    float dist_to_light = (xx - p).norm();
    Intersection test_inter = intersect(Ray(p_shifted, ws));

    // 如果交点距离光源足够近，说明没有被其它物体遮挡
    if (test_inter.happened && dist_to_light - test_inter.distance < 0.01f)
    {
        Vector3f f_r = inter.m->eval(wo, ws, inter.normal);
        float cosn = std::max(0.0f, dotProduct(ws, inter.normal));
        float cosnn = std::max(0.0f, dotProduct(-ws, light_pos.normal));

        L_dir = light_pos.emit * f_r * cosn * cosnn / (dist_to_light * dist_to_light) / pdf_light;
    }

    // 间接光追踪 (L_indir)
    if (get_random_float() <= RussianRoulette)
    {
        Vector3f wi = inter.m->sample(wo, inter.normal).normalized();
        Ray indir_ray(p_shifted, wi);
        Intersection ininter = intersect(indir_ray);

        // 如果打到了物体，并且打到的不是光源（避免Double Counting）
        if (ininter.happened && !ininter.m->hasEmission())
        {
            float pdf_indir = inter.m->pdf(wo, wi, inter.normal);

            // 避免极端除0导致的亮点(Fireflies)
            if (pdf_indir > 0.0001f) 
            {
                Vector3f f_r_indir = inter.m->eval(wo, wi, inter.normal);
                float cos_indir = std::max(0.0f, dotProduct(wi, inter.normal));

                // 递归调用 castRay 来得到弹射后路径对该点的辐射度
                Vector3f q_radiance = castRay(indir_ray, depth + 1);

                L_indir = q_radiance * f_r_indir * cos_indir / pdf_indir / RussianRoulette;
            }
        }
    }

    return L_dir + L_indir;
}
