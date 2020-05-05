#include <Mesh.hpp>

namespace Engine
{
    Mesh::Mesh() : _drawMode(GL_TRIANGLES), _hasIndex(false)
    {
		_vbo = 0;
		_vao = 0;

        _vertexData = std::vector<float>();
        _indexData = std::vector<unsigned int>();
        _attributeSizes = std::vector<int>();
        _numElements = 0;
    }

    Mesh::~Mesh()
    {
        _vertexData.clear();
        _indexData.clear();
        _attributeSizes.clear();

        glDeleteBuffers(1, &_vbo);
        if (_hasIndex)
        {
            glDeleteBuffers(1, &_ibo);
        }

        glBindVertexArray(_vao);
        for (int i = 0; i < _attributeSizes.size(); i++)
        {
            glDisableVertexAttribArray(i);
        }

        glBindVertexArray(0);
        glDeleteVertexArrays(1, &_vao);
    }

    void Mesh::CreateMesh()
    {
        glGenBuffers(1, &_vbo);
        SetVbo(&(_vertexData[0]), sizeof(float) * _vertexData.size());

        if (_hasIndex)
        {
            glGenBuffers(1, &_ibo);
            SetIbo(&(_indexData[0]), sizeof(unsigned int) * _indexData.size());
        }
    }

    void Mesh::SetVbo(float* vertexData, size_t bufSize)
    {
        if (_attributeSizes.empty() == true)
        {
            std::cout << "attributes must be set to generate VBO\n";
            throw std::exception();
        }

        // Copy host data to gpu
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, bufSize, vertexData, GL_STATIC_DRAW);
		
        glGenVertexArrays(1, &_vao);

        // Set VAO
        int vertexSize = 0;
        for (int i = 0; i < _attributeSizes.size(); i++)
        {
            vertexSize += _attributeSizes[i];
        }

        vertexSize *= sizeof(float);

        glBindVertexArray(_vao);
        int currentOffset = 0;
        for (int i = 0; i < _attributeSizes.size(); i++)
        {
            glVertexAttribPointer(i, _attributeSizes[i], GL_FLOAT, GL_FALSE, vertexSize, (void*)(sizeof(float) * currentOffset));
            glEnableVertexAttribArray(i);
            currentOffset += _attributeSizes[i];
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void Mesh::SetIbo(unsigned int* indexData, size_t bufSize)
    {
        if (!_hasIndex)
        {
            std::cout << "This mesh is not index mode\n";
            throw std::exception();
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufSize, indexData, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        GLenum glError = glGetError();
        if (glError != GL_NO_ERROR)
        {
            std::cout << "SetIbo error" << glError << "\n";
            throw std::exception();
        }
    }

    void Mesh::AddVertexData(float data)
    {
        _vertexData.push_back(data);
    }

    void Mesh::AddVertexData(glm::vec2 data)
    {
        _vertexData.push_back(data.x);
        _vertexData.push_back(data.y);
    }

    void Mesh::AddVertexData(glm::vec3 data)
    {
        _vertexData.push_back(data.x);
        _vertexData.push_back(data.y);
        _vertexData.push_back(data.z);
    }

    void Mesh::AddVertexData(glm::vec4 data)
    {
        _vertexData.push_back(data.x);
        _vertexData.push_back(data.y);
        _vertexData.push_back(data.z);
        _vertexData.push_back(data.w);
    }

    void Mesh::AddIndexData(unsigned int data)
    {
        _indexData.push_back(data);
    }

    void Mesh::AddAttribute(int data)
    {
        _attributeSizes.push_back(data);
    }
}