#include "Render.h"

// --- Helper: Normal Calculation ---
void calculateNormal(int x, int z) {
    if (x <= 0 || x >= TERRAIN_SIZE - 1 || z <= 0 || z >= TERRAIN_SIZE - 1) {
        glNormal3f(0.0f, 1.0f, 0.0f);
        return;
    }
    float dx_y = heightMap[x + 1][z] - heightMap[x - 1][z];
    float dz_y = heightMap[x][z + 1] - heightMap[x][z - 1];
    float nx = -dx_y;
    float ny = 4.0f;
    float nz = -dz_y;
    float length = sqrt(nx * nx + ny * ny + nz * nz);
    if (length > 0.0f) glNormal3f(nx / length, ny / length, nz / length);
    else glNormal3f(0.0f, 1.0f, 0.0f);
}

void renderFlatGround() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, sandTexture);

    float size = 1350.0f;
    float tiling = 300.0f; // Increased tiling for visual density

    glColor3f(0.85f, 0.75f, 0.5f);
    glNormal3f(0.0f, 1.0f, 0.0f);

    // Lake hole boundary
    float lx_min = LAKE_X - (float)LAKE_SIZE / 2.0f;
    float lx_max = LAKE_X + (float)LAKE_SIZE / 2.0f;
    float lz_min = LAKE_Z - (float)LAKE_SIZE / 2.0f;
    float lz_max = LAKE_Z + (float)LAKE_SIZE / 2.0f;

    glBegin(GL_QUADS);

    // 1. BACK SECTION (Behind the lake)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, 0.0f, -size);
    glTexCoord2f(tiling, 0.0f); glVertex3f(size, 0.0f, -size);
    glTexCoord2f(tiling, (lz_min - (-size)) / size * tiling); glVertex3f(size, 0.0f, lz_min);
    glTexCoord2f(0.0f, (lz_min - (-size)) / size * tiling); glVertex3f(-size, 0.0f, lz_min);

    // 2. FRONT SECTION (In front of the lake)
    glTexCoord2f(0.0f, (lz_max - (-size)) / size * tiling); glVertex3f(-size, 0.0f, lz_max);
    glTexCoord2f(tiling, (lz_max - (-size)) / size * tiling); glVertex3f(size, 0.0f, lz_max);
    glTexCoord2f(tiling, tiling); glVertex3f(size, 0.0f, size);
    glTexCoord2f(0.0f, tiling); glVertex3f(-size, 0.0f, size);

    // 3. LEFT SECTION (Left of the lake hole)
    glTexCoord2f(0.0f, (lz_min - (-size)) / size * tiling); glVertex3f(-size, 0.0f, lz_min);
    glTexCoord2f((lx_min - (-size)) / size * tiling, (lz_min - (-size)) / size * tiling); glVertex3f(lx_min, 0.0f, lz_min);
    glTexCoord2f((lx_min - (-size)) / size * tiling, (lz_max - (-size)) / size * tiling); glVertex3f(lx_min, 0.0f, lz_max);
    glTexCoord2f(0.0f, (lz_max - (-size)) / size * tiling); glVertex3f(-size, 0.0f, lz_max);

    // 4. RIGHT SECTION (Right of the lake hole)
    glTexCoord2f((lx_max - (-size)) / size * tiling, (lz_min - (-size)) / size * tiling); glVertex3f(lx_max, 0.0f, lz_min);
    glTexCoord2f(tiling, (lz_min - (-size)) / size * tiling); glVertex3f(size, 0.0f, lz_min);
    glTexCoord2f(tiling, (lz_max - (-size)) / size * tiling); glVertex3f(size, 0.0f, lz_max);
    glTexCoord2f((lx_max - (-size)) / size * tiling, (lz_max - (-size)) / size * tiling); glVertex3f(lx_max, 0.0f, lz_max);

    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void renderDeadTrees() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, treeTrunkTexture);
    glColor3f(1.0f, 1.0f, 1.0f); // White for texture

    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluQuadricTexture(quad, GL_TRUE);

    for (int i = 0; i < NUM_DEAD_TREES; i++) {
        glPushMatrix();
        float x = deadTreePositions[i][0];
        float z = deadTreePositions[i][2];
        float h = 20.0f + (float)rand() / RAND_MAX * 30.0f;
        float r = 2.0f + (float)rand() / RAND_MAX * 2.0f;

        glTranslatef(x, 0.0f, z);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

        // Draw the trunk (No branches, just a tall, thin cylinder)
        gluCylinder(quad, r, r * 0.5f, h, 16, 16);

        // Draw the top cap (dead wood color)
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.4f, 0.3f, 0.2f);
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, h);
        gluDisk(quad, 0.0f, r * 0.5f, 16, 1);
        glPopMatrix();
        glEnable(GL_TEXTURE_2D);

        glPopMatrix();
    }

    gluDeleteQuadric(quad);
    glDisable(GL_TEXTURE_2D);
}

void renderTerrain() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, sandTexture);

    glColor3f(0.85f, 0.75f, 0.5f);

    for (int x = 0; x < TERRAIN_SIZE - 1; x++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int z = 0; z < TERRAIN_SIZE; z++) {

            // Vertex 1: (x, z)
            calculateNormal(x, z);
            glTexCoord2f(x * 0.1f, z * 0.1f);
            glVertex3f(x, heightMap[x][z], z);

            // Vertex 2: (x+1, z)
            calculateNormal(x + 1, z);
            glTexCoord2f((x + 1) * 0.1f, z * 0.1f);
            glVertex3f(x + 1, heightMap[x + 1][z], z);
        }
        glEnd();
    }

    glDisable(GL_TEXTURE_2D);
}

