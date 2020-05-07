#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <exception>

#include <GL/glew.h>

namespace Engine
{
    class Shader
    {
    private:
        GLenum _shaderType;
        std::string _source;
        GLuint _id;

    public:
        Shader(std::string source, GLenum shaderType);
        ~Shader();
        GLuint getId() { return _id; }

        static std::string _source_dir;
    };
}