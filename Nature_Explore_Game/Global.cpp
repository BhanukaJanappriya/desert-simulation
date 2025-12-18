#include "Global.h"

// --- Window ---
int windowWidth = 1280;
int windowHeight = 720;

// --- Camera ---
float camX = 100, camY = 15, camZ = 120;
float lookX = 100, lookY = 15, lookZ = 119;
float yaw = 90.0f;
float pitch = 0.0f;
float _angle = -70.0f;
const float LOOK_DISTANCE = 1.0f;

// --- Terrain ---
float heightMap[TERRAIN_SIZE][TERRAIN_SIZE];

// --- Sun & Light ---
float sunOrbitalAngle = 0.0f;
const float SUN_ORBIT_RADIUS = 1000.0f;
const float SUN_GLOW_RADIUS = 30.0f;
GLfloat sunPosition[3] = { 0.0f, 0.0f, 0.0f };
GLfloat sun_emissive[] = { 1.0f, 1.0f, 0.8f, 1.0f };
GLfloat sun_zero_material[] = { 0.0f, 0.0f, 0.0f, 1.0f };
GLfloat sunRise = 1.0f;

// --- Fire & Torches ---
const float FLAME_SIZE = 10.0f;
const float FLICKER_BASE = 0.5f;
const float FLICKER_MAGNITUDE = 0.4f;
float flickerTime = 0.0f;
const float CAMPFIRE_X = 300.0f;
const float CAMPFIRE_Z = 130.0f;
const float CAMPFIRE_Y = 1.0f;
const float PYRAMID_TORCH_OFFSET = 100.0f;
GLfloat fire_emissive[] = { 1.0f, 0.4f, 0.0f, 1.0f };
GLfloat fire_zero_material[] = { 0.0f, 0.0f, 0.0f, 1.0f };

// --- Trees ---
const float DtoR = 0.017453f;
GLfloat root_height = 20.0f, sizeRatio = 5.0f;
int angle = 25, slices = 8, treeLevel = 6;
bool hasLeaf = true;
GLfloat lightPosTree[] = { 0.0f, 0.0f, 200.0f, 0.0f };
float deadTreePositions[NUM_DEAD_TREES][3];
const float FOREST_RANGE = 350.0f;

// --- Mountains ---
const float MOUNTAIN_X = -800.0f;
const float MOUNTAIN_Z = 600.0f;
const float MOUNTAIN_HEIGHT_SCALE = 150.0f;

// --- Materials ---
materialStruct Materials = { {0.6f, 0.44f, 0.24f, 1.0f}, {0.4f, 0.2f, 0.2f, 1.0f}, {0.2f, 0.1f, 0.1f, 1.0f}, 32.0f };
materialStruct LeafMaterial = { {0.3f, 0.8f, 0.2f, 0.5f}, {0.2f, 0.5f, 0.2f, 0.5f}, {0.2f, 0.5f, 0.2f, 0.5f}, 50.0f };
materialStruct SunMaterial = { {1.0f, 1.0f, 0.2f, 0.5f}, {1.0f, 1.0f, 0.2f, 0.5f}, {0.2f, 0.5f, 0.2f, 0.5f}, 100.0f };
materialStruct MoonMaterial = { {1.0f, 1.0f, 1.0f, 0.5f}, {1.0f, 0.9f, 0.9f, 0.5f}, {0.2f, 0.5f, 0.2f, 0.5f}, 100.0f };
lightingStruct LightingTree = { {0.05f, 0.05f, 0.0f, 0.5f}, {0.4f, 0.4f, 0.2f, 1.0f}, {0.8f, 0.7f, 1.0f, 0.5f} };
lightingStruct SunLighting = { {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 0.6f}, {1.0f, 1.0f, 1.0f, 0.5f} };

// --- Lighthouse ---
const float WINDOW_HEIGHT = 10.0f;
const float WINDOW_WIDTH = 4.0f;
const float WINDOW_DEPTH = 0.5f;
bool houseLightOn = false;
float fanAngle = 0.0f;
const float FAN_SPEED = 5.0f;
const float LIGHTHOUSE_X = 25.0f;
const float LIGHTHOUSE_Z = 300.0f;
const float LIGHTHOUSE_BASE_H = 5.0f;
const float LIGHTHOUSE_TOWER_H = 120.0f;
const float LIGHTHOUSE_BASE_SIZE = 20.0f;
const float LIGHTHOUSE_TOWER_R_BOT = 8.0f;
const float LIGHTHOUSE_TOWER_R_TOP = 5.0f;
const float LANTERN_RADIUS = LIGHTHOUSE_TOWER_R_TOP * 1.2f;
float spotAngle = 0.0f;
const float LIGHT_HEIGHT_OFFSET = LIGHTHOUSE_BASE_H + LIGHTHOUSE_TOWER_H + 5.0f;

