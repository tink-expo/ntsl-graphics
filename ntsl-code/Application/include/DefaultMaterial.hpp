#pragma once

#include <Material.hpp>

class DefaultMaterial : public Engine::Material
{
private:
public:
    void CreateMaterial();
    void UpdateUresolution(float res_w, float res_h);
    void UpdateUtime(float time);
};