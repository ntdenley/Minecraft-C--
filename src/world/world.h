#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <unordered_map>
#include <queue>

#include <util/hashtuple.h>
#include <world/chunk.h>

class World {
    public:
        World();
        ~World();

        std::vector<Chunk::BlockType> GetChunkData(int chunk_x, int chunk_y, int chunk_z);
        void Update();

        Chunk* GetChunk(int chunk_x, int chunk_y, int chunk_z);

        static World world;
        unsigned int num_chunks = 0, num_chunks_rendered = 0;
    
    private:
        std::unordered_map<std::tuple<int, int, int>, Chunk> chunks;
        std::queue<glm::vec3> chunk_queue;
        int render_distance = 3;
        int render_height = 1;
        unsigned int chunks_loading = 0;
};

#endif