// --- Weather ---
float fogDensity = 0.005f;
bool fogIncreasing = true;
float sandParticles[NUM_SAND_PARTICLES][3];
const float PARTICLE_SPEED = 2.0f;
const float PARTICLE_SIZE = 0.5f;
float dustClouds[NUM_DUST_CLOUDS][3];
const float DUST_CLOUD_SIZE = 50.0f;
const float DUST_CLOUD_H = 15.0f;
const float PARTICLE_RADIUS = 0.2f;

// --- Balloons ---
const float BALLOON_R = 30.0f;
const float BALLOON_HEIGHT = 45.0f;
const float BASKET_SIZE = 5.0f;
float balloon1X = -400.0f, balloon1Y = 300.0f, balloon1Z = -100.0f, balloon1Phase = 0.0f;
float balloon2X = 500.0f, balloon2Y = 450.0f, balloon2Z = -400.0f, balloon2Phase = 1.5f;
GLfloat balloonColors[NUM_STRIPES][3] = {
    {1.0f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.5f, 1.0f}, {1.0f, 0.0f, 1.0f},
    {1.0f, 0.8f, 0.0f}, {0.8f, 0.0f, 0.8f}, {0.0f, 0.8f, 0.8f}, {1.0f, 0.0f, 0.0f}
};

// --- Textures ---
GLuint sandTexture, houseWallTexture, skyTexture, woodTexture, barrelTexture;
GLuint shedTexture, skyboxTexture, roofTileTexture, chimneyTexture, windowsTexture;
GLuint basementTexture, treeTrunkTexture, treeLeafTexture, pyramidTexture, waterTexture;
GLuint lighthouseBaseTexture, lighthouseTopTexture, boxTexture, bridgeTexture;
GLuint doorTexture, balloonTexture;

// --- Crates & Barrels ---
float cratePositions[NUM_RANDOM_CRATES][3];
float barrelRollAngles[NUM_ROLLING_BARRELS];
float barrelRollSpeed = 5.0f;

// --- Museum ---
const float MUSEUM_X = 700.0f;
const float MUSEUM_Z = 50.0f;
const float MUSEUM_SCALE = 3.0f;
const float BASE_STEP_HEIGHT = 2.0f;
const float BASE_STEP_OVERHANG = 5.0f;
const float POLE_HEIGHT = 50.0f;
const float MUSEUM_WIDTH = 40.0f * MUSEUM_SCALE;
const float MUSEUM_DEPTH = 20.0f * MUSEUM_SCALE;
const float MUSEUM_TOWER_H = 15.0f * MUSEUM_SCALE;
const float POLE_R = 3.0f;
const float MUSEUM_ROOF_R = 15.0f * MUSEUM_SCALE;
GLfloat bronze_ambient[] = { 0.21f, 0.13f, 0.05f, 1.0f };
GLfloat bronze_diffuse[] = { 0.71f, 0.43f, 0.18f, 1.0f };
GLfloat bronze_specular[] = { 0.39f, 0.27f, 0.17f, 1.0f };
GLfloat bronze_shininess = 25.0f;
const float SPOT_DISTANCE = 100.0f;
const float SPOT_INTENSITY = 0.8f;

// --- Windmill ---
const float WMILL_H = 80.0f;
const float WMILL_R_BOT = 15.0f;
const float WMILL_R_TOP = 8.0f;
const float WMILL_BLADE_L = 60.0f;
const float WMILL_BLADE_W = 10.0f;
const float WMILL_BASE_H = 8.0f;
const float BLADE_PITCH_ANGLE = 10.0f;
const float WMILL1_X = -100.0f;
const float WMILL1_Z = -350.0f;
const float WMILL2_X = -150.0f;
const float WMILL2_Z = -250.0f;
float bladeAngle = 0.0f;
const float BLADE_SPEED = 1.5f;
GLfloat metallic_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat metallic_diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat metallic_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat metallic_shininess = 64.0f;

// --- Water ---
const float LAKE_X = 100.0f;
const float LAKE_Z = 350.0f;
float waterLevel = 1.0f;
float waveTimer = 0.0f;
const float BRIDGE_WIDTH = 10.0f;
const float BRIDGE_LENGTH = 400.0f;
const float BRIDGE_Y = 3.0f;
GLfloat lightPos[] = { 100.0f, 80.0f, 50.0f, 1.0f };

// --- Glass ---
GLfloat glass_ambient[] = { 0.0f, 0.0f, 0.0f, 0.2f };
GLfloat glass_diffuse[] = { 0.5f, 0.5f, 0.5f, 0.2f };
GLfloat glass_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat glass_shininess = 100.0f;

// --- Door Control ---
bool isDoorOpen = false;       // Starts closed
float currentDoorAngle = 0.0f; // Starts at 0 degrees