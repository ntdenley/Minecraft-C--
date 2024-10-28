#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <glm/glm.hpp>
#include <vfx/shader.h>

#define CHUNK_SIZE 32

// Sometimes less is more... just get the damn thing working and then refactor
// later on.
class Chunk {
    public:

        enum BlockType {
            AIR,
            DIRT,
        };

        enum Direction {
            NORTH,
            SOUTH,
            WEST,
            EAST,
            BOTTOM,
            TOP
        };

        Chunk(glm::vec3 offset, Shader *shader);
        ~Chunk();

        void Generate();
        void Render();
        void AddFace(glm::ivec3 pos, Direction direction);
        BlockType GetBlockData(int x, int y, int z);
        bool InBounds(int x, int y, int z);

        BlockType blockData[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];

        glm::vec3 offset;
        int vertexCount = 0, indexCount = 0;
        bool generated = false;
        bool ready = false;


    private:
        unsigned int VBO, VAO, EBO;
        glm::vec3 worldPos;
        Shader *shader;

        std::vector<float> vertices;
        std::vector<unsigned int> indices;
};

#endif