void renderChairAndTable() {

    glDisable(GL_TEXTURE_2D);
    glPushMatrix();

    // 1. Apply Texture/Color for the table top and legs
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, woodTexture);

    // Scale the entire model down to fit the house interior (e.g., scale 0.5)
    glScalef(1.0f, 1.0f, 1.0f);

    // Apply a simple rotation for visual interest
    glRotatef(30.0f, 0.0f, 1.0f, 0.0f);

    // Rotate the model based on the global angle
    glRotatef(_angle, 0.0f, 1.0f, 0.0f);

    glBegin(GL_QUADS);

    // ==================
    // TABLE TOP
    // ==================
    // Front
    glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, -0.2f, 2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2.0f, -0.2f, 2.0f);
    glTexCoord2f(1.0f, 0.5f); glVertex3f(2.0f, 0.2f, 2.0f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(-2.0f, 0.2f, 2.0f);
    // Right
    glNormal3f(1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(2.0f, -0.2f, -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(2.0f, 0.2f, -2.0f);
    glTexCoord2f(1.0f, 0.5f); glVertex3f(2.0f, 0.2f, 2.0f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(2.0f, -0.2f, 2.0f);
    // Back
    glNormal3f(0.0f, 0.0f, -1.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, -0.2f, -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.0f, 0.2f, -2.0f);
    glTexCoord2f(1.0f, 0.5f); glVertex3f(2.0f, 0.2f, -2.0f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(2.0f, -0.2f, -2.0f); // <- Corrected from original order
    // Left
    glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.0f, -0.2f, -2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.0f, -0.2f, 2.0f);
    glTexCoord2f(1.0f, 0.5f); glVertex3f(-2.0f, 0.2f, 2.0f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(-2.0f, 0.2f, -2.0f);
    // top
    glNormal3f(0.0f, 1.0f, 0.0f); glTexCoord2f(0.0f, 0.5f); glVertex3f(2.0f, 0.2f, 2.0f);
    glTexCoord2f(1.0f, 0.5f); glVertex3f(-2.0f, 0.2f, 2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.0f, 0.2f, -2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(2.0f, 0.2f, -2.0f);
    // bottom
    glNormal3f(0.0f, -1.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(2.0f, -0.2f, 2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.0f, -0.2f, 2.0f);
    glTexCoord2f(1.0f, 0.5f); glVertex3f(-2.0f, -0.2f, -2.0f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(2.0f, -0.2f, -2.0f);

    // ==================
    // TABLE LEGS (ORIGINAL)
    // ==================

    glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(1.8f, -0.2f, 1.6f);
    glTexCoord2f(0.5f, 0.0f); glVertex3f(1.4f, -0.2f, 1.6f);
    glTexCoord2f(0.5f, 0.5f); glVertex3f(1.4f, -3.0f, 1.6f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(1.8f, -3.0f, 1.6f);
    // back
    glNormal3f(0.0f, 0.0f, -1.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(1.8f, -0.2f, 1.2f);
    glTexCoord2f(0.5f, 0.0f); glVertex3f(1.4f, -0.2f, 1.2f);
    glTexCoord2f(0.5f, 0.5f); glVertex3f(1.4f, -3.0f, 1.2f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(1.8f, -3.0f, 1.2f);
    // right
    glNormal3f(1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(1.8f, -0.2f, 1.6f);
    glTexCoord2f(0.5f, 0.0f); glVertex3f(1.8f, -0.2f, 1.2f);
    glTexCoord2f(0.5f, 0.5f); glVertex3f(1.8f, -3.0f, 1.2f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(1.8f, -3.0f, 1.6f);
    // left
    glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(1.4f, -0.2f, 1.6f);
    glTexCoord2f(0.5f, 0.0f); glVertex3f(1.4f, -0.2f, 1.2f);
    glTexCoord2f(0.5f, 0.5f); glVertex3f(1.4f, -3.0f, 1.2f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(1.4f, -3.0f, 1.6f);

    // back leg back (4 faces)
    glNormal3f(0.0f, 0.0f, -1.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(1.8f, -0.2f, -1.2f);
    glTexCoord2f(0.5f, 0.0f); glVertex3f(1.4f, -0.2f, -1.2f);
    glTexCoord2f(0.5f, 0.5f); glVertex3f(1.4f, -3.0f, -1.2f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(1.8f, -3.0f, -1.2f);
    glNormal3f(0.0f, 0.0f, -1.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(1.8f, -0.2f, -1.6f);
    glTexCoord2f(0.5f, 0.0f); glVertex3f(1.4f, -0.2f, -1.6f);
    glTexCoord2f(0.5f, 0.5f); glVertex3f(1.4f, -3.0f, -1.6f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(1.8f, -3.0f, -1.6f);
    glNormal3f(1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(1.8f, -0.2f, -1.6f);
    glTexCoord2f(0.5f, 0.0f); glVertex3f(1.8f, -0.2f, -1.2f);
    glTexCoord2f(0.5f, 0.5f); glVertex3f(1.8f, -3.0f, -1.2f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(1.8f, -3.0f, -1.6f);
    glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(1.4f, -0.2f, -1.6f);
    glTexCoord2f(0.5f, 0.0f); glVertex3f(1.4f, -0.2f, -1.2f);
    glTexCoord2f(0.5f, 0.5f); glVertex3f(1.4f, -3.0f, -1.2f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(1.4f, -3.0f, -1.6f);

    // leg left front (4 faces)
    glNormal3f(0.0f, 0.0f, 1.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.8f, -0.2f, 1.6f);
    glTexCoord2f(0.5f, 0.0f); glVertex3f(-1.4f, -0.2f, 1.6f);
    glTexCoord2f(0.5f, 0.5f); glVertex3f(-1.4f, -3.0f, 1.6f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(-1.8f, -3.0f, 1.6f);
    glNormal3f(0.0f, 0.0f, -1.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.8f, -0.2f, 1.2f);
    glTexCoord2f(0.5f, 0.0f); glVertex3f(-1.4f, -0.2f, 1.2f);
    glTexCoord2f(0.5f, 0.5f); glVertex3f(-1.4f, -3.0f, 1.2f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(-1.8f, -3.0f, 1.2f);
    glNormal3f(1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.8f, -0.2f, 1.6f);
    glTexCoord2f(0.5f, 0.0f); glVertex3f(-1.8f, -0.2f, 1.2f);
    glTexCoord2f(0.5f, 0.5f); glVertex3f(-1.8f, -3.0f, 1.2f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(-1.8f, -3.0f, 1.6f);
    glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.4f, -0.2f, 1.6f);
    glTexCoord2f(0.5f, 0.0f); glVertex3f(-1.4f, -0.2f, 1.2f);
    glTexCoord2f(0.5f, 0.5f); glVertex3f(-1.4f, -3.0f, 1.2f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(-1.4f, -3.0f, 1.6f);

    // left leg back (4 faces)
    glNormal3f(0.0f, 0.0f, -1.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.8f, -0.2f, -1.2f);
    glTexCoord2f(0.5f, 0.0f); glVertex3f(-1.4f, -0.2f, -1.2f);
    glTexCoord2f(0.5f, 0.5f); glVertex3f(-1.4f, -3.0f, -1.2f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(-1.8f, -3.0f, -1.2f);
    glNormal3f(0.0f, 0.0f, -1.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.8f, -0.2f, -1.6f);
    glTexCoord2f(0.5f, 0.0f); glVertex3f(-1.4f, -0.2f, -1.6f);
    glTexCoord2f(0.5f, 0.5f); glVertex3f(-1.4f, -3.0f, -1.6f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(-1.8f, -3.0f, -1.6f);
    glNormal3f(1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.8f, -0.2f, -1.6f);
    glTexCoord2f(0.5f, 0.0f); glVertex3f(-1.8f, -0.2f, -1.2f);
    glTexCoord2f(0.5f, 0.5f); glVertex3f(-1.8f, -3.0f, -1.2f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(-1.8f, -3.0f, -1.6f);
    glNormal3f(-1.0f, 0.0f, 0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.4f, -0.2f, -1.6f);
    glTexCoord2f(0.5f, 0.0f); glVertex3f(-1.4f, -0.2f, -1.2f);
    glTexCoord2f(0.5f, 0.5f); glVertex3f(-1.4f, -3.0f, -1.2f);
    glTexCoord2f(0.0f, 0.5f); glVertex3f(-1.4f, -3.0f, -1.6f);


    // ==================
    // CHAIR BACK 
    // ==================

    // Front
    glVertex3f(-1.8f, 0.2f, -1.8f); glVertex3f(1.8f, 0.2f, -1.8f);
    glVertex3f(1.8f, 3.5f, -1.8f); glVertex3f(-1.8f, 3.5f, -1.8f);
    // Back
    glVertex3f(-1.8f, 0.2f, -2.0f); glVertex3f(1.8f, 0.2f, -2.0f);
    glVertex3f(1.8f, 3.5f, -2.0f); glVertex3f(-1.8f, 3.5f, -2.0f);
    // Left side
    glVertex3f(-1.8f, 0.2f, -2.0f); glVertex3f(-1.8f, 3.5f, -2.0f);
    glVertex3f(-1.8f, 3.5f, -1.8f); glVertex3f(-1.8f, 0.2f, -1.8f);
    // Right side
    glVertex3f(1.8f, 0.2f, -2.0f); glVertex3f(1.8f, 3.5f, -2.0f);
    glVertex3f(1.8f, 3.5f, -1.8f); glVertex3f(1.8f, 0.2f, -1.8f);
    // Top
    glVertex3f(-1.8f, 3.5f, -2.0f); glVertex3f(-1.8f, 3.5f, -1.8f);
    glVertex3f(1.8f, 3.5f, -1.8f); glVertex3f(1.8f, 3.5f, -2.0f);
    glDisable(GL_TEXTURE_2D);
    glEnd();
    glEnable(GL_TEXTURE_2D); // Re-enable texture

    glPopMatrix(); // End Main Table/Chair Transform
}

void renderWindmill(float posX, float posZ) {
    float baseY = 0.0f;
    float currentY = baseY;

    glPushMatrix();
    glTranslatef(posX, baseY, posZ);

    // ===================================================
    // 1. HEXAGONAL BASE 
    // ===================================================
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, houseWallTexture);
    glColor3f(0.5f, 0.5f, 0.5f); // Slightly different color for the base

    glPushMatrix();
    glTranslatef(0.0f, currentY + WMILL_BASE_H / 2.0f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Align for the base cylinder

    GLUquadric* quadBase = gluNewQuadric();
    gluQuadricNormals(quadBase, GLU_SMOOTH);
    gluQuadricTexture(quadBase, GL_TRUE);

    // Hexagonal cylinder (6 sides)
    gluCylinder(quadBase, WMILL_R_BOT * 1.5f, WMILL_R_BOT * 1.5f, WMILL_BASE_H, 6, 1);

    // Top Disk
    glTranslatef(0.0f, 0.0f, WMILL_BASE_H);
    gluDisk(quadBase, 0.0f, WMILL_R_BOT * 1.5f, 6, 1);

    gluDeleteQuadric(quadBase);
    glPopMatrix();

    currentY += WMILL_BASE_H;

    // ===================================================
    // 2. TOWER (Tapered Cylinder 
    // ===================================================
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, houseWallTexture);
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    glTranslatef(0.0f, currentY, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluQuadricTexture(quad, GL_TRUE);

    gluCylinder(quad, WMILL_R_BOT, WMILL_R_TOP, WMILL_H, 32, 32);

    gluDeleteQuadric(quad);
    glPopMatrix();

    currentY += WMILL_H;
    glDisable(GL_TEXTURE_2D);

    // ===================================================
    // 3. ROTOR HUB & AXLE (Metallic Material) 
    // ===================================================
    glPushAttrib(GL_LIGHTING_BIT);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, metallic_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, metallic_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, metallic_specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, metallic_shininess);
    glColor3f(0.8f, 0.8f, 0.8f);

    glPushMatrix();
    glTranslatef(0.0f, currentY, 1.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

    // Hub (Cone)
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    glutSolidCone(5.0f, 10.0f, 16, 1);
    glPopMatrix();

    // Axle (Cylinder)
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 5.0f);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    GLUquadric* quadAxle = gluNewQuadric();
    gluCylinder(quadAxle, 1.0f, 1.0f, 10.0f, 16, 1);
    gluDeleteQuadric(quadAxle);
    glPopMatrix();

    // ===================================================
    // 4. ROTATING BLADES
    // ===================================================
    glDisable(GL_TEXTURE_2D);
    glTranslatef(0.0f, 0.0f, 15.0f); // Move to front of hub
    // MAIN ROTOR ROTATION (spin)
    glRotatef(bladeAngle, 0.0f, 0.0f, 1.0f);

    for (int i = 0; i < 4; ++i) {

        glPushMatrix();

        // Spread blades evenly
        glRotatef((float)i * 90.0f, 0.0f, 0.0f, 1.0f); // ✅ Z-axis

        // OPTIONAL: slight aerodynamic pitch
        glRotatef(BLADE_PITCH_ANGLE, 1.0f, 0.0f, 0.0f); // small tilt ONLY

        // =========================
        // BLADE GEOMETRY 
        // =========================
        glColor3f(0.6f, 0.4f, 0.2f);
        float plankThickness = 1.0f;

        float frameW = 2.0f;
        float frameH = WMILL_BLADE_W + 1.0f;

        // Top plank
        glPushMatrix();
        glTranslatef(WMILL_BLADE_L / 2.0f, frameH / 2.0f, 0.0f);
        glScalef(WMILL_BLADE_L, frameW, plankThickness);
        glutSolidCube(1.0f);
        glPopMatrix();

        // Bottom plank
        glPushMatrix();
        glTranslatef(WMILL_BLADE_L / 2.0f, -frameH / 2.0f, 0.0f);
        glScalef(WMILL_BLADE_L, frameW, plankThickness);
        glutSolidCube(1.0f);
        glPopMatrix();

        // End plank
        glPushMatrix();
        glTranslatef(WMILL_BLADE_L, 0.0f, 0.0f);
        glScalef(frameW, frameH, plankThickness);
        glutSolidCube(1.0f);
        glPopMatrix();

        // Base plank (near hub)
        glPushMatrix();
        glTranslatef(WMILL_R_TOP * 1.5f, 0.0f, 0.0f);
        glScalef(frameW, frameH, plankThickness);
        glutSolidCube(1.0f);
        glPopMatrix();

        // Inner grid
        int numVerticalPlanks = 5;
        float innerPlankL = WMILL_BLADE_L - WMILL_R_TOP * 1.5f - frameW / 2.0f;
        float spacing = innerPlankL / numVerticalPlanks;

        for (int j = 1; j <= numVerticalPlanks; ++j) {
            glPushMatrix();
            glTranslatef(WMILL_R_TOP * 1.5f + j * spacing, 0.0f, 0.0f);
            glScalef(frameW, frameH, plankThickness);
            glutSolidCube(1.0f);
            glPopMatrix();
        }

        glPopMatrix(); // blade
    }


    glPopMatrix(); // End Rotor Hub Transform
    glPopAttrib();

    // ===================================================
    // 5. WINDOWS (Reusing House Window Code) 
    // ===================================================
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, woodTexture);
    glColor3f(1.0f, 1.0f, 1.0f);

    float windowY = 0.0f;
    for (int j = 0; j < 3; ++j) {
        windowY = currentY - (WMILL_H / 4.0f) * (float)j - 10.0f;
        for (int i = 0; i < 4; ++i) {
            float angle = (float)i * 30.0f;

            glPushMatrix();
            glTranslatef(0.0f, windowY, 0.0f);
            glRotatef(angle, 0.0f, 1.0f, 0.0f);

            // Radius calculation adjusted for the offset base height
            float towerBottomY = currentY - WMILL_H;
            float heightRatio = (windowY - towerBottomY) / WMILL_H;
            float radiusAtY = WMILL_R_BOT + (WMILL_R_TOP - WMILL_R_BOT) * heightRatio;

            glTranslatef(radiusAtY, 0.0f, 0.0f);

            glBegin(GL_QUADS);
            glNormal3f(1.0f, 0.0f, 0.0f);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, -WINDOW_HEIGHT / 4.0f, -WINDOW_WIDTH / 4.0f);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, -WINDOW_HEIGHT / 4.0f, WINDOW_WIDTH / 4.0f);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, WINDOW_HEIGHT / 4.0f, WINDOW_WIDTH / 4.0f);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, WINDOW_HEIGHT / 4.0f, -WINDOW_WIDTH / 4.0f);
            glEnd();

            glPopMatrix();
        }
    }
    glDisable(GL_TEXTURE_2D);

    glPopMatrix(); // End Main Windmill Transform
}

void renderWindmills() {
    renderWindmill(WMILL1_X, WMILL1_Z);
    renderWindmill(WMILL2_X, WMILL2_Z);
}

void drawColumn(float baseR, float topR, float h) {
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluCylinder(quad, baseR, topR, h, 16, 16);

    // Top and Bottom Plates (Architrave/Base)
    glColor3f(0.8f, 0.8f, 0.8f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, h);
    gluDisk(quad, 0.0f, topR * 1.2f, 16, 1);
    glPopMatrix();

    glPushMatrix();
    glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
    gluDisk(quad, 0.0f, baseR * 1.2f, 16, 1);
    glPopMatrix();

    gluDeleteQuadric(quad);
}

void renderMuseum() {
    float baseY = 0.0f;
    float colR = POLE_R;
    float towerH = MUSEUM_TOWER_H;
    float roofR = MUSEUM_ROOF_R;

    // Set material for the entire structure (Bronze)
    glPushAttrib(GL_LIGHTING_BIT);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, bronze_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, bronze_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, bronze_specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, bronze_shininess);
    glColor3f(1.0f, 0.85f, 0.5f); // Base color for the bronze look

    glPushMatrix();
    glTranslatef(MUSEUM_X, baseY, MUSEUM_Z-100.0f);
    glScalef(2.0f, 2.0f, 2.0f);

    // --- Determine Final Building Floor Level ---
    float buildingFloorY = BASE_STEP_HEIGHT * 3.0f; // 3 steps high

    // ===================================================
    // 1. THREE STEPPED BASES (From Ground to Building Level)
    // ===================================================
    float currentY = baseY;

    for (int i = 0; i < 3; ++i) {
        glPushMatrix();
        glTranslatef(0.0f, currentY + BASE_STEP_HEIGHT / 2.0f, 0.0f);

        float stepW = MUSEUM_WIDTH + BASE_STEP_OVERHANG * (3 - i) * 2.0f;
        float stepD = MUSEUM_DEPTH + BASE_STEP_OVERHANG * (3 - i) * 2.0f;

        glScalef(stepW, BASE_STEP_HEIGHT, stepD);
        glutSolidCube(1.0f);
        glPopMatrix();

        currentY += BASE_STEP_HEIGHT;
    }


    // ===================================================
    // 2. MAIN BUILDING BODY (Box at Floor Level)
    // ===================================================
    float halfW = MUSEUM_WIDTH / 2.0f;
    float halfD = MUSEUM_DEPTH / 2.0f;

    // Main Block
    glPushMatrix();
    glTranslatef(0.0f, buildingFloorY + towerH / 2.0f, 0.0f);
    glScalef(MUSEUM_WIDTH, towerH, MUSEUM_DEPTH);
    glutSolidCube(1.0f);
    glPopMatrix();

    // --- Front Facade and Steps (simplified) ---
    float facadeW = MUSEUM_WIDTH / 3.0f;
    glPushMatrix();
    glTranslatef(0.0f, buildingFloorY + towerH * 0.5f, halfD + 1.0f);
    glScalef(facadeW, towerH, 2.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Entrance Steps on the building floor
    glPushMatrix();
    glTranslatef(0.0f, buildingFloorY + 1.0f, halfD + 3.0f);
    glScalef(facadeW * 0.8f, 2.0f, 4.0f);
    glutSolidCube(1.0f);
    glPopMatrix();


    // ===================================================
    // 3. GIANT CORNER POLES (Attached to Building Corners)
    // ===================================================
    float poleY = buildingFloorY; // Poles start at the building floor level
    float poleH = POLE_HEIGHT;

    GLfloat poleOffsets[4][3] = {
        {-halfW, poleY, halfD},
        { halfW, poleY, halfD},
        { halfW, poleY, -halfD},
        {-halfW, poleY, -halfD}
    };

    for (int i = 0; i < 4; ++i) {
        glPushMatrix();
        glTranslatef(poleOffsets[i][0], poleOffsets[i][1], poleOffsets[i][2]);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Align with Y-up

        // Pole Body
        drawColumn(colR, colR, poleH);

        // Top Plate (on the building roof level)
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, poleH);
        GLUquadric* quadTop = gluNewQuadric();
        gluDisk(quadTop, 0.0f, colR * 1.5f, 16, 1);
        glPopMatrix();

        glPopMatrix(); // End Pole i Transform
    }


    // ===================================================
    // 4. ROOF (Crystalline Half-Globe)
    // ===================================================
    float roofY = buildingFloorY + towerH; // Top of the main body

    // Crystalline Half-Globe Dome
    glDisable(GL_TEXTURE_2D);
    glPushAttrib(GL_LIGHTING_BIT);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glass_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glass_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glass_specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, glass_shininess);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.8f, 0.9f, 1.0f, 0.1f);

    glPushMatrix();
    glTranslatef(0.0f, roofY, 0.0f); // Position above the roof base

    GLUquadric* quadDome = gluNewQuadric();
    gluQuadricNormals(quadDome, GLU_SMOOTH);
    gluSphere(quadDome, roofR, 32, 16);
    gluDeleteQuadric(quadDome);
    glPopMatrix();

    glDisable(GL_BLEND);
    glPopAttrib();

    // ===================================================
    // 5. SPOTLIGHT SETUP & BEAM RENDERING (Ground-level)
    // ===================================================
    
    // Target Y: Center of the building block
    GLfloat targetY = BASE_STEP_HEIGHT * 4.0f + towerH / 2.0f;
    GLfloat target[] = { MUSEUM_X, targetY, MUSEUM_Z };

    // Determine the corner positions of the widest step (Base 3)
    float base3_W = MUSEUM_WIDTH + BASE_STEP_OVERHANG * 2.0f;
    float base3_D = MUSEUM_DEPTH + BASE_STEP_OVERHANG * 2.0f;

    // Light positions (Ground level, at Base 3 corners)
    GLfloat lightOffsets[4][2] = {
        {-base3_W / 2.0f, -base3_D / 2.0f}, // Back-Left
        { base3_W / 2.0f, -base3_D / 2.0f}, // Back-Right
        { base3_W / 2.0f,  base3_D / 2.0f}, // Front-Right
        {-base3_W / 2.0f,  base3_D / 2.0f}  // Front-Left
    };

    for (int i = 0; i < 4; ++i) {
        // --- Calculate Positions ---
        GLfloat lightX = lightOffsets[i][0];
        GLfloat lightZ = lightOffsets[i][1];
        GLfloat lightY = 2.0f; // Bulb is slightly above the ground

        glPushMatrix();
        glTranslatef(lightX, lightY, lightZ);

        // --- 5a. Render Bulb and Cover (Visual Source) ---
        glDisable(GL_LIGHTING);

        // Cone Cover (Mouth up)
        glColor3f(0.5f, 0.5f, 0.5f);
        glPushMatrix();
        glRotatef(-180.0f, 0.0f, 1.0f, 0.0f);
        
        GLUquadric* quadCover = gluNewQuadric();
        gluCylinder(quadCover, 2.0f, 0.0f, 1.0f, 8, 1);
        gluDeleteQuadric(quadCover);
        glPopMatrix();

        // Bulb (Sphere inside the cover)
        glColor3f(1.0f, 0.8f, 0.8f); // Reddish-White Bulb
        glutSolidSphere(0.5f, 8, 8);

        glEnable(GL_LIGHTING);

        // --- 5b. Light Source (GL_LIGHT) Setup ---

        // Light World Position (relative to MUSEUM_X/Z)
        GLfloat spotPos[] = { MUSEUM_X + lightX, lightY, MUSEUM_Z + lightZ, 1.0f };
        glLightfv(GL_LIGHT5 + i, GL_POSITION, spotPos);

        // Direction: Vector from current bulb to building center
        GLfloat dirX = target[0] - spotPos[0];
        GLfloat dirY = target[1] - spotPos[1];
        GLfloat dirZ = target[2] - spotPos[2];

        float len = sqrt(dirX * dirX + dirY * dirY + dirZ * dirZ);
        GLfloat direction[] = { dirX / len, dirY / len, dirZ / len };
        glLightfv(GL_LIGHT5 + i, GL_SPOT_DIRECTION, direction);

        glEnable(GL_LIGHT5 + i);

        // --- 5c. Visible Light Beam (Translucent Cone) ---
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(1.0f, 0.2f, 0.2f, 0.05f); // Translucent Red Beam

        glPushMatrix();

        // 1. Aim (Yaw: rotation around Y)
        float yaw = atan2(dirX, dirZ) * -180.0f / M_PI;
        glRotatef(yaw, 0.0f, -1.0f, 0.0f);

        // 2. Pitch (Tilt down from vertical)
        float pitch = atan2(dirY, sqrt(dirX * dirX + dirZ * dirZ)) * -180.0f / M_PI;
        glRotatef(pitch + 90.0f, -1.0f, 0.0f, 0.0f); // +90 to align cone (which is along Z)

        // Draw Cone
        GLUquadric* quadBeam = gluNewQuadric();
        float beamLength = 400.0f;
        float beamRadius = beamLength * tan(10.0f * DtoR); // Use 10 deg cutoff

        gluCylinder(quadBeam, 0.0f, beamRadius, beamLength, 16, 1);
        gluDeleteQuadric(quadBeam);

        glPopMatrix(); // End Beam Rotation

        glEnable(GL_LIGHTING);
        glDisable(GL_BLEND);
        glPopMatrix(); // End Bulb/Cover Transform
    }

    
    glPopMatrix(); // End Main Museum Transform
    glPopAttrib(); // Restore global lighting
}

void renderFan() {
    float fanRadius = 8.0f;
    float bladeLength = fanRadius * 0.8f;
    float bladeWidth = 1.5f;
    float poleHeight = 2.0f;
    float poleScaleY = 2.0f;

    float poleEndLocalY = -poleHeight * poleScaleY / 2.0f;

    // --- 1. Metal Pole (Axle) ---
    glColor3f(0.5f, 0.5f, 0.5f); // Metal pole
    glPushMatrix();
    glScalef(0.25f/2, -4.0f, 0.25f/2);
    glutSolidCube(1.0f);
    glPopMatrix();

    // --- 3. Blades (Mounted to the Disk/Pole End) ---
    glPushMatrix();
    glTranslatef(0.0f, poleEndLocalY, 0.0f); // Translate to the disk level
    glRotatef(fanAngle, 0.0f, 1.0f, 0.0f); // Rotate blades

    glColor3f(0.3f, 0.2f, 0.1f); // Wooden blades
    for (int i = 0; i < 4; ++i) {
        glPushMatrix();
        glRotatef((float)i * 90.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(bladeLength / 2.0f, 0.0f, 0.0f);
        glScalef(bladeLength, 0.1f, bladeWidth);
        glutSolidCube(1.0f);
        glPopMatrix();
    }
    glPopMatrix(); // End blades rotation/translation

    // --- 2. Mounting Disk (at the end of the pole) ---
    glColor3f(0.3f, 0.3f, 0.3f); // Darker metal disk
    glPushMatrix();
    glTranslatef(0.0f, poleEndLocalY + 0.2f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Orient to be horizontal (in XY plane)
    GLUquadric* quadDisk = gluNewQuadric();
    gluDisk(quadDisk, 0.0f, 1.5f, 16, 1);
    gluDeleteQuadric(quadDisk);
    glPopMatrix();



    // --- 4. Light Bulb and Cone Cover ---
    float bulbY = poleEndLocalY ; // Position bulb slightly below the disk
    float bulbRadius = 0.2f;
    float coneHeight = 3.0f;
    float coneRadius = 2.0f;

    glTranslatef(0.0f, bulbY, 7.0f); // Ensure we're in the fan's local space
    // --- 1. Metal Pole (Axle) ---
    glColor3f(0.5f, 0.5f, 0.5f); // Metal pole
    glPushMatrix();
    glScalef(0.25f/2, -1.0f, 0.25f/2);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Cone Cover
    glColor3f(0.5f, 0.5f, 0.5f);
    glPushMatrix();
    glTranslatef(0.0f, bulbY / 2.0f, 0.0f);
    glScalef(1.0f / 2, 1.0f / 2, 1.0f / 2);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f); // Orient to point down
    GLUquadric* quadCone = gluNewQuadric();
    gluCylinder(quadCone, coneRadius, 0.0f, coneHeight, 16, 1);
    gluDeleteQuadric(quadCone);
    glPopMatrix();

    if (houseLightOn) {
        GLfloat bulbGlow[] = { 1.0f, 1.0f, 0.8f, 1.0f };
        glMaterialfv(GL_FRONT, GL_EMISSION, bulbGlow);
        glColor3f(1.0f, 1.0f, 0.8f); // Bright yellow/white color
    }
    else {
        GLfloat bulbDark[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glMaterialfv(GL_FRONT, GL_EMISSION, bulbDark);
        glColor3f(0.3f, 0.3f, 0.3f); // Dark grey when off
    }

    // 2. Draw the Bulb Geometry
    // Move down slightly so it peeks out of the cone
    glTranslatef(0.0f, -0.5f, 0.0f);
    glutSolidSphere(bulbRadius, 16, 16);

    // 3. Position the Actual Light (GL_LIGHT3)
    // We are at the center of the bulb sphere right now.
    GLfloat lightPosLocal[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glLightfv(GL_LIGHT3, GL_POSITION, lightPosLocal);

    // 4. Point the Spot Down
    GLfloat spotDirLocal[] = { 0.0f, -1.0f, 0.0f };
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spotDirLocal);

    // 5. Light Settings (Cone width, softness)
    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 60.0f);
    glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 10.0f);

    // 6. Reset Emission
    GLfloat noEmission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);

    glPopMatrix(); // End Bulb Area Transform
}

void renderHouse() {
    // --- House Dimensions and Position ---
    float houseX = 300.0f;
    float houseZ = 50.0f;
    float base_Y = 0.0f;

    // Main body dimensions
    float mW = 40.0f, mH = 20.0f; // Dimensions are 40x20x20
    float mD = 20.0f;
    const float WALL_THICKNESS = 1.0f;

    // Basement dimensions
    float bH = 1.0f;
    float num_basements = 2.0f;
    float total_basement_height = bH * num_basements;

    // Roof dimensions
    float rH = 10.0f;
    float roof_overhang = 3.0f;

    // Detail dimensions
    float doorW = 5.0f, doorH = 10.0f;
    float stepW = 10.0f, stepD = 4.0f, stepH = 0.5f;
    float pipeW = 3.0f, pipeH = 7.0f;

    // Center offsets
    float halfW = mW / 2.0f;
    float halfD = mD / 2.0f;
    float halfH = mH / 2.0f;


    glPushMatrix();
    glTranslatef(houseX, base_Y + total_basement_height, houseZ);

    // ===================================================
    // 1. STACKED BASEMENTS 
    // ===================================================
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, shedTexture);
    glColor3f(1.0f, 1.0f, 1.0f);

    for (int i = 0; i < num_basements; ++i) {
        glPushMatrix();
        glTranslatef(0.0f, -(i * bH + bH / 2.0f), 0.0f);
        glScalef(mW + 4.0f, bH, mD + 4.0f);
        glutSolidCube(1.0f);
        glPopMatrix();
    }

    // ===================================================
    // 2. MAIN BODY WALLS 
    // ===================================================

    float wall_bottom_Y = 0.0f;
    float wall_top_Y = mH;

    // --- LEFT WALL (-X) ---
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, windowsTexture);
    glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    // Vertices MUST be defined properly to create the full surface
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfW, wall_bottom_Y, -halfD);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfW, wall_bottom_Y, halfD);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfW, wall_top_Y, halfD);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfW, wall_top_Y, -halfD);
    glEnd();

    // --- RIGHT WALL (+X) ---
    glBindTexture(GL_TEXTURE_2D, windowsTexture);
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(halfW, wall_bottom_Y, halfD);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfW, wall_bottom_Y, -halfD);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(halfW, wall_top_Y, -halfD);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(halfW, wall_top_Y, halfD);
    glEnd();

    // --- BACK WALL (-Z) ---
    glBindTexture(GL_TEXTURE_2D, houseWallTexture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfW, wall_bottom_Y, -halfD);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfW, wall_bottom_Y, -halfD);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(halfW, wall_top_Y, -halfD);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfW, wall_top_Y, -halfD);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);

    // --- FRONT WALL (+Z) - Door Cutout ---
    glBindTexture(GL_TEXTURE_2D, houseWallTexture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    float front_wall_Z = halfD; // The actual boundary Z
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, treeTrunkTexture);

    // Left of Door 
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfW, 0.0f, front_wall_Z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfW, mH, front_wall_Z);
    glTexCoord2f(0.5f - doorW / mW, 1.0f); glVertex3f(-doorW / 2.0f, mH, front_wall_Z);
    glTexCoord2f(0.5f - doorW / mW, 0.0f); glVertex3f(-doorW / 2.0f, 0.0f, front_wall_Z);

    // Right of Door 
    glTexCoord2f(0.5f + doorW / mW, 0.0f); glVertex3f(doorW / 2.0f, 0.0f, front_wall_Z);
    glTexCoord2f(0.5f + doorW / mW, 1.0f); glVertex3f(doorW / 2.0f, mH, front_wall_Z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(halfW, mH, front_wall_Z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfW, 0.0f, front_wall_Z);

    // Above Door
    float doorTopY = doorH;
    glTexCoord2f(0.5f - doorW / mW, doorTopY / mH); glVertex3f(-doorW / 2.0f, doorTopY, front_wall_Z);
    glTexCoord2f(0.5f - doorW / mW, 1.0f); glVertex3f(-doorW / 2.0f, mH, front_wall_Z);
    glTexCoord2f(0.5f + doorW / mW, 1.0f); glVertex3f(doorW / 2.0f, mH, front_wall_Z);
    glTexCoord2f(0.5f + doorW / mW, doorTopY / mH); glVertex3f(doorW / 2.0f, doorTopY, front_wall_Z);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    // ===================================================
    //  ANIMATED DOOR LOGIC
    // ===================================================
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, doorTexture);
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix(); // Start Door Transform

    // 1. Move the Origin to the "Hinge" (Left side of the door frame)
    //    X = -doorW/2.0f (Left edge of door)
    //    Z = front_wall_Z + 0.1f (Slightly in front of wall)
    glTranslatef(-doorW / 2.0f, 0.0f, front_wall_Z + 0.1f);

    // 2. Rotate around the Hinge (Y-axis)
    glRotatef(currentDoorAngle, 0.0f, 1.0f, 0.0f);

    // 3. Draw the Door (Relative to the Hinge)
    //    Since (0,0,0) is now the hinge, we draw from X=0 to X=doorW
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);

    // Bottom-Left (At Hinge)
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);

    // Bottom-Right (Swing Edge)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(doorW, 0.0f, 0.0f);

    // Top-Right (Swing Edge)
    glTexCoord2f(1.0f, 0.0f); glVertex3f(doorW, doorH, 0.0f);

    // Top-Left (At Hinge)
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, doorH, 0.0f);
    glEnd();

    glPopMatrix(); // End Door Transform
    glDisable(GL_TEXTURE_2D);


    // ===================================================
    // 3. ANGLED ROOF TOP
    // ===================================================
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, roofTileTexture);

    glPushMatrix();
    glTranslatef(0.0f, mH, 0.0f);

    // Gabled Ends (Triangular Faces - adjusted for overhang)
    glBegin(GL_TRIANGLES);
    // Front End (Z + mD/2 + overhang)
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-mW / 2.0f - roof_overhang, 0.0f, mD / 2.0f + roof_overhang);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(mW / 2.0f + roof_overhang, 0.0f, mD / 2.0f + roof_overhang);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, rH, mD / 2.0f + roof_overhang);
    // Back End (Z - mD/2 - overhang)
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-mW / 2.0f - roof_overhang, 0.0f, -mD / 2.0f - roof_overhang);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(mW / 2.0f + roof_overhang, 0.0f, -mD / 2.0f - roof_overhang);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, rH, -mD / 2.0f - roof_overhang);
    glEnd();

    // Sloped Roof Sides (Quads - adjusted for overhang)
    glBegin(GL_QUADS);
    // Left Slope (-X to 0) - Tiling based on length
    float roof_length_tiling = (mD + 2 * roof_overhang) / 10.0f;
    float roof_slope_tiling = (mW / 2.0f + roof_overhang) / 10.0f;

    glNormal3f(-1.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-mW / 2.0f - roof_overhang, 0.0f, mD / 2.0f + roof_overhang);
    glTexCoord2f(roof_length_tiling, 0.0f); glVertex3f(-mW / 2.0f - roof_overhang, 0.0f, -mD / 2.0f - roof_overhang);
    glTexCoord2f(roof_length_tiling, roof_slope_tiling); glVertex3f(0.0f, rH, -mD / 2.0f - roof_overhang);
    glTexCoord2f(0.0f, roof_slope_tiling); glVertex3f(0.0f, rH, mD / 2.0f + roof_overhang);

    // Right Slope (+X to 0)
    glNormal3f(1.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(mW / 2.0f + roof_overhang, 0.0f, mD / 2.0f + roof_overhang);
    glTexCoord2f(roof_length_tiling, 0.0f); glVertex3f(mW / 2.0f + roof_overhang, 0.0f, -mD / 2.0f - roof_overhang);
    glTexCoord2f(roof_length_tiling, roof_slope_tiling); glVertex3f(0.0f, rH, -mD / 2.0f - roof_overhang);
    glTexCoord2f(0.0f, roof_slope_tiling); glVertex3f(0.0f, rH, mD / 2.0f + roof_overhang);
    glEnd();

    glPopMatrix(); // End Roof Transform


    // ===================================================
    // 4. SMOKE PIPE (Chimney)
    // ===================================================
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, chimneyTexture);

    glPushMatrix();
    // Position on the roof ridge
    glTranslatef(0.0f, mH + rH + pipeH / 3.0f, mD / 2.0f - pipeW);
    glScalef(pipeW, pipeH, pipeW);
    glColor3f(1.0f, 1.0f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();



    //================================================== =

    // 5. HOUSE INTERIOR 

    // ===================================================

    float poleEndLocalY = -5.0f * 5.0f / 2.0f; // -12.5f (from renderFan pole scale)

    float bulbY = poleEndLocalY - 1.0f; // -13.5f (from renderFan)



    glDisable(GL_TEXTURE_2D);

    glPushMatrix();

    glTranslatef(0.0f, mH, 0.0f); // Fan's local origin (ceiling level)

    renderFan();

    glPopMatrix();



    glPushMatrix();
    // 1. Move inside the house (Local X, Z)
    // 2. Y-position: 0.0f is the house floor. The table/chair model is drawn from Y=-0.2, 
    //    so we translate up by 0.2 to put its floor on the house floor (Y=0).
    glTranslatef(houseX, 5.0f, houseZ-5.0f);

    // 3. Rotate to face center
    glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);

    // 4. Draw
    renderChairAndTable();
    glPopMatrix();




    

    // 8. Actually Toggle the Light Switch
    if (houseLightOn) {
        glEnable(GL_LIGHT3);
    }
    else {
        glDisable(GL_LIGHT3);
    }




    glPopMatrix(); // End Main House Transform
}

