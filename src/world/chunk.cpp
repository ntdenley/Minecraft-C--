#include <world/chunk.h>

// Macro for converting 3D coordinates to a 1D index
#define pos_to_index(x, y, z) (x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE)

// Cube vertices
float CUBE_VERTS[] = {
    // Position     // Texture Coords
    // x,  y,  z,   u, v
        0, 0, 0,     0, 0, // north face (-z)
        1, 0, 0,     1, 0,
        1, 1, 0,     1, 1,
        0, 1, 0,     0, 1,

        0, 1, 1,     1, 1, // south face (+z)
        1, 1, 1,     0, 1,
        1, 0, 1,     0, 0,
        0, 0, 1,     1, 0, 

        0, 1, 0,     1, 1, // west face (-x)
        0, 1, 1,     0, 1,
        0, 0, 1,     0, 0,
        0, 0, 0,     1, 0, 

        1, 0, 0,     0, 0, // east face (+x)
        1, 0, 1,     1, 0,
        1, 1, 1,     1, 1,
        1, 1, 0,     0, 1,

        0, 0, 0,     0, 0, // bottom face (-y)
        0, 0, 1,     1, 0,
        1, 0, 1,     1, 1,
        1, 0, 0,     0, 1,

        
        1, 1, 0,     0, 1, // top face (+y)
        1, 1, 1,     1, 1,
        0, 1, 1,     1, 0,
        0, 1, 0,     0, 0,
};
// Cube indices
unsigned int CUBE_INDICES[] = { 0,  1,  2,  2,  3,  0 };

Chunk::Chunk(glm::vec3 offset, Shader shaderProg) : offset(offset), shader(shaderProg) {
    // Initialize the chunk
    worldPos = offset * static_cast<float>(CHUNK_SIZE);

    // Generate the block data
    for(int x = 0; x < CHUNK_SIZE; x++)
    for(int y = 0; y < CHUNK_SIZE; y++)
    for(int z = 0; z < CHUNK_SIZE; z++)
        blockData[pos_to_index(x, y, z)] = BlockType::DIRT;

    // Generate the VAO, VBO, and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind the VAO to the GL State Machine
    glBindVertexArray(VAO);
}

Chunk::~Chunk() {
    // Delete the VAO, VBO, and EBO
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

Chunk::BlockType Chunk::GetBlockData(int x, int y, int z){
    if (InBounds(x, y, z))
        return blockData[pos_to_index(x, y, z)];
    return BlockType::AIR;
}

bool Chunk::InBounds(int x, int y, int z) {
    return x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE;
}

void Chunk::Generate() {
    // Generate the chunk
    for(int x = 0; x < CHUNK_SIZE; x++) {
    for(int y = 0; y < CHUNK_SIZE; y++) {
    for(int z = 0; z < CHUNK_SIZE; z++) {
        // Get the position of the block (relative to the chunk)
        glm::vec3 pos = glm::vec3(x, y, z);
        
        // Check if the block is solid, and if so, add the faces
        if(GetBlockData(x, y, z) != BlockType::AIR){
            // For each face, check adjacent blocks to see if face should be added
            if(!GetBlockData(x, y, z-1))
                AddFace(pos, Direction::NORTH);
            if(!GetBlockData(x, y, z+1))
                AddFace(pos, Direction::SOUTH);
            if(!GetBlockData(x-1, y, z))
                AddFace(pos, Direction::WEST);
            if(!GetBlockData(x+1, y, z))
                AddFace(pos, Direction::EAST);
            if(!GetBlockData(x, y-1, z))
                AddFace(pos, Direction::BOTTOM);
            if(!GetBlockData(x, y+1, z))
                AddFace(pos, Direction::TOP);
        }
    }}}

    // Bind the vertex array object
    glBindVertexArray(VAO);

    // Bind the vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Bind the element buffer object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Tell OpenGL how to interpret the vertex data
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture Coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Chunk::Render() {
    // Bind the vertex array object
    glBindVertexArray(VAO);

    // Shift the chunk to the correct position
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, worldPos);
    shader.setMat4("model", model);

    // Draw the chunk
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void Chunk::AddFace(glm::ivec3 pos, Direction direction) {

    // Vertices
    int vert_offset = direction * 20;
    for(int i = 0; i < 4; i++) {
        float *ptr = &CUBE_VERTS[vert_offset + i * 5];

        // Position
        vertices.push_back(*ptr++ + pos.x);
        vertices.push_back(*ptr++ + pos.y);
        vertices.push_back(*ptr++ + pos.z);

        // Texture Coords
        vertices.push_back(*ptr++);
        vertices.push_back(*ptr);
    }

    // Indices
    for(int i = 0; i < 6; i++) {
        indices.push_back(CUBE_INDICES[i] + vertexCount);
    }

    // 4 new vertices and 6 new indices
    indexCount += 6;
    vertexCount += 4;
}