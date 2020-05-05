#pragma once

#include <glm/glm.hpp>

#include <Program.hpp>

namespace Engine
{
    class Material
    {
    protected:
        Program* _program;
		Program* _picking_program;


    public:
        Material();
        ~Material();
        Program *GetProgram() { return _program; }
		Program *GetPickingProgram() { return _picking_program; }
    };
}