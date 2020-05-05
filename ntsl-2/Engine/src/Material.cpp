#include <Material.hpp>

namespace Engine
{
    Material::Material()
    {
        _program = new Program();
    }

    Material::~Material()
    {
        delete _program;
    }
}