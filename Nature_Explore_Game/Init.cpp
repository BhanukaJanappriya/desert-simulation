#include "Init.h"

// --- Helper for Randomization ---
float rand_FloatRange(float a, float b) {
    return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}

// --- Texture Loading (SOIL2) ---
GLuint loadTexture(const char* path) {
    GLuint texID = SOIL_load_OGL_texture(
        path,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT
    );

    if (texID == 0) {
        std::cerr << "SOIL loading error: " << SOIL_last_result() << std::endl;
        std::cerr << "Could not load texture: " << path << std::endl;
    }
    else {
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    return texID;
}

void loadTextures() {
    sandTexture = loadTexture("D:/My Projects/OpenGL/CSC3281/Nature_Explore_Game/Assets/Textures/sand.jpg");
    barrelTexture = loadTexture("D:/My Projects/OpenGL/CSC3281/Nature_Explore_Game/Assets/Textures/barrel.jpg");
    woodTexture = loadTexture("D:/My Projects/OpenGL/CSC3281/Nature_Explore_Game/Assets/Textures/house_wood.jpg");
    shedTexture = loadTexture("D:/My Projects/OpenGL/CSC3281/Nature_Explore_Game/Assets/Textures/brick_d.jpg");
    skyboxTexture = loadTexture("D:/My Projects/OpenGL/CSC3281/Nature_Explore_Game/Assets/Textures/sky.png");
    houseWallTexture = loadTexture("D:/My Projects/OpenGL/CSC3281/Nature_Explore_Game/Assets/Textures/house_brick.jpg");
    roofTileTexture = loadTexture("D:/My Projects/OpenGL/CSC3281/Nature_Explore_Game/Assets/Textures/house_wood.jpg");
    chimneyTexture = loadTexture("D:/My Projects/OpenGL/CSC3281/Nature_Explore_Game/Assets/Textures/brick.png");
    windowsTexture = loadTexture("D:/My Projects/OpenGL/CSC3281/Nature_Explore_Game/Assets/Textures/house_windows.jpg");
    basementTexture = loadTexture("D:/My Projects/OpenGL/CSC3281/Nature_Explore_Game/Assets/Textures/rock.png");
    treeTrunkTexture = loadTexture("D:/My Projects/OpenGL/CSC3281/Nature_Explore_Game/Assets/Textures/tree_trunk.jpg");
    treeLeafTexture = loadTexture("D:/My Projects/OpenGL/CSC3281/Nature_Explore_Game/Assets/Textures/leaf.jpg");
    pyramidTexture = loadTexture("D:/My Projects/OpenGL/CSC3281/Nature_Explore_Game/Assets/Textures/pyramid.jpg");
    waterTexture = loadTexture("D:/My Projects/OpenGL/CSC3281/Nature_Explore_Game/Assets/Textures/waveHeightmap.png");
    lighthouseBaseTexture = loadTexture("D:/My Projects/OpenGL/CSC3281/Nature_Explore_Game/Assets/Textures/lighthouse.png");
    lighthouseTopTexture = loadTexture("D:/My Projects/OpenGL/CSC3281/Nature_Explore_Game/Assets/Textures/brick.png");
    boxTexture = loadTexture("D:/My Projects/OpenGL/CSC3281/Nature_Explore_Game/Assets/Textures/wood.jpg");
    bridgeTexture = loadTexture("D:/My Projects/OpenGL/CSC3281/Nature_Explore_Game/Assets/Textures/steel.jpg");
    doorTexture = loadTexture("D:/My Projects/OpenGL/CSC3281/Nature_Explore_Game/Assets/Textures/door.jpg");
    balloonTexture = loadTexture("D:/My Projects/OpenGL/CSC3281/Nature_Explore_Game/Assets/Textures/balloon.jpg");
}

// --- OpenGL Core Setup ---
void initOpenGL() {
    glClearColor(0.6f, 0.8f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    GLfloat mat_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 16.0f);
}

// --- Lighting Setup ---
void initLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    // Light 0 & 1 always on
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    // GL_LIGHT0 (Sun) - Properties
    GLfloat ambient[] = { 0.4f, 0.4f, 0.35f, 1.0f };
    GLfloat diffuse[] = { 1.0f, 0.95f, 0.8f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

    // GL_LIGHT1 (Forest/Moon)
    GLfloat ambient1[] = { 0.05f, 0.05f, 0.0f, 1.0f };
    GLfloat diffuse1[] = { 0.4f, 0.4f, 0.2f, 1.0f };
    GLfloat defaultLight1Pos[] = { 0.0f, 80.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, defaultLight1Pos);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);

    // GL_LIGHT2 (Lighthouse Spot)
    glEnable(GL_LIGHT2);
    GLfloat spotAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat spotDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT2, GL_AMBIENT, spotAmbient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, spotDiffuse);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 15.0f);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 10.0f);

    // GL_LIGHT3 (House Interior)
    glEnable(GL_LIGHT3);
    GLfloat interiorDiffuse[] = { 0.8f, 0.8f, 0.7f, 1.0f };
    GLfloat interiorAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat interiorPosition[] = { 300.0f, 30.0f, 100.0f, 1.0f };
    glLightfv(GL_LIGHT3, GL_POSITION, interiorPosition);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, interiorDiffuse);
    glLightfv(GL_LIGHT3, GL_AMBIENT, interiorAmbient);
    glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.01f);
    glDisable(GL_LIGHT3); // Start disabled

    // GL_LIGHT4 (Pyramid Torches)
    glEnable(GL_LIGHT4);
    GLfloat torchAmbient[] = { 0.5f, 0.3f, 0.0f, 1.0f };
    GLfloat torchDiffuse[] = { 1.0f, 0.6f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT4, GL_AMBIENT, torchAmbient);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, torchDiffuse);
    glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, 0.01f);
    glDisable(GL_LIGHT4);

    // Museum Spotlights (5-8)
    GLfloat redDiffuse[] = { 0.8f, 0.0f, 0.0f, 1.0f };
    GLfloat redAmbient[] = { 0.1f, 0.0f, 0.0f, 1.0f };
    for (int i = 5; i <= 8; ++i) {
        glEnable(GL_LIGHT3 + i);
        glLightfv(GL_LIGHT3 + i, GL_DIFFUSE, redDiffuse);
        glLightfv(GL_LIGHT3 + i, GL_AMBIENT, redAmbient);
        glLightf(GL_LIGHT3 + i, GL_SPOT_CUTOFF, 15.0f);
        glLightf(GL_LIGHT3 + i, GL_SPOT_EXPONENT, 15.0f);
    }
}

