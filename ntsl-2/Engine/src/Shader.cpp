#include <Shader.hpp>

namespace Engine
{
    Shader::Shader(std::string source, GLenum shaderType) : _id(0)
    {
        // Load shader file
        source = _source_dir + source;
        std::ifstream fp(source);
        if (fp.is_open())
        {
            std::stringstream buf;
            buf << fp.rdbuf();

            if ((_id = glCreateShader(shaderType)) != 0)
            {
                _source = buf.str();
                const char* sourceChar = _source.c_str();
                glShaderSource(_id, 1, &sourceChar, NULL);
            }
            else
            {
                std::cout << "GL could not create new shader\n";
                throw std::exception();
            }
        }
        else
        {
            std::cout << "GLSL source is not found\n";
            throw std::exception();
        }

        // Compile the shader
        glCompileShader(_id);
        GLint status = 0;
        glGetShaderiv(_id, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            GLint infoLogLength;
            glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &infoLogLength);
            GLchar* infoLog = (GLchar*)malloc(sizeof(GLchar) * (infoLogLength + 1));
            glGetShaderInfoLog(_id, infoLogLength, NULL, infoLog);
            std::cout << "Shader could not compile: " << infoLog << "\n";
            throw std::exception();

            free(infoLog);
        }
    }

    Shader::~Shader()
    {
        glDeleteShader(_id);
    }

    std::string Shader::_source_dir = "";
}