void renderVerticalExtendedHouse() {
    // --- House Dimensions and Position ---
    float houseX = 100.0f;
    float houseZ = 40.0f;
    float base_Y = 0.0f;


    float pipeW = 3.0f, pipeH = 7.0f;

    // Main body dimensions
    float mH = 30.0f;           // Vertically Extended Height (from previous solution)
    float front_back_D = 25.0f; // Original Depth (D) of Front/Back walls
    float front_back_W = 40.0f; // Original Width (W) of Front/Back walls

    // EXTEND WALLS: Left/Right walls (W) are 1.5x the length of Front/Back walls (D)
    float left_right_D = front_back_D * 1.2f;
    float left_right_W = front_back_W * 1.2f;

    // Basement dimensions
    float bH = 2.0f;
    float num_basements = 2.0f;
    float total_basement_height = bH * num_basements;

    // Roof dimensions
    float rH = 10.0f;
    float roof_overhang = 3.0f; // Extended overhang

    // Detail dimensions
    float doorW = 5.0f, doorH = 10.0f;
    float stepW = 10.0f, stepD = 4.0f, stepH = 0.5f;

    glPushMatrix();
    // Position the house so the main floor (Y=0) sits on top of the stacked basements
    glTranslatef(houseX, base_Y + total_basement_height, houseZ);
    glColor3f(1.0f, 1.0f, 1.0f); // Base color for textured objects

    // ===================================================
    // 1. STACKED BASEMENTS (Located on the flat ground)
    // ===================================================
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, basementTexture);

    for (int i = 0; i < num_basements; ++i) {
        glPushMatrix();
        glTranslatef(0.0f, -(i * bH + bH / 2.0f), 0.0f);

        // Scale to the longest wall dimensions for a uniform base
        glScalef(left_right_W + 4.0f, bH, left_right_D + 4.0f);
        glutSolidCube(1.0f);
        glPopMatrix();
    }

    glDisable(GL_TEXTURE_2D);

    // ===================================================
    // 2. MAIN BODY WALLS (Textured Quads & Extensions)
    // ===================================================
    float wall_center_Y = mH / 2.0f;

    // --- Wall Drawing Helper with Fixed TexCoords ---
    auto drawTexturedWall = [&](GLfloat w, GLfloat d, GLfloat x, GLfloat z, GLfloat normalX, GLfloat normalZ, GLuint textureID, bool isFrontBack) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        glNormal3f(normalX, 0.0f, normalZ);

        // Calculate tiling factor (e.g., 4 tiles across the wall)
        float tilesX = isFrontBack ? w / 10.0f : d / 10.0f;
        float tilesY = mH / 10.0f;

        if (normalX == 1.0f) { // RIGHT WALL (+X)
            glTexCoord2f(0.0f, 0.0f); glVertex3f(x, 0.0f, z - d / 2);
            glTexCoord2f(tilesX, 0.0f); glVertex3f(x, 0.0f, z + d / 2);
            glTexCoord2f(tilesX, tilesY); glVertex3f(x, mH, z + d / 2);
            glTexCoord2f(0.0f, tilesY); glVertex3f(x, mH, z - d / 2);
        }
        else if (normalX == -1.0f) { // LEFT WALL (-X)
            glTexCoord2f(0.0f, 0.0f); glVertex3f(x, 0.0f, z + d / 2);
            glTexCoord2f(tilesX, 0.0f); glVertex3f(x, 0.0f, z - d / 2);
            glTexCoord2f(tilesX, tilesY); glVertex3f(x, mH, z - d / 2);
            glTexCoord2f(0.0f, tilesY); glVertex3f(x, mH, z + d / 2);
        }
        else if (normalZ == 1.0f) { // FRONT WALL (+Z)
            glTexCoord2f(0.0f, 0.0f); glVertex3f(x - w / 2, 0.0f, z);
            glTexCoord2f(tilesX, 0.0f); glVertex3f(x + w / 2, 0.0f, z);
            glTexCoord2f(tilesX, tilesY); glVertex3f(x + w / 2, mH, z);
            glTexCoord2f(0.0f, tilesY); glVertex3f(x - w / 2, mH, z);
        }
        else if (normalZ == -1.0f) { // BACK WALL (-Z)
            glTexCoord2f(tilesX, 0.0f); glVertex3f(x - w / 2, 0.0f, z);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(x + w / 2, 0.0f, z);
            glTexCoord2f(0.0f, tilesY); glVertex3f(x + w / 2, mH, z);
            glTexCoord2f(tilesX, tilesY); glVertex3f(x - w / 2, mH, z);
        }
        glEnd();
        glDisable(GL_TEXTURE_2D);
        };

    // A. LEFT WALL (-X side) - Uses the integrated windows texture
    drawTexturedWall(left_right_D, mH, -front_back_W / 2.0f, 0.0f, -1.0f, 0.0f, windowsTexture, false);

    // B. RIGHT WALL (+X side) - Uses the integrated windows texture
    drawTexturedWall(left_right_D, mH, front_back_W / 2.0f, 0.0f, 1.0f, 0.0f, windowsTexture, false);

    // C. BACK WALL (-Z side) - Simple house wall texture
    drawTexturedWall(front_back_W, mH, 0.0f, -left_right_D / 2.0f, 0.0f, -1.0f, houseWallTexture, true);

    // D. FRONT WALL (+Z side) - Door Cutout (Still needed for the hole)
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, houseWallTexture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);

    // ... (Door Cutout Quads and Door Solid Quad - Using front_back_W)
    // Left of Door
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-front_back_W / 2.0f, 0.0f, left_right_D / 2.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-front_back_W / 2.0f, mH, left_right_D / 2.0f);
    glTexCoord2f(0.5f - doorW / front_back_W, 1.0f); glVertex3f(-doorW / 2.0f, mH, left_right_D / 2.0f);
    glTexCoord2f(0.5f - doorW / front_back_W, 0.0f); glVertex3f(-doorW / 2.0f, 0.0f, left_right_D / 2.0f);

    // Right of Door
    glTexCoord2f(0.5f + doorW / front_back_W, 0.0f); glVertex3f(doorW / 2.0f, 0.0f, left_right_D / 2.0f);
    glTexCoord2f(0.5f + doorW / front_back_W, 1.0f); glVertex3f(doorW / 2.0f, mH, left_right_D / 2.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(front_back_W / 2.0f, mH, left_right_D / 2.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(front_back_W / 2.0f, 0.0f, left_right_D / 2.0f);

    // Above Door
    float doorTopY = doorH;
    glTexCoord2f(0.5f - doorW / front_back_W, doorTopY / mH); glVertex3f(-doorW / 2.0f, doorTopY, left_right_D / 2.0f);
    glTexCoord2f(0.5f - doorW / front_back_W, 1.0f); glVertex3f(-doorW / 2.0f, mH, left_right_D / 2.0f);
    glTexCoord2f(0.5f + doorW / front_back_W, 1.0f); glVertex3f(doorW / 2.0f, mH, left_right_D / 2.0f);
    glTexCoord2f(0.5f + doorW / front_back_W, doorTopY / mH); glVertex3f(doorW / 2.0f, doorTopY, left_right_D / 2.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    // Door Quad (Solid Color)
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, doorTexture);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-doorW / 2.0f, 0.0f, left_right_D / 2.0f + 0.1f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(doorW / 2.0f, 0.0f, left_right_D / 2.0f + 0.1f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(doorW / 2.0f, doorH, left_right_D / 2.0f + 0.1f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-doorW / 2.0f, doorH, left_right_D / 2.0f + 0.1f);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    // ===================================================
    // 3. ANGLED ROOF TOP (Extended & Textured)
    // ===================================================
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, roofTileTexture);

    glPushMatrix();
    glTranslatef(0.0f, mH, 0.0f);

    float ext_W = front_back_W / 2.0f + roof_overhang;
    float ext_D = left_right_D / 2.0f + roof_overhang;

    // Gabled Ends (Triangular Faces)
    glBegin(GL_TRIANGLES);
    // Front End (+Z side)
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-ext_W, 0.0f, ext_D);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(ext_W, 0.0f, ext_D);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, rH, ext_D);
    // Back End (-Z side)
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-ext_W, 0.0f, -ext_D);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(ext_W, 0.0f, -ext_D);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, rH, -ext_D);
    glEnd();

    // Sloped Roof Sides (Quads)
    float roof_length_tiling = (left_right_D + 2 * roof_overhang) / 10.0f;
    float roof_slope_tiling = (front_back_W / 2.0f + roof_overhang) / 10.0f;

    glBegin(GL_QUADS);
    // Left Slope (-X side)
    glNormal3f(-1.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-ext_W, 0.0f, ext_D);
    glTexCoord2f(roof_length_tiling, 0.0f); glVertex3f(-ext_W, 0.0f, -ext_D);
    glTexCoord2f(roof_length_tiling, roof_slope_tiling); glVertex3f(0.0f, rH, -ext_D);
    glTexCoord2f(0.0f, roof_slope_tiling); glVertex3f(0.0f, rH, ext_D);

    // Right Slope (+X side)
    glNormal3f(1.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(ext_W, 0.0f, ext_D);
    glTexCoord2f(roof_length_tiling, 0.0f); glVertex3f(ext_W, 0.0f, -ext_D);
    glTexCoord2f(roof_length_tiling, roof_slope_tiling); glVertex3f(0.0f, rH, -ext_D);
    glTexCoord2f(0.0f, roof_slope_tiling); glVertex3f(0.0f, rH, ext_D);
    glEnd();

    glPopMatrix(); // End Roof Transform


    // ===================================================
    // 4. SMOKE PIPE (Chimney)
    // ===================================================
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, chimneyTexture);

    glPushMatrix();
    glTranslatef(0.0f, mH + rH + pipeH / 3.0f, ext_D - 1.0f); // Adjust Z position for overhang
    glScalef(pipeW, pipeH, pipeW);
    glColor3f(1.0f, 1.0f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();






    glPopMatrix(); // End Main House Transform
}

