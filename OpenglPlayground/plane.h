#pragma once

extern float planeVertices[] = {
    // positions          // normal             // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
     5.0f, -0.5f,  5.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
    -5.0f, -0.5f,  5.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,

     5.0f, -0.5f,  5.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
     5.0f, -0.5f, -5.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f
};
