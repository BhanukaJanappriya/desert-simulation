#pragma once
#include "Global.h"

// Helper Functions
float rand_FloatRange(float a, float b);
GLuint loadTexture(const char* path);

// Initialization Routines
void initOpenGL();
void initLighting();
void loadTextures();
void generateHeightMap();
void generateWildernessPositions();
void initializeRollingBarrels();
void generateParticlePositions();
void generateCratePositions();
void setupFog();