void renderComplexHouse() {
    // --- House Dimensions and Position ---
    float houseX = 250.0f;
    float houseZ = 130.0f;
    float base_Y = 0.0f;

    // Main body dimensions
    float mW = 55.0f;
    float mH = 18.0f;
    float mD = 25.0f;
    const float WALL_THICKNESS = 1.0f;

    // Roof dimensions
    float rH = 10.0f;
    float roof_overhang = 2.0f; // EXTEND ROOF BOUNDARIES

    // Detail dimensions
    float doorW = 5.0f, doorH = 10.0f;
    float winW = 4.0f, winH = 6.0f;
    float pipeW = 3.0f, pipeH = 7.0f;
    float stepW = 10.0f, stepD = 4.0f, stepH = 0.5f;

    // Center offsets
    float halfW = mW / 2.0f;
    float halfD = mD / 2.0f;

    // Note: This house sits directly on Y=0 (no basement offset)
    float total_basement_height = 0.0f;

    glPushMatrix();
    glTranslatef(houseX, base_Y, houseZ);

    // ===================================================
    // 1. BASEMENT & MAIN BODY 
    // ===================================================


    float wall_bottom_Y = 0.0f;
    float wall_top_Y = mH;

    // Apply initial blend/color for side/back walls
    glEnable(GL_TEXTURE_2D);
    glColor4f(1.0f, 1.0f, 1.0f, 0.7f);

    // --- LEFT WALL (-X) ---
    glBindTexture(GL_TEXTURE_2D, houseWallTexture);
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfW, wall_bottom_Y, -halfD);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-halfW, wall_bottom_Y, halfD);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-halfW, wall_top_Y, halfD);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfW, wall_top_Y, -halfD);
    glEnd();

    // --- RIGHT WALL (+X) ---
    glBindTexture(GL_TEXTURE_2D, houseWallTexture);
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(halfW, wall_bottom_Y, halfD);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfW, wall_bottom_Y, -halfD);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(halfW, wall_top_Y, -halfD);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(halfW, wall_top_Y, halfD);
    glEnd();

    // --- BACK WALL (-Z) ---
    glBindTexture(GL_TEXTURE_2D, houseWallTexture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfW, wall_bottom_Y, -halfD);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfW, wall_bottom_Y, -halfD);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(halfW, wall_top_Y, -halfD);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfW, wall_top_Y, -halfD);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);

    // --- FRONT WALL (+Z) - Door Cutout ---
    glBindTexture(GL_TEXTURE_2D, houseWallTexture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    float front_wall_Z = halfD;

    // Left of Door 
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfW, 0.0f, front_wall_Z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-halfW, mH, front_wall_Z);
    glTexCoord2f(0.5f - doorW / mW, 1.0f); glVertex3f(-doorW / 2.0f, mH, front_wall_Z);
    glTexCoord2f(0.5f - doorW / mW, 0.0f); glVertex3f(-doorW / 2.0f, 0.0f, front_wall_Z);

    // Right of Door 
    glTexCoord2f(0.5f + doorW / mW, 0.0f); glVertex3f(doorW / 2.0f, 0.0f, front_wall_Z);
    glTexCoord2f(0.5f + doorW / mW, 1.0f); glVertex3f(doorW / 2.0f, mH, front_wall_Z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(halfW, mH, front_wall_Z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfW, 0.0f, front_wall_Z);

    // Above Door
    float doorTopY = doorH;
    glTexCoord2f(0.5f - doorW / mW, doorTopY / mH); glVertex3f(-doorW / 2.0f, doorTopY, front_wall_Z);
    glTexCoord2f(0.5f - doorW / mW, 1.0f); glVertex3f(-doorW / 2.0f, mH, front_wall_Z);
    glTexCoord2f(0.5f + doorW / mW, 1.0f); glVertex3f(doorW / 2.0f, mH, front_wall_Z);
    glTexCoord2f(0.5f + doorW / mW, doorTopY / mH); glVertex3f(doorW / 2.0f, doorTopY, front_wall_Z);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    // ===================================================
    //  ANIMATED DOOR LOGIC
    // ===================================================
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, doorTexture);
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix(); // Start Door Transform

    // 1. Move the Origin to the "Hinge" (Left side of the door frame)
    //    X = -doorW/2.0f (Left edge of door)
    //    Z = front_wall_Z + 0.1f (Slightly in front of wall)
    glTranslatef(-doorW / 2.0f, 0.0f, front_wall_Z + 0.1f);

    // 2. Rotate around the Hinge (Y-axis)
    glRotatef(currentDoorAngle, 0.0f, 1.0f, 0.0f);

    // 3. Draw the Door (Relative to the Hinge)
    //    Since (0,0,0) is now the hinge, we draw from X=0 to X=doorW
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);

    // Bottom-Left (At Hinge)
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);

    // Bottom-Right (Swing Edge)
    glTexCoord2f(0.0f, 0.0f); glVertex3f(doorW, 0.0f, 0.0f);

    // Top-Right (Swing Edge)
    glTexCoord2f(1.0f, 0.0f); glVertex3f(doorW, doorH, 0.0f);

    // Top-Left (At Hinge)
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, doorH, 0.0f);
    glEnd();

    glPopMatrix(); // End Door Transform
    glDisable(GL_TEXTURE_2D);


    // ===================================================
    // 3. ANGLED ROOF TOP & 5. SMOKE PIPE 
    // ===================================================
    glColor3f(1.0f, 1.0f, 1.0f); // Roof
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, roofTileTexture);

    glPushMatrix();
    glTranslatef(0.0f, mH, 0.0f);

    // Gabled Ends (Triangular Faces)
    glBegin(GL_TRIANGLES);
    // Front End (Z + mD/2 + overhang)
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfW - roof_overhang, 0.0f, mD / 2.0f + roof_overhang);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfW + roof_overhang, 0.0f, mD / 2.0f + roof_overhang);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, rH, mD / 2.0f + roof_overhang);
    // Back End (Z - mD/2 - overhang)
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfW - roof_overhang, 0.0f, -mD / 2.0f - roof_overhang);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(halfW + roof_overhang, 0.0f, -mD / 2.0f - roof_overhang);
    glTexCoord2f(0.5f, 1.0f); glVertex3f(0.0f, rH, -mD / 2.0f - roof_overhang);
    glEnd();

    // Sloped Roof Sides (Quads)
    float roof_length_tiling = (mD + 2 * roof_overhang) / 10.0f;
    float roof_slope_tiling = (mW / 2.0f + roof_overhang) / 10.0f;

    glBegin(GL_QUADS);
    // Left Slope (-X to 0)
    glNormal3f(-1.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-halfW - roof_overhang, 0.0f, mD / 2.0f + roof_overhang);
    glTexCoord2f(roof_length_tiling, 0.0f); glVertex3f(-halfW - roof_overhang, 0.0f, -mD / 2.0f - roof_overhang);
    glTexCoord2f(roof_length_tiling, roof_slope_tiling); glVertex3f(0.0f, rH, -mD / 2.0f - roof_overhang);
    glTexCoord2f(0.0f, roof_slope_tiling); glVertex3f(0.0f, rH, mD / 2.0f + roof_overhang);

    // Right Slope (+X to 0)
    glNormal3f(1.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(halfW + roof_overhang, 0.0f, mD / 2.0f + roof_overhang);
    glTexCoord2f(roof_length_tiling, 0.0f); glVertex3f(halfW + roof_overhang, 0.0f, -mD / 2.0f - roof_overhang);
    glTexCoord2f(roof_length_tiling, roof_slope_tiling); glVertex3f(0.0f, rH, -mD / 2.0f - roof_overhang);
    glTexCoord2f(0.0f, roof_slope_tiling); glVertex3f(0.0f, rH, mD / 2.0f + roof_overhang);
    glEnd();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    // Chimney
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, chimneyTexture);
    glPushMatrix();
    glTranslatef(0.0f, mH + rH + pipeH / 3.0f, mD / 2.0f - pipeW);
    glScalef(pipeW, pipeH, pipeW);
    glColor3f(1.0f, 1.0f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    // ===================================================
    // 6. HOUSE INTERIOR 
    // ===================================================
    float poleEndLocalY = -5.0f * 5.0f / 2.0f;
    float bulbY = poleEndLocalY - 1.0f;

    glPushMatrix();
    glTranslatef(0.0f, mH - 1.0f, 0.0f);
    renderFan();
    glPopMatrix();

    /*if (houseLightOn) {
        glEnable(GL_LIGHT3);
    }
    else {
        glDisable(GL_LIGHT3);
    }*/


    glPushMatrix();
    glTranslatef(houseX-10.0f, 5.0f, houseZ); // Move to a corner
    renderChairAndTable();
    glPopMatrix();

    

    

    glPopMatrix(); // End Main House Transform
}

