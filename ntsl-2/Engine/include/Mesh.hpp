#pragma once

#include <iostream>
#include <vector>
#include <exception>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Engine
{
    class Mesh
    {
    private:
        GLuint _vao;
        GLuint _vbo; // vertex buffer object of mesh
        GLuint _ibo; // index buffer object of mesh
        GLenum _drawMode; // draw mode
        std::vector<int> _attributeSizes;

        int _numElements = 0;
        bool _hasIndex = false;

        std::vector<float> _vertexData;
        std::vector<unsigned int> _indexData;

        void SetVbo(float* vertexData, size_t bufSize);
        void SetIbo(unsigned int* indexData, size_t bufSize);
    public:
        Mesh();
        ~Mesh();
        void CreateMesh();
        void AddVertexData(float data);
        void AddVertexData(glm::vec2 data);
        void AddVertexData(glm::vec3 data);
        void AddVertexData(glm::vec4 data);
        void AddIndexData(unsigned int data);
        void AddAttribute(int data);

        GLuint GetVao() { return _vao; }
        GLuint GetVbo() { return _vbo; }
        GLuint GetIbo() { return _ibo; }
        GLenum GetDrawMode() { return _drawMode; }
        void SetDrawMode(GLenum drawMode) { _drawMode = drawMode; }
        int GetNumElements() { return _numElements; }
        void SetNumElements(int numElements) { _numElements = numElements; }
        bool GetHasIndex() { return _hasIndex; }
        void SetHasIndex(bool hasIndex) { _hasIndex = hasIndex; }
    };
}