// --- Data Generation Functions ---

void generateHeightMap() {
    srand(1234);
    for (int x = 0; x < TERRAIN_SIZE; x++) {
        for (int z = 0; z < TERRAIN_SIZE; z++) {
            float freq = 0.08f;
            float nx = x * freq;
            float nz = z * freq;
            heightMap[x][z] = sin(nx) * cos(nz) * 4.0f + ((rand() % 100) / 100.0f) * 0.8f;
        }
    }
}

void generateWildernessPositions() {
    srand(static_cast<unsigned int>(time(0)) + 4);
    float sceneArea = 1000.0f;
    for (int i = 0; i < NUM_DEAD_TREES; i++) {
        deadTreePositions[i][0] = ((float)rand() / RAND_MAX * (2.0f * sceneArea)) - sceneArea;
        deadTreePositions[i][2] = ((float)rand() / RAND_MAX * (2.0f * sceneArea)) - sceneArea;
        deadTreePositions[i][1] = 0.0f;
    }
}

void initializeRollingBarrels() {
    srand(static_cast<unsigned int>(time(0)) + 3);
    for (int i = 0; i < NUM_ROLLING_BARRELS; i++) {
        barrelRollAngles[i] = rand_FloatRange(0.0f, 360.0f);
    }
}

void generateParticlePositions() {
    srand(static_cast<unsigned int>(time(0)) + 2);

    // Sand particles
    float particleArea = 1000.0f;
    for (int i = 0; i < NUM_SAND_PARTICLES; i++) {
        sandParticles[i][0] = ((float)rand() / RAND_MAX * (2.0f * particleArea)) - particleArea;
        sandParticles[i][1] = ((float)rand() / RAND_MAX * 5.0f) + 0.1f;
        sandParticles[i][2] = ((float)rand() / RAND_MAX * (2.0f * particleArea)) - particleArea;
    }

    // Dust Clouds
    float cloudArea = 1500.0f;
    for (int i = 0; i < NUM_DUST_CLOUDS; i++) {
        dustClouds[i][0] = ((float)rand() / RAND_MAX * (2.0f * cloudArea)) - cloudArea;
        dustClouds[i][1] = ((float)rand() / RAND_MAX * 20.0f) + 5.0f;
        dustClouds[i][2] = ((float)rand() / RAND_MAX * (2.0f * cloudArea)) - cloudArea;
    }
}

void generateCratePositions() {
    srand(static_cast<unsigned int>(time(0)) + 1);
    float sceneSize = 1000.0f;
    for (int i = 0; i < NUM_RANDOM_CRATES; i++) {
        cratePositions[i][0] = ((float)rand() / RAND_MAX * (2.0f * sceneSize)) - sceneSize;
        cratePositions[i][2] = ((float)rand() / RAND_MAX * (2.0f * sceneSize)) - sceneSize;
        cratePositions[i][1] = 0.0f;
    }
}

void setupFog() {
    glEnable(GL_FOG);
    GLfloat fogColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_EXP);
    glFogf(GL_FOG_DENSITY, fogDensity);
}