void renderLighthouse() {
    float baseY = 0.0f;
    float currentY = baseY;

    glPushMatrix();
    glTranslatef(LIGHTHOUSE_X, baseY, LIGHTHOUSE_Z);

    // ===================================================
    // 1. LIGHTHOUSE BASE & 2. MAIN TOWER - 
    // ===================================================
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, lighthouseBaseTexture);
    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    glTranslatef(0.0f, currentY + LIGHTHOUSE_BASE_H / 2.0f, 0.0f);
    glScalef(LIGHTHOUSE_BASE_SIZE, LIGHTHOUSE_BASE_H, LIGHTHOUSE_BASE_SIZE);
    glutSolidCube(1.0f);
    glPopMatrix();

    currentY += LIGHTHOUSE_BASE_H;

    glPushMatrix();
    glTranslatef(0.0f, currentY, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluQuadricTexture(quad, GL_TRUE);

    glBindTexture(GL_TEXTURE_2D, lighthouseBaseTexture);
    glColor3f(1.0f, 1.0f, 1.0f);
    gluCylinder(quad, LIGHTHOUSE_TOWER_R_BOT, LIGHTHOUSE_TOWER_R_TOP, LIGHTHOUSE_TOWER_H, 32, 32);

    glDisable(GL_TEXTURE_2D);
    glColor3f(0.95f, 0.95f, 0.95f);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, LIGHTHOUSE_TOWER_H);
    gluDisk(quad, 0.0f, LIGHTHOUSE_TOWER_R_TOP, 32, 1);
    glPopMatrix();
    gluDeleteQuadric(quad);
    glPopMatrix();

    // --- Add Windows to the Tower --- 
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, woodTexture);
    for (int i = 0; i < 4; ++i) {
        float angle = (float)i * 90.0f;
        glPushMatrix();
        glTranslatef(0.0f, currentY + LIGHTHOUSE_TOWER_H / 2.0f, 0.0f);
        glRotatef(angle, 0.0f, 1.0f, 0.0f);
        float avgR = (LIGHTHOUSE_TOWER_R_BOT + LIGHTHOUSE_TOWER_R_TOP) / 2.0f;
        glTranslatef(avgR, 0.0f, 0.0f);
        glBegin(GL_QUADS);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, -WINDOW_HEIGHT / 2.0f, -WINDOW_WIDTH / 2.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0f, -WINDOW_HEIGHT / 2.0f, WINDOW_WIDTH / 2.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0f, WINDOW_HEIGHT / 2.0f, WINDOW_WIDTH / 2.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, WINDOW_HEIGHT / 2.0f, -WINDOW_WIDTH / 2.0f);
        glEnd();
        glPopMatrix();
    }
    glDisable(GL_TEXTURE_2D);

    currentY += LIGHTHOUSE_TOWER_H;

    // --- Add Disk below the Glass Globe 
    glColor3f(0.6f, 0.6f, 0.6f);
    glPushMatrix();
    glTranslatef(0.0f, currentY, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    GLUquadric* quadDisk = gluNewQuadric();
    gluDisk(quadDisk, LIGHTHOUSE_TOWER_R_TOP, LANTERN_RADIUS * 1.5f, 32, 1);
    gluDeleteQuadric(quadDisk);
    glPopMatrix();

    // ===================================================
    // 3. LANTERN ROOM & GLASS GLOBE 
    // ===================================================
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, lighthouseTopTexture);
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, currentY + 0.5f, 0.0f);
    glScalef(LANTERN_RADIUS * 1.5f, 1.0f, LANTERN_RADIUS * 1.5f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glPushAttrib(GL_LIGHTING_BIT);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glass_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glass_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glass_specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, glass_shininess);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.6f, 0.8f, 1.0f, 0.2f);

    glPushMatrix();
    glTranslatef(0.0f, currentY + 2.0f, 0.0f);
    GLUquadric* quadGlass = gluNewQuadric();
    gluQuadricNormals(quadGlass, GLU_SMOOTH);
    gluSphere(quadGlass, LANTERN_RADIUS, 32, 32);
    gluDeleteQuadric(quadGlass);
    glPopMatrix();
    glDisable(GL_BLEND);
    glPopAttrib();

    // --- 3c. Rotating Inner Globe Light Source & Light Position/Direction ---

    // START ROTATION TRANSFORMATIONS
    glPushMatrix();
    glTranslatef(0.0f, currentY + 2.0f, 0.0f); // Translate to the center of the globe
    glRotatef(spotAngle, 0.0f, 1.0f, 0.0f); // Apply the rotation

    // Draw the globe light source
    glColor3f(1.0f, 1.0f, 0.8f);
    glDisable(GL_LIGHTING);
    glutSolidSphere(LANTERN_RADIUS * 0.2f, 16, 16);
    glEnable(GL_LIGHTING);

    // ===================================================
    // 4. SPOTLIGHT PLACEMENT (GL_LIGHT2)
    // ===================================================

    GLfloat lightPosition[] = {
        LIGHTHOUSE_X,
        baseY + LIGHT_HEIGHT_OFFSET - 3.0f,
        LIGHTHOUSE_Z,
        1.0f
    };
    glLightfv(GL_LIGHT2, GL_POSITION, lightPosition);

    // Direction: Emitted from the rotating globe
    GLfloat lightDirection[3];
    lightDirection[0] = cos(spotAngle * DtoR);
    lightDirection[1] = 0.0f;
    lightDirection[2] = sin(spotAngle * DtoR);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lightDirection);


    // ===================================================
    // 5. VISIBLE LIGHT BEAM (Translucent Cone/Frustum)
    // ===================================================
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0f, 1.0f, 0.8f, 0.08f);

    glPushMatrix();

    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

    GLUquadric* quadBeam = gluNewQuadric();
    float beamCutoff = 15.0f;
    float beamLength = 400.0f;
    float beamRadius = beamLength * tan(beamCutoff * DtoR);

    gluCylinder(quadBeam, 0.0f, beamRadius, beamLength, 16, 1);
    gluDeleteQuadric(quadBeam);

    glPopMatrix(); // End Light Beam Transform

    // Restore state
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);


    glPopMatrix(); // End Inner Globe Rotation Transform

    glPopMatrix(); // End Lighthouse Transform
}

