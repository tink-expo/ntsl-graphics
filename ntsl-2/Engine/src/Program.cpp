#include <Program.hpp>

namespace Engine
{
    Program::Program()
    {
        _programId = glCreateProgram();
        _shaders = std::vector<Shader*>();
        _linked = false;
    }

    Program::~Program()
    {
        while (!_shaders.empty())
        {
            glDetachShader(_programId, _shaders.back()->getId());
            delete _shaders.back();
            _shaders.pop_back();
        }

        glDeleteProgram(_programId);
    }

    void Program::AttachShader(std::string filename, GLenum shaderType)
    {
        if (_linked)
        {
            std::cout << "Cannot attach more shaders to this program\n";
            return;
        }

        glUseProgram(_programId);
        Shader* s;
        try {
            s = new Shader(filename, shaderType);
            _shaders.push_back(s);
            glAttachShader(_programId, s->getId());
        }
        catch (std::exception)
        {
            std::cout << "Shader " << filename << " could not be attached to this program" << std::endl;;
        }
    }

    void Program::LinkShader()
    {
        if (_linked)
        {
            std::cout << "Program is already linked\n";
            return;
        }

        glLinkProgram(_programId);

        GLint GLerror;
        glGetProgramiv(_programId, GL_LINK_STATUS, &GLerror);
        if (GLerror == GL_FALSE)
        {
            std::cout << "Error linking program" << std::endl;
        }
        else
        {
            _linked = true;
        }
    }
}