#include "Global.h"
#include "Init.h"
#include "Render.h"



// --- Helper Logic ---
void updateCameraLook() {
    float yawRad = yaw * M_PI / 180.0f;
    float pitchRad = pitch * M_PI / 180.0f;
    float hDist = LOOK_DISTANCE * cos(pitchRad);
    lookY = camY + LOOK_DISTANCE * sin(pitchRad);
    lookX = camX + hDist * cos(yawRad);
    lookZ = camZ + hDist * sin(yawRad);
}

// --- Input Handling ---
void keyboard(unsigned char key, int x, int y) {
    float moveSpeed = 1.0f;
    float yawRad = yaw * M_PI / 180.0f;

    if (key == 'w' || key == 'W') { camX += cos(yawRad) * moveSpeed; camZ += sin(yawRad) * moveSpeed; }
    if (key == 's' || key == 'S') { camX -= cos(yawRad) * moveSpeed; camZ -= sin(yawRad) * moveSpeed; }
    if (key == 'a' || key == 'A') { camX += sin(yawRad) * moveSpeed; camZ -= cos(yawRad) * moveSpeed; }
    if (key == 'd' || key == 'D') { camX -= sin(yawRad) * moveSpeed; camZ += cos(yawRad) * moveSpeed; }
    if (key == 'e' || key == 'E') camY += moveSpeed;
    if (key == 'q' || key == 'Q') camY -= moveSpeed;
    if (key == 't' || key == 'T') {
        yaw = 225.0f; pitch = 0.0f;
        camX = 150.0f; camY = 5.0f; camZ = 150.0f;
    }
    if (key == 'g' || key == 'G') {
        houseLightOn = !houseLightOn; 
        std::cout << "House Light Toggled: " << (houseLightOn ? "ON" : "OFF") << std::endl;
    }

    if (key == 'o' || key == 'O') {
        isDoorOpen = !isDoorOpen;
    }
    updateCameraLook();
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    float rotationSpeed = 2.0f;
    switch (key) {
    case GLUT_KEY_LEFT: yaw += rotationSpeed; break;
    case GLUT_KEY_RIGHT: yaw -= rotationSpeed; break;
    case GLUT_KEY_UP: pitch += rotationSpeed; if (pitch > 89.0f) pitch = 89.0f; break;
    case GLUT_KEY_DOWN: pitch -= rotationSpeed; if (pitch < -89.0f) pitch = -89.0f; break;
    }
    updateCameraLook();
    glutPostRedisplay();
}

void reshape(int w, int h) {
    windowWidth = w; windowHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

// --- Main Loop Functions ---
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(camX, camY, camZ, lookX, lookY, lookZ, 0, 1, 0);

    // Light Position Update
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Day/Night Background
    float a = (sunRise + 1.0f) / 2.0f;
    glClearColor(0.05f * a, 0.05f * a, 0.1f * a, 1.0f);

    if (sunRise < -0.1f) {
        glEnable(GL_FOG); // Turn Fog ON

        // Set Fog Color to match the Sky Color (So it looks natural, not grey)
        GLfloat fogColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
        glFogfv(GL_FOG_COLOR, fogColor);

        // Update Density (This applies the animation calculated in idle())
        glFogf(GL_FOG_DENSITY, fogDensity);
    }
    else {
        glDisable(GL_FOG); // Turn Fog OFF during the day
    }
    // Spotlight Logic
    if (sunRise < 0.0f) glEnable(GL_LIGHT2); else glDisable(GL_LIGHT2);

    // --- RENDER CALLS (From Render.h) ---
    renderSky();
    renderFlatGround();
    renderTerrain();
    renderBarrels();
    renderHouse();
    renderWoods();
    renderComplexHouse();
    renderVerticalExtendedHouse();
    renderForest();
    renderPyramids();
    renderLake();
    renderSmallLakes();
    renderBridge();
    renderLighthouse();
    renderWindmills();
    renderBalloons();
    renderCrates();
    renderSandParticles();
    renderDustStorms();
    renderSun();
    renderMuseum();
    renderCampfireFlame(CAMPFIRE_X, CAMPFIRE_Y, CAMPFIRE_Z);
    renderPyramidTorches();
    renderDeadTrees();
	renderChairAndTable();

    glutSwapBuffers();
}

void idle() {
    // Animation Logic
    fanAngle += FAN_SPEED; if (fanAngle > 360.0f) fanAngle -= 360.0f;
    spotAngle += 2.0f; if (spotAngle > 360.0f) spotAngle -= 360.0f;

    // Fog Logic
    if (fogIncreasing) {
        fogDensity += 0.00005f;
        if (fogDensity > 0.02f) fogIncreasing = false;
    }
    else {
        fogDensity -= 0.00005f;
        if (fogDensity < 0.005f) fogIncreasing = true;
    }

    // Barrels
    for (int i = 0; i < NUM_ROLLING_BARRELS; i++) {
        barrelRollAngles[i] += barrelRollSpeed;
        if (barrelRollAngles[i] > 360.0f) barrelRollAngles[i] -= 360.0f;
    }

    flickerTime += 0.1f; if (flickerTime > 2.0f * M_PI) flickerTime -= 2.0f * M_PI;

    // Balloon Animation
    balloon1Phase += 0.015f; balloon2Phase += 0.02f;
    balloon1Y += sin(balloon1Phase) * 0.5f; balloon2Y += cos(balloon2Phase) * 0.7f;
    balloon1X += 0.2f; balloon2X -= 0.1f;

    // --- DOOR ANIMATION ---
    float doorSpeed = 4.0f; // Speed of opening/closing
    if (isDoorOpen) {
        if (currentDoorAngle < 90.0f) currentDoorAngle += doorSpeed;
    }
    else {
        if (currentDoorAngle > 0.0f) currentDoorAngle -= doorSpeed;
    }

    waveTimer += 0.05f;
    bladeAngle += BLADE_SPEED; if (bladeAngle > 360.0f) bladeAngle -= 360.0f;

    sunOrbitalAngle += 0.2f; if (sunOrbitalAngle > 360.0f) sunOrbitalAngle -= 360.0f;
    sunRise = sin(sunOrbitalAngle * DtoR);

    // Day/Night Light Intensity
    float flicker = 1.0f + cos(waveTimer * 5.0f) * 0.02f;
    GLfloat newDiffuse[] = { (1.0f * sunRise) * flicker, (0.95f * sunRise) * flicker, (0.8f * sunRise) * flicker, 1.0f };

    if (sunRise > 0.0f) glLightfv(GL_LIGHT0, GL_DIFFUSE, newDiffuse);
    else { GLfloat zero[] = { 0.0f, 0.0f, 0.0f, 1.0f }; glLightfv(GL_LIGHT0, GL_DIFFUSE, zero); }

    glutPostRedisplay();
}

int main(int argc, char** argv) {
    camX = 150.0f; camY = 5.0f; camZ = 150.0f; yaw = 225.0f;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("3D Desert Terrain & House - Modular");

    // --- Init Calls (From Init.h) ---
    generateHeightMap();
    initOpenGL();
    initLighting();
    loadTextures();
    generateCratePositions();
    generateParticlePositions();
    generateWildernessPositions();
    initializeRollingBarrels();
    //setupFog();

    updateCameraLook();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    std::cout << "Modularization Complete.\nControls:\nQ/E: Yaw | R/F: Pitch | T: Reset | G: House Light\n";

    glutMainLoop();
    return 0;
}