void drawBulgingBarrel(float radius, float height, float bulgeFactor) {
    int segments = 32; // Circumferential segments
    float numRings = 16.0f; // Height subdivisions for the bulge curve
    float halfHeight = height / 2.0f;

    // Texture coordinates for the barrel body (assuming central 50% of the image)
    const float TEX_U_MIN = 0.25f;
    const float TEX_U_MAX = 0.75f;
    const float TEX_V_MIN = 0.0f;
    const float TEX_V_MAX = 1.0f;

    // --- 1. Bulging Cylinder Body (GL_QUAD_STRIPs for rings) ---
    for (int j = 0; j < numRings; ++j) {
        glBegin(GL_QUAD_STRIP);

        float z1 = (float)j / numRings * height - halfHeight;
        float z2 = (float)(j + 1) / numRings * height - halfHeight;

        // Normalized Z for bulge curve (-1.0 to 1.0)
        float r1_z = z1 / halfHeight;
        float r2_z = z2 / halfHeight;

        // Bulge calculation: R = baseR + Bulge * (1 - z^2)
        float r1 = radius + bulgeFactor * (1.0f - r1_z * r1_z);
        float r2 = radius + bulgeFactor * (1.0f - r2_z * r2_z);

        // Texture V coordinates (mapped to the height of the texture)
        float texV1 = TEX_V_MIN + (TEX_V_MAX - TEX_V_MIN) * ((float)j / numRings);
        float texV2 = TEX_V_MIN + (TEX_V_MAX - TEX_V_MIN) * ((float)(j + 1) / numRings);

        for (int i = 0; i <= segments; ++i) {
            float angle = (float)i / segments * 2.0f * M_PI;
            float cosA = cos(angle);
            float sinA = sin(angle);

            float texU = TEX_U_MIN + (TEX_U_MAX - TEX_U_MIN) * ((float)i / segments);

            // Vertex 1 (Bottom ring of the current strip)
            glNormal3f(r1 * cosA, r1 * sinA, 0.0f); // Simple radial normal
            glTexCoord2f(texU, texV1);
            glVertex3f(r1 * cosA, r1 * sinA, z1);

            // Vertex 2 (Top ring of the current strip)
            glNormal3f(r2 * cosA, r2 * sinA, 0.0f);
            glTexCoord2f(texU, texV2);
            glVertex3f(r2 * cosA, r2 * sinA, z2);
        }
        glEnd();
    }

    // --- 2. Circular Caps (GL_TRIANGLE_FAN) ---
    float capZ_pos = halfHeight;
    float capZ_neg = -halfHeight;
    float capR = radius + bulgeFactor * (1.0f - 1.0f); // Radius at Z=halfHeight or Z=-halfHeight

    // Top Cap (+Z) - Use first texture circle (U: 0.0 to 0.25, V: 0.0 to 0.5)
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.125f, 0.25f); // Center of texture circle (U: 0.0-0.25, V: 0.0-0.5)
    glVertex3f(0.0f, 0.0f, capZ_pos);

    for (int i = 0; i <= segments; ++i) {
        float angle = (float)i / segments * 2.0f * M_PI;
        float cosA = cos(angle);
        float sinA = sin(angle);

        // Map the circle to the texture square
        float texU = 0.125f + cosA * 0.125f;
        float texV = 0.25f + sinA * 0.25f;

        glTexCoord2f(texU, texV);
        glVertex3f(capR * cosA, capR * sinA, capZ_pos);
    }
    glEnd();

    // Bottom Cap (-Z) - Use second texture circle (U: 0.0 to 0.25, V: 0.5 to 1.0)
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.125f, 0.75f); // Center of texture circle (U: 0.0-0.25, V: 0.5-1.0)
    glVertex3f(0.0f, 0.0f, capZ_neg);

    for (int i = 0; i <= segments; ++i) {
        float angle = (float)i / segments * 2.0f * M_PI;
        float cosA = cos(angle);
        float sinA = sin(angle);

        // Map the circle to the texture square
        float texU = 0.125f + cosA * 0.125f;
        float texV = 0.75f + sinA * 0.25f;

        glTexCoord2f(texU, texV);
        glVertex3f(capR * cosA, capR * sinA, capZ_neg);
    }
    glEnd();
}

void renderBarrels() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, barrelTexture);

    float radius = 2.0f;
    float height = 6.5f;
    float bulge = 1.0f;

    // --- 1. Static Barrels ---

    // Barrel 1: Standing (Right of Main House)
    glPushMatrix();
    glTranslatef(135.0f, radius, 110.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Z-axis is up
    drawBulgingBarrel(radius, height, bulge);
    glPopMatrix();

    // Barrel 2: Standing (Front-Center)
    glPushMatrix();
    glTranslatef(105.0f, radius, 130.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    drawBulgingBarrel(radius, height, bulge);
    glPopMatrix();

    // Barrel 3: Lying Down (Front-Left)
    glPushMatrix();
    glTranslatef(85.0f, radius, 130.0f);
    glRotatef(10.0f, 1.0f, 0.0f, 0.0f);
    drawBulgingBarrel(radius, height * 1.5f, bulge);
    glPopMatrix();


    // --- 2. Rolling Barrels ---
    float rollRadius = 4.0f;
    float rollHeight = 8.0f;
    float rollBulge = 1.2f;

    for (int i = 0; i < NUM_ROLLING_BARRELS; i++) {
        glPushMatrix();

        float x = 50.0f + (float)i * 10.0f;
        float z = 450.0f + (float)i * 15.0f;

        glTranslatef(x, rollRadius, z);

        glRotatef(30.0f, 0.0f, 1.0f, 0.0f);

        glRotatef(barrelRollAngles[i], 0.0f, 0.0f, 1.0f);

        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

        drawBulgingBarrel(rollRadius, rollHeight, rollBulge);

        glPopMatrix();
    }

    glDisable(GL_TEXTURE_2D);
}

void drawWaterSurface(float centerX, float centerZ, int size, float waveTimer, float tilingFactor) {

    // Waves parameters (Slightly randomized for variety)
    float maxWaveHeight = 1.0f + (float)(rand() % 5) / 10.0f;
    float frequency = 0.05f + (float)(rand() % 2) / 100.0f;

    for (int x = 0; x < size - 1; x += 1) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int z = 0; z < size; z += 1) {

            // Calculate a point on the water surface (relative to lake center)
            float worldX1 = centerX - (float)size / 2.0f + (float)x;
            float worldZ1 = centerZ - (float)size / 2.0f + (float)z;
            float worldX2 = centerX - (float)size / 2.0f + (float)(x + 1);
            float worldZ2 = centerZ - (float)size / 2.0f + (float)z;

            // Simple wave function (sum of two sinusoids for complex motion)
            float wave1_z1 = sin(worldX1 * frequency + waveTimer * 1.5f) * cos(worldZ1 * frequency * 0.5f) * maxWaveHeight;
            float wave2_z1 = sin(worldZ1 * frequency * 1.2f + waveTimer * 0.8f) * maxWaveHeight * 0.5f;
            float waveY1 = waterLevel + wave1_z1 + wave2_z1;

            float wave1_z2 = sin(worldX2 * frequency + waveTimer * 1.5f) * cos(worldZ2 * frequency * 0.5f) * maxWaveHeight;
            float wave2_z2 = sin(worldZ2 * frequency * 1.2f + waveTimer * 0.8f) * maxWaveHeight * 0.5f;
            float waveY2 = waterLevel + wave1_z2 + wave2_z2;


            // Normal approximation: Just point straight up for simplicity with small waves
            glNormal3f(0.0f, 1.0f, 0.0f);

            // Vertex 1: (x, z)
            glTexCoord2f((float)x / (float)size * tilingFactor, (float)z / (float)size * tilingFactor);
            glVertex3f(worldX1, waveY1, worldZ1);

            // Vertex 2: (x+1, z)
            glTexCoord2f((float)(x + 1) / (float)size * tilingFactor, (float)z / (float)size * tilingFactor);
            glVertex3f(worldX2, waveY2, worldZ2);
        }
        glEnd();
    }
}

void renderLake() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, waterTexture);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set a translucent color 
    glColor4f(0.3f, 0.6f, 1.0f, 0.7f);

    drawWaterSurface(LAKE_X, LAKE_Z, LAKE_SIZE, waveTimer, 15.0f);

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void renderSmallLakes() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, waterTexture);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.3f, 0.6f, 1.0f, 0.7f);

    srand(200); // Seed for reproducible random positions

    for (int i = 0; i < NUM_SMALL_LAKES; ++i) {
        // Position far away from the main scene, but within the extended flat ground
        float randX = -600.0f + (float)(rand() % 1200);
        float randZ = -600.0f + (float)(rand() % 1200);

        // Ensure not too close to the main lake or houses
        if (abs(randX - LAKE_X) < 300.0f && abs(randZ - LAKE_Z) < 300.0f) continue;
        if (abs(randX - 100.0f) < 200.0f && abs(randZ - 100.0f) < 200.0f) continue;

        int rSize = MIN_LAKE_SIZE + (rand() % (MAX_LAKE_SIZE - MIN_LAKE_SIZE));
        float rTiling = 5.0f + (float)(rand() % 10);

        drawWaterSurface(randX, randZ, rSize, waveTimer, rTiling);
    }

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void renderBridge() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, bridgeTexture); // Reuse existing wood texture
    glColor3f(1.0f, 1.0f, 1.0f);

    // Bridge dimensions
    float length = BRIDGE_LENGTH;
    float width = BRIDGE_WIDTH;
    float height = 0.5f;

    // Bridge position (across the center of the lake, along X axis)
    float startX = LAKE_X - length / 2.0f;
    float endX = LAKE_X + length / 2.0f;
    float bridgeZ = LAKE_Z + 10.0f;
    float bridgeY = BRIDGE_Y;

    // --- Deck Planks (Simple Solid Cube) ---
    glPushMatrix();
    glTranslatef(LAKE_X, bridgeY + height / 2.0f, bridgeZ);
    glScalef(length, height, width);
    glutSolidCube(1.0f);
    glPopMatrix();

    // --- Railings (Thin bars on the side) ---
    float railH = 3.0f;
    float railW = 0.5f;
    float railSpacing = 10.0f;
    int numRails = (int)(length / railSpacing);

    for (int i = 0; i <= numRails; ++i) {
        float currentX = startX + (float)i * railSpacing;

        // Left Rail Post
        glPushMatrix();
        glTranslatef(currentX, bridgeY + height + railH / 2.0f, bridgeZ - width / 2.0f);
        glScalef(railW, railH, railW);
        glutSolidCube(1.0f);
        glPopMatrix();

        // Right Rail Post
        glPushMatrix();
        glTranslatef(currentX, bridgeY + height + railH / 2.0f, bridgeZ + width / 2.0f);
        glScalef(railW, railH, railW);
        glutSolidCube(1.0f);
        glPopMatrix();
    }

    glDisable(GL_TEXTURE_2D);
}

void renderWoods() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, treeTrunkTexture);

    // Plank 1: Leaning against the main house (Long and thin)
    glPushMatrix();
    glTranslatef(120.0f, 0.0f, 95.0f);
    glRotatef(-15.0f, 0.0f, 0.0f, 1.0f); // Lean forward slightly
    glRotatef(20.0f, 0.0f, 1.0f, 0.0f);  // Angled away from wall
    glScalef(15.0f, 1.0f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Planks 2 & 3: Stacked/Scattered Logs (Front-Left)
    glPushMatrix();
    glTranslatef(75.0f, 0.5f, 135.0f); // Low on the ground
    glRotatef(-30.0f, 0.0f, 1.0f, 0.0f); // Angled

    // Bottom plank
    glPushMatrix();
    glScalef(8.0f, 1.5f, 2.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Top plank (slightly offset)
    glPushMatrix();
    glTranslatef(1.0f, 1.0f, 0.0f);
    glScalef(7.0f, 1.5f, 1.5f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix(); // End Planks Transform

    glDisable(GL_TEXTURE_2D);
}

void materials(materialStruct* m)
{
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m->specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m->shininess);
}

void lightingTree(lightingStruct* li)
{
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, li->ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, li->diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, li->specular);
}

const float TRUNK_TILING_FACTOR = 4.0f; // For horizontal wrap repetition

float truncatedCone(float botRadius, float height)
{
    float topRadius = botRadius * 0.75f;
    if (topRadius < 0.1f) topRadius = 0.1f;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, treeTrunkTexture);

    glBegin(GL_QUAD_STRIP);
    int dTheta = 360 / slices;
    for (int theta = 0; theta <= 360; theta += dTheta) {
        float cosT = cos(theta * DtoR);
        float sinT = sin(theta * DtoR);
        float texX = (float)theta / 360.0f * TRUNK_TILING_FACTOR; // Tiling Factor

        glNormal3f(cosT, sinT, 0.0f);

        glTexCoord2f(texX, 0.0f);
        glVertex3f(botRadius * cosT, botRadius * sinT, 0.0f);

        glTexCoord2f(texX, 1.0f);
        glVertex3f(topRadius * cosT, topRadius * sinT, height);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);

    return topRadius;
}

