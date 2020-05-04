#pragma once

#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "Shader.hpp"

namespace Engine
{
    class Program
    {
    private:
        std::vector<Shader*> _shaders;
        bool _linked;
        GLuint _programId;

    public:
        Program();
        ~Program();
        void AttachShader(std::string filename, GLenum shaderType);
        void LinkShader();
        GLuint GetProgramId() { return _programId; }
    };
}