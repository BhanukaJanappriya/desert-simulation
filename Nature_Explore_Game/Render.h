#pragma once
#include "Global.h"

// Helper / Math Functions
void calculateNormal(int x, int z);
float truncatedCone(float botRadius, float height);
void drawSquare(int size, materialStruct* material);
void materials(materialStruct* m);
void lightingTree(lightingStruct* li);

// Object Rendering Functions
void renderSky();
void renderFlatGround();
void renderDeadTrees();
void renderTerrain();
void renderChairAndTable();
void renderWindmills();
void drawColumn(float baseR, float topR, float h);
void renderMuseum();
void renderFan();
void renderHouse();
void renderVerticalExtendedHouse();
void renderComplexHouse();
void renderLighthouse();
void renderBarrels();
void renderLake();
void renderSmallLakes();
void renderBridge();
void renderWoods();
void renderForest();
void renderCrates();
void renderSandParticles();
void renderDustStorms();
void renderSun();
void renderCampfireFlame(float x, float y, float z);
void renderPyramidTorches();
void renderPyramids();
void renderBalloons();

// Recursive Tree Function
void drawTree(int level, float botRadius, float height, float a, int rX, int rY, float leafSize);