void drawTree(int level, float botRadius, float height, float a, int rX, int rY, float leafSize) {

    glPushMatrix();
    glRotatef(a, (float)rX, (float)rY, 0.0f);

    materials(&Materials);
    glColor3f(0.6f, 0.44f, 0.24f);
    botRadius = truncatedCone(botRadius, height);

    if (level-- > 0) {
        glTranslatef(0.0f, 0.0f, height);
        height *= 0.8f;
        if (botRadius < 0.5f) { // Stop recursion for very thin branches
            level = 0;
        }

        if (level % 2 == 0) {
            drawTree(level, botRadius, height, (float)angle, 0, 1, leafSize);
            drawTree(level, botRadius, height, (float)-angle, 0, 1, leafSize);
        }
        else {
            drawTree(level, botRadius, height, (float)angle, 1, 0, leafSize);
            drawTree(level, botRadius, height, (float)-angle, 1, 0, leafSize);
        }

    }
    else if (hasLeaf) {
        // Draw leaves as a final cone
        glColor3f(1.0f, 1.0f, 1.0f);
        materials(&LeafMaterial);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, treeLeafTexture);

        glPushMatrix();
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        GLUquadric* quad = gluNewQuadric();
        gluQuadricTexture(quad, GL_TRUE); // Enable GLU texture generation
        gluCylinder(quad, botRadius * leafSize * 0.5f, 0.0f, height * 1.5f, slices, 1);
        gluDeleteQuadric(quad);
        glPopMatrix();

        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();
}

void drawSquare(int size, materialStruct* material) {
    // Helper to draw a quad (used for sun/moon)
    materials(material);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f((float)-size, (float)-size);
    glVertex2f((float)-size, (float)size);
    glVertex2f((float)size, (float)size);
    glVertex2f((float)size, (float)-size);
    glEnd();
}

void renderSun() {

    // Calculate Sun's position based on orbital angle (in the XZ-plane for simplicity, but high Y)
    // The sun is placed relative to the world origin for shading consistency, but far away.
    sunPosition[0] = SUN_ORBIT_RADIUS * cos(sunOrbitalAngle * DtoR);
    sunPosition[1] = SUN_ORBIT_RADIUS * sin(sunOrbitalAngle * DtoR); // Use Y for the main angle
    sunPosition[2] = 0.0f; // Fixed Z

    // Small offset to add gliding effect (circular path in XZ plane)
    sunPosition[0] += 50.0f * cos(sunOrbitalAngle * DtoR * 5.0f);
    sunPosition[2] += 50.0f * sin(sunOrbitalAngle * DtoR * 5.0f);

    // Update GL_LIGHT0's position to track the Sun (Directional shading)
    // W=0 for directional, but Sun is positional in the code
    GLfloat lightPosDynamic[] = { sunPosition[0], sunPosition[1], sunPosition[2], 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosDynamic);

    // Only draw the Sun if it is above the horizon
    if (sunPosition[1] < 0.0f) {
        // Render Moon (simple sphere, less intense)
        glColor3f(0.8f, 0.9f, 1.0f);
        glPushMatrix();
        glTranslatef(-sunPosition[0], -sunPosition[1], -sunPosition[2]); // Opposite side of orbit
        glDisable(GL_LIGHTING);
        glutSolidSphere(SUN_GLOW_RADIUS * 0.5f, 16, 16);
        glEnable(GL_LIGHTING);
        glPopMatrix();
        return;
    }

    // --- Render Sun (Self-Luminous Sphere) ---

    // 1. Temporarily enable emission and draw the sphere
    glPushAttrib(GL_LIGHTING_BIT);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, sun_emissive); // Sun glows

    // Optional: Flickering effect on the glow color
    float flicker = 1.0f + cos(waveTimer * 5.0f) * 0.1f;
    glColor3f(1.0f, 1.0f * flicker, 0.8f * flicker);

    glPushMatrix();
    glTranslatef(sunPosition[0], sunPosition[1], sunPosition[2]);
    glDisable(GL_LIGHTING); // Sphere is self-luminous
    glutSolidSphere(SUN_GLOW_RADIUS, 32, 32);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    // 2. Restore state by removing emission 
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, sun_zero_material);
    glPopAttrib();
}

void renderForest() {
    glDisable(GL_TEXTURE_2D);
    glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT); // Save lighting and enable states
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    // Set up the tree-specific light position (directional, from the "sun/moon")
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosTree);

    // Set the overall ambient light of the scene based on sunRise
    GLfloat a = (sunRise + 1.0f) / 2.0f; // Map -1 to 1 to 0 to 1
    GLfloat overallAmbient = 0.05f + 0.5f * a; // Ambient light changes from dark to bright
    GLfloat globalAmbient[] = { overallAmbient, overallAmbient, overallAmbient, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);





    // Draw random trees
    srand(100);
    for (int i = 0; i < NUM_TREES; i++) {
        glPushMatrix();

        // Random Position (Centered around 100, 100 near the terrain/house area)
        float randX = 100.0f + ((float)rand() / RAND_MAX * (2.0f * FOREST_RANGE)) - FOREST_RANGE;
        float randZ = 100.0f + ((float)rand() / RAND_MAX * (2.0f * FOREST_RANGE)) - FOREST_RANGE;

        // Random Parameters
        float rHeight = root_height * (0.8f + (float)(rand() % 40) / 100.0f);
        float rRatio = sizeRatio * (0.8f + (float)(rand() % 40) / 100.0f);
        float rRadius = rHeight / rRatio;
        float rYaw = (float)(rand() % 360);
        float rLeafSize = 8.0f * (0.5f + (float)(rand() % 100) / 100.0f);

        glTranslatef(randX, 0.0f, randZ);
        glRotatef(rYaw, 0.0f, 1.0f, 0.0f);

        // Rotate the model so the Z-axis-based tree stands on the Y-axis
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, -rRadius); // Correct initial Z translation to plant the base

        drawTree(treeLevel, rRadius, rHeight, 0, 0, 0, rLeafSize);

        glPopMatrix();
    }

    glPopAttrib();
    glEnable(GL_TEXTURE_2D);
}

void drawTexturedCrate(float size) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, boxTexture);
    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);

    // Front face (+Z)
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size / 2, -size / 2, size / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size / 2, -size / 2, size / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size / 2, size / 2, size / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size / 2, size / 2, size / 2);

    // Back face (-Z)
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-size / 2, -size / 2, -size / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-size / 2, size / 2, -size / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(size / 2, size / 2, -size / 2);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(size / 2, -size / 2, -size / 2);

    // Top face (+Y)
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size / 2, size / 2, -size / 2);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size / 2, size / 2, size / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size / 2, size / 2, size / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size / 2, size / 2, -size / 2);

    // Bottom face (-Y)
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-size / 2, -size / 2, -size / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(size / 2, -size / 2, -size / 2);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(size / 2, -size / 2, size / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-size / 2, -size / 2, size / 2);

    // Right face (+X)
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(size / 2, -size / 2, -size / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(size / 2, size / 2, -size / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(size / 2, size / 2, size / 2);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(size / 2, -size / 2, size / 2);

    // Left face (-X)
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-size / 2, -size / 2, -size / 2);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-size / 2, -size / 2, size / 2);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-size / 2, size / 2, size / 2);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-size / 2, size / 2, -size / 2);

    glEnd();
    glDisable(GL_TEXTURE_2D);
}

void renderCrates() {
    float size = 5.0f;

    // --- 1. Random Crates ---
    for (int i = 0; i < NUM_RANDOM_CRATES; i++) {
        glPushMatrix();
        glTranslatef(cratePositions[i][0], cratePositions[i][1] + size / 2.0f, cratePositions[i][2]);
        glRotatef(static_cast<float>(rand() % 360), 0.0f, 1.0f, 0.0f); // Random rotation
        drawTexturedCrate(size);
        glPopMatrix();
    }

    // --- 2. Crates near House 1 (renderHouse: 300, 100) ---
    float h1X = 300.0f; float h1Z = 90.0f;

    glPushMatrix(); // Crate Stack 1
    glTranslatef(h1X + 25.0f, size / 2.0f, h1Z - 20.0f);
    drawTexturedCrate(size);
    glTranslatef(0.0f, size - 0.8f, 0.0f); drawTexturedCrate(size * 0.8f);
    glPopMatrix();

    glPushMatrix(); // Single Crate 2
    glTranslatef(h1X + 20.0f, size / 2.0f, h1Z - 10.0f);
    glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
    drawTexturedCrate(size * 1.2f);
    glPopMatrix();

    // --- 3. Crates near House 2 (renderVerticalExtendedHouse: 100, 100) ---
    float h2X = 100.0f; float h2Z = 60.0f;

    glPushMatrix(); // Crate Stack 3
    glTranslatef(h2X + 30.0f, size / 2.0f, h2Z + 30.0f);
    drawTexturedCrate(size);
    glTranslatef(0.0f, size, 0.0f); drawTexturedCrate(size);
    glTranslatef(0.0f, size, 0.0f); drawTexturedCrate(size);
    glPopMatrix();

    // --- 4. Crates near House 3 (renderComplexHouse: 250, 150) ---
    float h3X = 250.0f; float h3Z = 150.0f;

    glPushMatrix(); // Crate Stack 4
    glTranslatef(h3X - 30.0f, size / 2.0f, h3Z + 15.0f);
    glRotatef(10.0f, 0.0f, 1.0f, 0.0f);
    drawTexturedCrate(size * 1.5f);
    glPopMatrix();
}

void renderSandParticles() {
    glDisable(GL_LIGHTING); // Ensure particles don't block ambient light

    // Set up Glass Material for transparency and specularity
    glPushAttrib(GL_LIGHTING_BIT | GL_ENABLE_BIT);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glass_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glass_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glass_specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, glass_shininess);

    glEnable(GL_LIGHTING); // Need lighting for the glass material to look reflective/glossy

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set particle color (faded white/clear) and high transparency
    glColor4f(1.0f, 1.0f, 1.0f, 0.3f); // Highly transparent white

    for (int i = 0; i < NUM_SAND_PARTICLES; i++) {
        glPushMatrix();
        // Update position based on animation
        float x = sandParticles[i][0] + fmod(waveTimer * PARTICLE_SPEED, 3000.0f);
        float y = sandParticles[i][1];
        float z = sandParticles[i][2];

        glTranslatef(x, y, z);

        // Draw a small sphere
        glutSolidSphere(PARTICLE_RADIUS, 4, 4); // Low resolution for performance

        glPopMatrix();
    }

    glPopAttrib(); // Restore original lighting/enable state
    glDisable(GL_BLEND);
}

void renderDustStorms() {
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, sandTexture); // Reuse sand texture

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Dust color (very light, very transparent)
    glColor4f(0.8f, 0.8f, 0.8f, 0.15f); // High transparency, near white/grey

    for (int i = 0; i < NUM_DUST_CLOUDS; i++) {
        glPushMatrix();

        // Update position based on animation
        float x = dustClouds[i][0] + fmod(waveTimer * PARTICLE_SPEED * 0.5f, 5000.0f);
        float y = dustClouds[i][1];
        float z = dustClouds[i][2];

        glTranslatef(x, y, z);

        // Billboarding: Rotate around Y to face the camera
        float dx = camX - x;
        float dz = camZ - z;
        float angleY = atan2(dx, dz) * 180.0f / M_PI;
        glRotatef(angleY, 0.0f, 1.0f, 0.0f);

        // --- NEW: Irregular/Dynamic Transformations for Cloud Shape ---
        // Subtle rotation around the Z-axis (local axis)
        glRotatef(waveTimer * 10.0f + (float)i * 15.0f, 0.0f, 0.0f, 1.0f);

        // Subtle non-uniform scaling (simulating puffing)
        float scaleFactor = 1.0f + sin(waveTimer * 0.5f + (float)i) * 0.1f;
        glScalef(scaleFactor, 1.0f / scaleFactor, 1.0f);
        // --- END NEW TRANSFORMATIONS ---

        // Draw the large textured quad
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-DUST_CLOUD_SIZE / 2, -DUST_CLOUD_H / 2, 0);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(DUST_CLOUD_SIZE / 2, -DUST_CLOUD_H / 2, 0);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(DUST_CLOUD_SIZE / 2, DUST_CLOUD_H / 2, 0);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-DUST_CLOUD_SIZE / 2, DUST_CLOUD_H / 2, 0);
        glEnd();

        glPopMatrix();
    }

    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
}

void drawIrregularFlame(float height, float baseRadius, float flickerTime, bool smallFlame) {
    int segments = 16;
    int rings = 5;

    // Scale the base radius slightly to match the flicker in height
    float baseR = baseRadius * (1.0f + sin(flickerTime * 2.5f) * 0.05f);

    // Set material for self-luminous glow
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, fire_emissive);

    // Dynamic color based on flicker (Yellow core, Orange outer)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int j = 0; j < rings; ++j) {
        glBegin(GL_QUAD_STRIP);
        float y1 = (float)j / rings * height;
        float y2 = (float)(j + 1) / rings * height;

        float r1 = baseR * (1.0f - y1 / height); // Tapering radius
        float r2 = baseR * (1.0f - y2 / height);

        // Turbulence/Irregularity factor based on height and time
        float turbulence_amp = 0.2f + 0.5f * y1 / height;

        // Animate color: Orange -> Yellow -> White
        float color_blend = (float)j / rings; // 0.0 at base, 1.0 at top

        // Outer Orange/Red
        glColor4f(1.0f, 0.5f - color_blend * 0.5f, 0.0f, 0.8f * (1.0f - color_blend));

        for (int i = 0; i <= segments; ++i) {
            float angle = (float)i / segments * 2.0f * M_PI;

            // Perlin/Sinusoidal noise for irregular shape
            float noise = sin(angle * 5.0f + flickerTime * 8.0f) * cos(y1 * 2.0f) * turbulence_amp;

            float cosA = cos(angle) + noise;
            float sinA = sin(angle) + noise;

            // Normalize new components (approximately)
            float norm_factor = 1.0f / sqrt(cosA * cosA + sinA * sinA);
            cosA *= norm_factor;
            sinA *= norm_factor;

            // Vertex 1 (Bottom ring)
            glNormal3f(cosA, sinA, 0.0f);
            glVertex3f(r1 * cosA, y1, r1 * sinA);

            // Vertex 2 (Top ring)
            glNormal3f(cosA, sinA, 0.0f);
            glVertex3f(r2 * cosA, y2, r2 * sinA);
        }
        glEnd();
    }

    // Central White/Yellow Tip (Self-Luminous)
    glColor4f(1.0f, 1.0f, 0.8f, 0.9f);
    glPushMatrix();
    glTranslatef(0.0f, height * 0.7f, 0.0f);
    glutSolidSphere(baseR * 0.3f, 8, 8);
    glPopMatrix();

    glDisable(GL_BLEND);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, fire_zero_material);
}

