#pragma once

#define _USE_MATH_DEFINES 
#include <SOIL2.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector> // Good practice for dynamic arrays, though we'll stick to your arrays
#include <GL/glut.h>
#include <GL/glu.h>

// --- Struct Definitions ---
typedef struct materialStruct {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess;
} materialStruct;

typedef struct lightingStruct {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
} lightingStruct;

// --- Constants ---
// Window
extern int windowWidth;
extern int windowHeight;

// Camera
extern const float LOOK_DISTANCE;

// Terrain
const int TERRAIN_SIZE = 200; // Must be const in header for array sizing
extern float heightMap[TERRAIN_SIZE][TERRAIN_SIZE];

// Sun/Light
extern const float SUN_ORBIT_RADIUS;
extern const float SUN_GLOW_RADIUS;
extern const float FLAME_SIZE;
extern const float FLICKER_BASE;
extern const float FLICKER_MAGNITUDE;
extern const float CAMPFIRE_X;
extern const float CAMPFIRE_Z;
extern const float CAMPFIRE_Y;
extern const float PYRAMID_TORCH_OFFSET;

// Tree
extern const float DtoR;
const int NUM_DEAD_TREES = 20; // Const for array sizing
const int MOUNTAIN_SIZE = 400;
extern const float MOUNTAIN_X;
extern const float MOUNTAIN_Z;
extern const float MOUNTAIN_HEIGHT_SCALE;
const int NUM_TREES = 75; 
extern const float FOREST_RANGE;

// Lighthouse
extern const float WINDOW_HEIGHT;
extern const float WINDOW_WIDTH;
extern const float WINDOW_DEPTH;
extern const float FAN_SPEED;
extern const float LIGHTHOUSE_X;
extern const float LIGHTHOUSE_Z;
extern const float LIGHTHOUSE_BASE_H;
extern const float LIGHTHOUSE_TOWER_H;
extern const float LIGHTHOUSE_BASE_SIZE;
extern const float LIGHTHOUSE_TOWER_R_BOT;
extern const float LIGHTHOUSE_TOWER_R_TOP;
extern const float LANTERN_RADIUS;
extern const float LIGHT_HEIGHT_OFFSET;

// Weather
extern float fogDensity;
extern bool fogIncreasing;
const int NUM_SAND_PARTICLES = 1000;
extern const float PARTICLE_SPEED;
extern const float PARTICLE_SIZE;
const int NUM_DUST_CLOUDS = 20;
extern const float DUST_CLOUD_SIZE;
extern const float DUST_CLOUD_H;
extern const float PARTICLE_RADIUS;

// Balloons
extern const float BALLOON_R;
extern const float BALLOON_HEIGHT;
extern const float BASKET_SIZE;
const int NUM_STRIPES = 8;

// Crate/Barrel
const int NUM_RANDOM_CRATES = 15;
const int NUM_ROLLING_BARRELS = 2;
extern float barrelRollSpeed;

// Museum
extern const float MUSEUM_X;
extern const float MUSEUM_Z;
extern const float MUSEUM_SCALE;
extern const float BASE_STEP_HEIGHT;
extern const float BASE_STEP_OVERHANG;
extern const float POLE_HEIGHT;
extern const float MUSEUM_WIDTH;
extern const float MUSEUM_DEPTH;
extern const float MUSEUM_TOWER_H;
extern const float POLE_R;
extern const float MUSEUM_ROOF_R;
extern const float SPOT_DISTANCE;
extern const float SPOT_INTENSITY;

// Windmill
extern const float WMILL_H;
extern const float WMILL_R_BOT;
extern const float WMILL_R_TOP;
extern const float WMILL_BLADE_L;
extern const float WMILL_BLADE_W;
extern const float WMILL_BASE_H;
extern const float BLADE_PITCH_ANGLE;
extern const float WMILL1_X;
extern const float WMILL1_Z;
extern const float WMILL2_X;
extern const float WMILL2_Z;
extern const float BLADE_SPEED;

// Water
const int LAKE_SIZE = 350;
extern const float LAKE_X;
extern const float LAKE_Z;
const int NUM_SMALL_LAKES = 10;
const int MIN_LAKE_SIZE = 50;
const int MAX_LAKE_SIZE = 100;
extern const float BRIDGE_WIDTH;
extern const float BRIDGE_LENGTH;
extern const float BRIDGE_Y;

// --- Global Variables (Extern) ---
extern float _angle;
extern float camX, camY, camZ;
extern float lookX, lookY, lookZ;
extern float yaw, pitch;

extern float sunOrbitalAngle;
extern GLfloat sunPosition[3];
extern GLfloat sun_emissive[];
extern GLfloat sun_zero_material[];

extern float flickerTime;
extern GLfloat fire_emissive[];
extern GLfloat fire_zero_material[];

extern GLfloat root_height, sizeRatio;
extern int angle, slices, treeLevel;
extern bool hasLeaf;
extern GLfloat sunRise;
extern GLfloat lightPosTree[];

extern float deadTreePositions[NUM_DEAD_TREES][3];

extern materialStruct Materials;
extern materialStruct LeafMaterial;
extern materialStruct SunMaterial;
extern materialStruct MoonMaterial;
extern lightingStruct LightingTree;
extern lightingStruct SunLighting;

extern bool houseLightOn;
extern float fanAngle;

extern float sandParticles[NUM_SAND_PARTICLES][3];
extern float dustClouds[NUM_DUST_CLOUDS][3];

extern float balloon1X, balloon1Y, balloon1Z, balloon1Phase;
extern float balloon2X, balloon2Y, balloon2Z, balloon2Phase;
extern GLfloat balloonColors[NUM_STRIPES][3];

extern GLuint sandTexture, houseWallTexture, skyTexture, woodTexture, barrelTexture;
extern GLuint shedTexture, skyboxTexture, roofTileTexture, chimneyTexture, windowsTexture;
extern GLuint basementTexture, treeTrunkTexture, treeLeafTexture, pyramidTexture, waterTexture;
extern GLuint lighthouseBaseTexture, lighthouseTopTexture, boxTexture, bridgeTexture;
extern GLuint doorTexture, balloonTexture;

extern float spotAngle;
extern float cratePositions[NUM_RANDOM_CRATES][3];
extern float barrelRollAngles[NUM_ROLLING_BARRELS];

extern GLfloat glass_ambient[];
extern GLfloat glass_diffuse[];
extern GLfloat glass_specular[];
extern GLfloat glass_shininess;

extern GLfloat bronze_ambient[];
extern GLfloat bronze_diffuse[];
extern GLfloat bronze_specular[];
extern GLfloat bronze_shininess;

extern float bladeAngle;
extern GLfloat metallic_ambient[];
extern GLfloat metallic_diffuse[];
extern GLfloat metallic_specular[];
extern GLfloat metallic_shininess;

extern float waterLevel;
extern float waveTimer;
extern GLfloat lightPos[];

// --- Door Control ---
extern bool isDoorOpen;
extern float currentDoorAngle;