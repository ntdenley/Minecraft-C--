#include <world/world.h>
#include <iostream>

using namespace std;

World *World::world = nullptr;

World::World(Shader *shader) : shader(shader), running(true) {
    chunk_thread = thread(&World::GenerateChunks, this);
}

World::~World() {
    running = false;
    if (chunk_thread.joinable()) {
        chunk_thread.join();
    }
}

void World::GenerateChunks(){
    while (running){
        unique_lock<mutex> lock(chunk_mutex);
        if(!chunk_queue.empty()){
            glm::ivec3 chunk_pos = chunk_queue.front();
            auto chunk_key = make_tuple(chunk_pos.x, chunk_pos.y, chunk_pos.z);
            chunk_queue.pop();
            chunks_to_render.erase(chunk_key);
            lock.unlock();

            // Load the chunk
            Chunk *chunk = new Chunk(chunk_pos, shader);
            chunk->Generate();

            lock.lock();
            if (chunks.find(chunk_key) == chunks.end()) {
                chunks[chunk_key] = chunk;
            }
            lock.unlock();
        } else {
            lock.unlock();
            this_thread::sleep_for(chrono::milliseconds(10));
        }
    }
}

Chunk* World::GetChunk(int chunk_x, int chunk_y, int chunk_z) {
    lock_guard<mutex> lock(chunk_mutex);
    auto it = chunks.find(std::make_tuple(chunk_x, chunk_y, chunk_z));
    if (it != chunks.end()) {
        return it->second;
    }
    return nullptr;
}

void World::Update(glm::vec3 player_pos) {
    // Get the chunk that the player is in
    int chunk_x = (int)player_pos.x / CHUNK_SIZE;
    int chunk_y = (int)player_pos.y / CHUNK_SIZE;
    int chunk_z = (int)player_pos.z / CHUNK_SIZE;

    // Load the chunks around the player
    for (int x = -render_distance; x <= render_distance; x++)
    for (int y = -render_height; y <= render_height; y++)
    for (int z = -render_distance; z <= render_distance; z++) {
        // Get the chunk position
        int new_chunk_x = chunk_x + x;
        int new_chunk_y = chunk_y + y;
        int new_chunk_z = chunk_z + z;

        // Create the key
        tuple<int, int, int> chunk_tup = make_tuple(new_chunk_x, new_chunk_y, new_chunk_z);
        
        // Check if the chunk is already loaded
        bool chunk_loaded = false;
        {   
            lock_guard<mutex> lock(chunk_mutex);
            // Chunk is loaded, set flag
            if(chunks.find(chunk_tup) != chunks.end())
                chunk_loaded = true;

            // Chunk is not loaded, add it to the queue
            else if (chunks_to_render.find(chunk_tup) == chunks_to_render.end()){
                chunk_queue.push(glm::ivec3(new_chunk_x, new_chunk_y, new_chunk_z));
                chunks_to_render.insert(chunk_tup);
                chunks_loading++;
            }
        }

        // Render the chunk
        if(chunk_loaded){
            Chunk* chunk = nullptr;
            {
                lock_guard<mutex> lock(chunk_mutex);
                chunk = chunks[chunk_tup];
            }
            if (chunk) {
                chunk->Render();
            }
        }
    }
}