void drawBalloonEnvelope(float radius, float height) {
    int slices = 32;

    // Enable Texturing for the envelope
    glEnable(GL_TEXTURE_2D);
    // Assuming you have a general balloon texture or are reusing skybox/sand for testing
    glBindTexture(GL_TEXTURE_2D, balloonTexture);
    glColor3f(1.0f, 1.0f, 1.0f); // White for full texture color

    // --- 1. Main Envelope ---
    glPushMatrix();
    glTranslatef(0.0f, height * 0.3f, 0.0f); // Center top part

    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluQuadricTexture(quad, GL_TRUE); // Enable texture for the envelope

    // Top Hemisphere
    glutSolidSphere(radius, slices, slices);
    glPopMatrix();

    // Main Body (Tapered Lower Half)
    glPushMatrix();
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, -height * 0.3f);
    gluCylinder(quad, radius, radius * 0.5f, height * 0.6f, slices, slices);
    glPopMatrix();

    // Burner Opening / Neck (Bottom)
    glPushMatrix();
    glTranslatef(0.0f, -height * 0.3f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    gluDisk(quad, radius * 0.4f, radius * 0.5f, slices, 1);
    glPopMatrix();

    gluDeleteQuadric(quad);
    glDisable(GL_TEXTURE_2D); // Disable after envelope
}

void drawBasketAndRopes(float radius, float height, float basketSize) {
    float neckY = -height * 0.3f; // Y-position where ropes connect to the envelope
    float neckR = radius * 0.5f;
    float ropeLength = height * 0.4f; // Vertical drop of the ropes

    // --- 1. Basket ---
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, boxTexture); // Use boxTexture for wicker basket
    glColor3f(1.0f, 1.0f, 1.0f);

    float basketY = neckY - ropeLength; // Y-position of the basket center

    glPushMatrix();
    glTranslatef(0.0f, basketY, 0.0f);
    glScalef(basketSize, basketSize * 0.5f, basketSize);

    // Use glutSolidCube and relies on automatic texcoords for the basket
    glutSolidCube(1.0f);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);

    // --- 2. Ropes (Thin cylinders) ---
    glColor3f(0.3f, 0.3f, 0.3f); // Dark Grey Ropes
    GLUquadric* quadRope = gluNewQuadric();
    float ropeThickness = 0.2f;

    for (int i = 0; i < 4; ++i) {
        glPushMatrix();
        float angle = (float)i * 90.0f * DtoR;
        float ropeOffsetX = neckR * cos(angle);
        float ropeOffsetZ = neckR * sin(angle);

        // --- Move to the connection point on the envelope neck ---
        glTranslatef(ropeOffsetX, neckY, ropeOffsetZ);

        // --- Orient cylinder to connect to the basket ---
        // Calculate the vector from the neck connection point to a corner of the basket
        float targetX = (basketSize / 2.0f) * cos(angle);
        float targetZ = (basketSize / 2.0f) * sin(angle);
        float targetY = basketY + basketSize * 0.5f * 0.5f; // Basket top edge

        float dx = targetX - ropeOffsetX;
        float dy = targetY - neckY;
        float dz = targetZ - ropeOffsetZ;

        float dist = sqrt(dx * dx + dy * dy + dz * dz);
        float pitch = atan2(sqrt(dx * dx + dz * dz), dy) * 180.0f / M_PI;
        float yaw = atan2(dz, dx) * 180.0f / M_PI;

        glRotatef(yaw, 0.0f, 1.0f, 0.0f);
        glRotatef(pitch - 90.0f, 1.0f, 0.0f, 0.0f); // Pitch to connect

        // Draw the rope cylinder
        gluCylinder(quadRope, ropeThickness, ropeThickness * 0.8f, dist, 8, 1);

        glPopMatrix();
    }

    gluDeleteQuadric(quadRope);
}

void renderHotAirBalloon(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    // Envelope
    drawBalloonEnvelope(BALLOON_R, BALLOON_HEIGHT);

    // Basket and Ropes
    drawBasketAndRopes(BALLOON_R, BALLOON_HEIGHT, BASKET_SIZE);

    glEnable(GL_TEXTURE_2D);
    glPopMatrix();
}

void renderBalloons() {
    renderHotAirBalloon(balloon1X, balloon1Y, balloon1Z);
    renderHotAirBalloon(balloon2X, balloon2Y, balloon2Z);
}

void renderCampfireFlame(float x, float y, float z) {
    glDisable(GL_LIGHTING);

    glPushMatrix();
    glTranslatef(x, y, z);

    // Base Logs (Textured)
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, treeTrunkTexture);
    glColor3f(0.3f, 0.2f, 0.1f);

    for (int i = 0; i < 4; ++i) {
        glPushMatrix();
        glRotatef((float)i * 45.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(3.0f, 0.5f, 0.0f);
        glScalef(6.0f, 1.0f, 1.0f);
        glutSolidCube(1.0f);
        glPopMatrix();
    }
    glDisable(GL_TEXTURE_2D);

    // --- Flame Geometry ---
    float flameScale = 1.0f + sin(flickerTime * 3.0f) * 0.05f;
    glPushMatrix();
    glTranslatef(0.0f, 1.0f, 0.0f); // Start flame above logs
    glScalef(flameScale, 1.0f, flameScale);
    drawIrregularFlame(FLAME_SIZE, FLAME_SIZE * 0.4f, flickerTime, false);
    glPopMatrix();

    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void renderPyramidTorches() {
    float startX = 500.0f;
    float startZ = 200.0f;
    float pSpacing = 160.0f;
    float torchH = 40.0f;
    float torchR = 0.5f;

    // Simulate light flickering magnitude and set light intensity
    float flicker = FLICKER_BASE + sin(flickerTime * 10.0f) * FLICKER_MAGNITUDE;
    GLfloat torchDiffuse[] = { flicker * 1.0f, flicker * 0.6f, flicker * 0.0f, 1.0f };
    glLightfv(GL_LIGHT4, GL_DIFFUSE, torchDiffuse);

    for (int i = 0; i < 3; ++i) {
        float pX = startX + (float)i * pSpacing;
        float pZ = startZ;

        float offsets[4][2] = {
            {-PYRAMID_TORCH_OFFSET, -PYRAMID_TORCH_OFFSET},
            { PYRAMID_TORCH_OFFSET, -PYRAMID_TORCH_OFFSET},
            { PYRAMID_TORCH_OFFSET,  PYRAMID_TORCH_OFFSET},
            {-PYRAMID_TORCH_OFFSET,  PYRAMID_TORCH_OFFSET}
        };

        for (int j = 0; j < 4; ++j) {
            float tX = pX + offsets[j][0];
            float tZ = pZ + offsets[j][1];

            glPushMatrix();
            glTranslatef(tX, -5.0f, tZ); // Torches on flat ground (Y=0)

            // --- 1. Torch Stick (Wood/Dark Color) ---
            glDisable(GL_TEXTURE_2D);
            glColor3f(0.3f, 0.2f, 0.1f);
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, 0.0f);
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
            GLUquadric* quadTorch = gluNewQuadric();
            gluCylinder(quadTorch, torchR, torchR, torchH, 8, 1);
            gluDeleteQuadric(quadTorch);
            glPopMatrix();

            // --- 2. Torch Head (Cone Cup) ---
            glColor3f(0.5f, 0.5f, 0.5f); // Metal/Grey
            glPushMatrix();
            glTranslatef(0.0f, torchH, 0.0f);
            glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Point cone mouth up
            GLUquadric* quadHead = gluNewQuadric();
            gluCylinder(quadHead, torchR * 3.0f, torchR * 0.5f, 3.0f, 8, 1);
            gluDeleteQuadric(quadHead);
            glPopMatrix();

            // --- 3. Fire Flame on Top ---
            glDisable(GL_LIGHTING);
            glPushMatrix();
            glTranslatef(0.0f, torchH, 0.0f); // Above the cup
            drawIrregularFlame(FLAME_SIZE * 0.3f, FLAME_SIZE * 0.1f, flickerTime + (float)j, true); // Smaller flames
            glEnable(GL_LIGHTING);
            glPopMatrix();


            // --- 4. Light Source (GL_LIGHT4 Repositioning) ---
            glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, 0.005f * flicker);
            GLfloat lightPos[] = { tX, torchH + 3.0f, tZ, 1.0f };
            glLightfv(GL_LIGHT4, GL_POSITION, lightPos);
            glEnable(GL_LIGHT4);

            glPopMatrix();
        }
        glDisable(GL_LIGHT4);
    }
}

void renderPyramids() {
    float pBase = 300.0f; // Base side length
    float pHeight = 240.0f;
    float pSpacing = 320.0f; // Spacing between pyramids
    float startX = 550.0f;
    float startZ = 450.0f;
    float baseY = 0.0f; // Sits on the flat ground

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, pyramidTexture);
    glColor3f(1.0f, 1.0f, 1.0f); // White to let texture colors through

    for (int i = 0; i < 3; ++i) {
        glPushMatrix();
        // Translate to position for Pyramid i
        glTranslatef(startX + (float)i * pSpacing, baseY, startZ);

        // Define vertices (relative to the pyramid's origin)
        GLfloat apex[] = { 0.0f, pHeight, 0.0f };
        GLfloat v1[] = { -pBase / 2.0f, 0.0f, -pBase / 2.0f };
        GLfloat v2[] = { pBase / 2.0f, 0.0f, -pBase / 2.0f };
        GLfloat v3[] = { pBase / 2.0f, 0.0f, pBase / 2.0f };
        GLfloat v4[] = { -pBase / 2.0f, 0.0f, pBase / 2.0f };

        // --- 1. Base (Square Quad) ---
        glBegin(GL_QUADS);
        glNormal3f(0.0f, -1.0f, 0.0f); // Normal pointing down
        glTexCoord2f(0.0f, 0.0f); glVertex3fv(v4);
        glTexCoord2f(1.0f, 0.0f); glVertex3fv(v3);
        glTexCoord2f(1.0f, 1.0f); glVertex3fv(v2);
        glTexCoord2f(0.0f, 1.0f); glVertex3fv(v1);
        glEnd();

        // --- 2. Side Faces (Triangles) ---
        glBegin(GL_TRIANGLES);

        // Helper to calculate normal for a triangle
        auto calculateFaceNormal = [](GLfloat pA[], GLfloat pB[], GLfloat pC[]) {
            GLfloat edge1[] = { pB[0] - pA[0], pB[1] - pA[1], pB[2] - pA[2] };
            GLfloat edge2[] = { pC[0] - pA[0], pC[1] - pA[1], pC[2] - pA[2] };
            GLfloat normal[] = {
                edge1[1] * edge2[2] - edge1[2] * edge2[1],
                edge1[2] * edge2[0] - edge1[0] * edge2[2],
                edge1[0] * edge2[1] - edge1[1] * edge2[0]
            };
            float len = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
            if (len > 0.0f) glNormal3f(normal[0] / len, normal[1] / len, normal[2] / len);
            };

        // Face 1: Front (-Z)
        calculateFaceNormal(v1, v2, apex);
        glTexCoord2f(0.0f, 0.0f); glVertex3fv(v1);
        glTexCoord2f(1.0f, 0.0f); glVertex3fv(v2);
        glTexCoord2f(0.5f, 1.0f); glVertex3fv(apex);

        // Face 2: Right (+X)
        calculateFaceNormal(v2, v3, apex);
        glTexCoord2f(0.0f, 0.0f); glVertex3fv(v2);
        glTexCoord2f(1.0f, 0.0f); glVertex3fv(v3);
        glTexCoord2f(0.5f, 1.0f); glVertex3fv(apex);

        // Face 3: Back (+Z)
        calculateFaceNormal(v3, v4, apex);
        glTexCoord2f(0.0f, 0.0f); glVertex3fv(v3);
        glTexCoord2f(1.0f, 0.0f); glVertex3fv(v4);
        glTexCoord2f(0.5f, 1.0f); glVertex3fv(apex);

        // Face 4: Left (-X)
        calculateFaceNormal(v4, v1, apex);
        glTexCoord2f(0.0f, 0.0f); glVertex3fv(v4);
        glTexCoord2f(1.0f, 0.0f); glVertex3fv(v1);
        glTexCoord2f(0.5f, 1.0f); glVertex3fv(apex);

        glEnd();

        glPopMatrix();
    }

    glDisable(GL_TEXTURE_2D);
}

void renderSky() {
    // sunRise is the global variable ranging from -1 (night) to 1 (day)
    float a = (sunRise + 1.0f) / 2.0f; // Map -1 to 1 to 0 to 1

    // Dynamic sky color: Ranges from dark blue/black to light blue/white
    float r = 0.6f * a + 0.1f * (1.0f - a);
    float g = 0.8f * a + 0.1f * (1.0f - a);
    float b = 1.0f * a + 0.2f * (1.0f - a);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, skyboxTexture);

    // APPLY DYNAMIC COLOR to the dome vertices
    glColor3f(r, g, b);

    glDepthMask(GL_FALSE);

    glPushMatrix();
    glTranslatef(camX, camY, camZ); // Center on camera

    float radius = 800.0f;

    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GLU_SMOOTH);

    glScalef(-1.0f, 1.0f, 1.0f); // Invert normals for inside viewing

    // Draw the sphere
    gluSphere(quad, radius, 64, 64);

    gluDeleteQuadric(quad);

    glPopMatrix();

    // Restore settings
    glDepthMask(GL_TRUE);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}