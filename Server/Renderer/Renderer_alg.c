#include "Renderer_alg.h"

#define MAX_SHAPES 10

// Angles for rotation
GLfloat cubeAngle = 0.0f;
GLfloat pyramidAngle = 0.0f;
GLfloat cylinderAngle = 0.0f;
GLfloat sphereAngle = 0.0f;
GLfloat coneAngle = 0.0f;
GLfloat prismAngle = 0.0f;

typedef struct {
    char shapeType[10];  // e.g., "sphere", "cube", "cylinder", "cone", "pyramid", "prism"
    GLfloat param1;      // e.g., radius for sphere or side length for cube
    GLfloat param2;      // e.g., height for cylinder or cone
    GLint slices;        // Number of slices for cylinder, cone, or sphere
    GLint stacks;        // Number of stacks for sphere
    GLint n;             // Number of sides for prism

    GLfloat position[3]; // x, y, z position
    GLfloat rotation[3]; // rotation angles (x, y, z)
} Shape;



Shape shapes[MAX_SHAPES];
int shapeCount = 0;

void parseInput(const char* input) {
    const char* ptr = input;

    while (ptr != NULL) {
        char shapeType[20];
        GLfloat param1 = 0, param2 = 0;
        GLint n = 0;
        char paramBuffer[100];

        // Skip any leading spaces
        while (*ptr == ' ' || *ptr == '\n') ptr++;

        // Extract the shape name (e.g., sphere, cube, etc.)
        if (sscanf(ptr, "%s", shapeType) != 1) break;  // Stop if no more shapes

        // Skip the shape name and move to the parameters
        ptr = strchr(ptr, ' ') + 1;

        // Handle Sphere
        if (strcmp(shapeType, "sphere") == 0) {
            // Extract -radius value
            if (sscanf(ptr, "-radius=%f", &param1) == 1) {
                strcpy(shapes[shapeCount].shapeType, "sphere");
                shapes[shapeCount].param1 = param1;
                shapes[shapeCount].slices = 30;
                shapes[shapeCount].stacks = 30;
                shapes[shapeCount].position[0] = 0.0f;
                shapes[shapeCount].position[1] = 1.5f;
                shapes[shapeCount].position[2] = -8.0f;
                shapeCount++;
            }
        }

        // Handle Cube
        else if (strcmp(shapeType, "cube") == 0) {
            // Extract -side value
            if (sscanf(ptr, "-side=%f", &param1) == 1) {
                strcpy(shapes[shapeCount].shapeType, "cube");
                shapes[shapeCount].param1 = param1;
                shapes[shapeCount].position[0] = -2.5f;
                shapes[shapeCount].position[1] = 0.0f;
                shapes[shapeCount].position[2] = -8.0f;
                shapeCount++;
            }
        }
        // Handle Cylinder
        else if (strcmp(shapeType, "cylinder") == 0) {
            // Extract -radius and -height values
            GLfloat radius = 0, height = 0;
            if (sscanf(ptr, "-radius=%f -height=%f", &radius, &height) == 2) {
                strcpy(shapes[shapeCount].shapeType, "cylinder");
                shapes[shapeCount].param1 = radius;
                shapes[shapeCount].param2 = height;
                shapes[shapeCount].slices = 30;
                shapes[shapeCount].position[0] = 2.5f;
                shapes[shapeCount].position[1] = 0.0f;
                shapes[shapeCount].position[2] = -8.0f;
                shapeCount++;
            }
        }
        // Handle Cone
        else if (strcmp(shapeType, "cone") == 0) {
            // Extract -radius and -height values
            GLfloat radius = 0, height = 0;
            if (sscanf(ptr, "-radius=%f -height=%f", &radius, &height) == 2) {
                strcpy(shapes[shapeCount].shapeType, "cone");
                shapes[shapeCount].param1 = radius;
                shapes[shapeCount].param2 = height;
                shapes[shapeCount].slices = 30;
                shapes[shapeCount].position[0] = 0.0f;
                shapes[shapeCount].position[1] = -2.5f;
                shapes[shapeCount].position[2] = -8.0f;
                shapeCount++;
            }
        }
        // Handle Prism
        else if (strcmp(shapeType, "prism") == 0) {
            GLfloat radius = 0, length = 0;
            int sides = 0;
            if (sscanf(ptr, "-radius=%f -length=%f -sides=%d", &radius, &length, &sides) == 3) {
                strcpy(shapes[shapeCount].shapeType, "prism");
                shapes[shapeCount].param1 = radius;
                shapes[shapeCount].param2 = length;
                shapes[shapeCount].n = sides;
                shapes[shapeCount].position[0] = 0.0f;
                shapes[shapeCount].position[1] = 0.0f;
                shapes[shapeCount].position[2] = -8.0f;
                shapeCount++;
            }
        }
        // Handle Pyramid
        else if (strcmp(shapeType, "pyramid") == 0) {
            GLfloat height = 0;
            if (sscanf(ptr, "-height=%f", &height) == 1) {
                // Since the pyramid only has height, we'll assume the base is 1
                strcpy(shapes[shapeCount].shapeType, "pyramid");
                shapes[shapeCount].param1 = height;
                shapes[shapeCount].position[0] = 3.0f;
                shapes[shapeCount].position[1] = 1.0f;
                shapes[shapeCount].position[2] = -8.0f;
                shapeCount++;
            }
        }
        // Handle any unrecognized shape (error handling)
        else {
            printf("Error: Unrecognized shape '%s'. Skipping.\n", shapeType);
        }

        // Move to the next shape in the input string
        ptr = strchr(ptr, '&');
        if (ptr != NULL) {
            ptr++; // Skip '&'
        } else {
            break; // End of input
        }
    }
}



void drawSphere(GLfloat radius, GLint slices, GLint stacks) {
    GLUquadric *quad = gluNewQuadric();

    for (int stack = 0; stack <= stacks; ++stack) {
        GLfloat colorValue = (GLfloat)stack / stacks;
        glColor3f(colorValue, 1.0f - colorValue, 0.5f);

        glBegin(GL_QUAD_STRIP);
        for (int slice = 0; slice <= slices; ++slice) {
            GLfloat theta = 2.0f * M_PI * slice / slices;
            GLfloat phi1 = M_PI * (stack) / stacks;
            GLfloat phi2 = M_PI * (stack + 1) / stacks;

            GLfloat x1 = radius * sinf(phi1) * cosf(theta);
            GLfloat y1 = radius * cosf(phi1);
            GLfloat z1 = radius * sinf(phi1) * sinf(theta);
            glVertex3f(x1, y1, z1);

            GLfloat x2 = radius * sinf(phi2) * cosf(theta);
            GLfloat y2 = radius * cosf(phi2);
            GLfloat z2 = radius * sinf(phi2) * sinf(theta);
            glVertex3f(x2, y2, z2);
        }
        glEnd();
    }

    gluDeleteQuadric(quad);
}

// Function to draw a cone with a gradient color
void drawCone(GLfloat radius, GLfloat height, GLint slices) {
    GLfloat halfHeight = height / 2.0f;

    // Draw the base circle
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 0.5f, 0.0f);  // Color at the center of the base
    glVertex3f(0.0f, -halfHeight, 0.0f);  // Center of the base

    for (int i = 0; i <= slices; ++i) {
        GLfloat angle = 2.0f * M_PI * i / slices;
        GLfloat x = radius * cos(angle);
        GLfloat z = radius * sin(angle);
        glColor3f(0.5f, 0.0f, 1.0f);  // Color around the edge of the base
        glVertex3f(x, -halfHeight, z);
    }
    glEnd();

    // Draw the cone's side
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= slices; ++i) {
        GLfloat angle = 2.0f * M_PI * i / slices;
        GLfloat x = radius * cos(angle);
        GLfloat z = radius * sin(angle);

        glColor3f(1.0f, 0.5f, 0.0f);  // Color at the tip
        glVertex3f(0.0f, halfHeight, 0.0f);  // Tip of the cone
        glColor3f(0.5f, 0.0f, 1.0f);  // Color along the base edge
        glVertex3f(x, -halfHeight, z);  // Base edge
    }
    glEnd();
}



// Function to draw a cube
void drawCube(GLfloat sideLength) {
    glBegin(GL_QUADS);

    // Front face
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex3f(-sideLength/2, -sideLength/2,  sideLength/2);
    glVertex3f( sideLength/2, -sideLength/2,  sideLength/2);
    glVertex3f( sideLength/2,  sideLength/2,  sideLength/2);
    glVertex3f(-sideLength/2,  sideLength/2,  sideLength/2);

    // Back face
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex3f(-sideLength/2, -sideLength/2, -sideLength/2);
    glVertex3f(-sideLength/2,  sideLength/2, -sideLength/2);
    glVertex3f( sideLength/2,  sideLength/2, -sideLength/2);
    glVertex3f( sideLength/2, -sideLength/2, -sideLength/2);

    // Left face
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glVertex3f(-sideLength/2, -sideLength/2, -sideLength/2);
    glVertex3f(-sideLength/2, -sideLength/2,  sideLength/2);
    glVertex3f(-sideLength/2,  sideLength/2,  sideLength/2);
    glVertex3f(-sideLength/2,  sideLength/2, -sideLength/2);

    // Right face
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
    glVertex3f(sideLength/2, -sideLength/2, -sideLength/2);
    glVertex3f(sideLength/2,  sideLength/2, -sideLength/2);
    glVertex3f(sideLength/2,  sideLength/2,  sideLength/2);
    glVertex3f(sideLength/2, -sideLength/2,  sideLength/2);

    // Top face
    glColor3f(1.0f, 0.0f, 1.0f); // Magenta
    glVertex3f(-sideLength/2, sideLength/2, -sideLength/2);
    glVertex3f(-sideLength/2, sideLength/2,  sideLength/2);
    glVertex3f( sideLength/2, sideLength/2,  sideLength/2);
    glVertex3f( sideLength/2, sideLength/2, -sideLength/2);

    // Bottom face
    glColor3f(0.0f, 1.0f, 1.0f); // Cyan
    glVertex3f(-sideLength/2, -sideLength/2, -sideLength/2);
    glVertex3f( sideLength/2, -sideLength/2, -sideLength/2);
    glVertex3f( sideLength/2, -sideLength/2,  sideLength/2);
    glVertex3f(-sideLength/2, -sideLength/2,  sideLength/2);

    glEnd();
}

// Function to draw a pyramid
void drawPyramid(GLfloat height) {
    glBegin(GL_TRIANGLES);

    // Front face
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex3f(0.0f, height, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Right face
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex3f(0.0f, height, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Back face
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glVertex3f(0.0f, height, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    // Left face
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
    glVertex3f(0.0f, height, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();
}

// Function to draw a cylinder with gradient color
void drawCylinder(GLfloat radius, GLfloat height, GLint slices) {
    GLfloat angle;
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= slices; i++) {
        angle = i * 2.0f * M_PI / slices;
        GLfloat x = radius * cos(angle);
        GLfloat y = radius * sin(angle);

        glColor3f((float)i / slices, 0.0f, 1.0f - (float)i / slices);
        glVertex3f(x, y, height / 2);
        glVertex3f(x, y, -height / 2);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i < slices; i++) {
        angle = i * 2.0f * M_PI / slices;
        GLfloat x = radius * cos(angle);
        GLfloat y = radius * sin(angle);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(x, y, height / 2);
    }
    glEnd();

    glBegin(GL_POLYGON);
    for (int i = 0; i < slices; i++) {
        angle = i * 2.0f * M_PI / slices;
        GLfloat x = radius * cos(angle);
        GLfloat y = radius * sin(angle);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(x, y, -height / 2);
    }
    glEnd();
}

void drawPrism(float radius, float length, int n) {
    // Angle increment for each side of the cylinder
    float angleIncrement = 2 * M_PI / n;

    // Draw the cylindrical side (side surface)
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= n; i++) {
        float angle = i * angleIncrement;
        float x = radius * cos(angle);
        float y = radius * sin(angle);

        // Draw two vertices at the top and bottom
        glVertex3f(x, y, 0.0f);             // Bottom
        glVertex3f(x, y, length);           // Top
    }
    glEnd();

    // Draw the bottom face
    glBegin(GL_POLYGON);
    for (int i = 0; i < n; i++) {
        float angle = i * angleIncrement;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();

    // Draw the top face
    glBegin(GL_POLYGON);
    for (int i = 0; i < n; i++) {
        float angle = i * angleIncrement;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex3f(x, y, length);
    }
    glEnd();
}



/**
// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Render the cube
    glTranslatef(-2.5f, 0.0f, -8.0f);
    glRotatef(cubeAngle, 1.0f, 1.0f, 1.0f);
    drawCube(2.0f);

    // Render the pyramid
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -8.0f);
    glRotatef(pyramidAngle, 1.0f, 0.0f, 0.0f);
    drawPyramid(1.5f);

    // Render the cylinder
    glLoadIdentity();
    glTranslatef(2.5f, 0.0f, -8.0f);
    glRotatef(cylinderAngle, 0.0f, 1.0f, 0.0f);
    drawCylinder(0.5f, 2.0f, 20);

    //Render sphere
    glLoadIdentity();
    glTranslatef(0.0f, 2.5f, -8.0f);  // Adjusted position for the sphere
    glRotatef(sphereAngle, 0.0f, 1.0f, 0.0f);  // Rotate the sphere around Y-axis
    drawSphere(1.5f, 30, 30);

    //Render cone
    glLoadIdentity();
    glTranslatef(0.0f, -2.5f, -8.0f);  // Adjusted position for the cone
    glRotatef(coneAngle, 1.0f, 0.0f, 0.0f);  // Rotate the cone around X-axis
    drawCone(1.0f, 2.0f, 50);

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -8.0f);
    glRotatef(prismAngle, 0.0f, 1.0f, 0.0f);
    drawPrism(1.0f, 2.0f, 9);  // Draw a cylinder with radius 1.0, length 2.0, and 20 sides

    glutSwapBuffers();
}
*/

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < shapeCount; i++) {
        Shape currentShape = shapes[i];
        //printf(&shapes[i]);

        // Set up transformations for each shape
        glLoadIdentity();
        glTranslatef(currentShape.position[0], currentShape.position[1], currentShape.position[2]);
        glRotatef(currentShape.rotation[0], 1.0f, 0.0f, 0.0f);
        glRotatef(currentShape.rotation[1], 0.0f, 1.0f, 0.0f);
        glRotatef(currentShape.rotation[2], 0.0f, 0.0f, 1.0f);

        // Determine which shape to draw based on shapeType
        if (strcmp(currentShape.shapeType, "sphere") == 0) {
            drawSphere(currentShape.param1, currentShape.slices, currentShape.stacks);
        }
        else if (strcmp(currentShape.shapeType, "cube") == 0) {
            drawCube(currentShape.param1);
        }
        else if (strcmp(currentShape.shapeType, "cylinder") == 0) {
            drawCylinder(currentShape.param1, currentShape.param2, currentShape.slices);
        }
        else if (strcmp(currentShape.shapeType, "cone") == 0) {
            drawCone(currentShape.param1, currentShape.param2, currentShape.slices);
        }
        else if (strcmp(currentShape.shapeType, "pyramid") == 0) {
            drawPyramid(currentShape.param1);
        }
        else if (strcmp(currentShape.shapeType, "prism") == 0) {
            drawPrism(currentShape.param1, currentShape.param2, currentShape.n);
        }
    }

    glutSwapBuffers();
}




/**
// Timer function to update angles
void timer(int value) {
    cubeAngle += 2.0f;
    if (cubeAngle > 360) cubeAngle -= 360;

    pyramidAngle += 3.0f;
    if (pyramidAngle > 360) pyramidAngle -= 360;

    cylinderAngle += 1.0f;
    if (cylinderAngle > 360) cylinderAngle -= 360;

    sphereAngle += 1.0f;  // Increment the angle to rotate the sphere
    if (sphereAngle > 360.0f) sphereAngle -= 360.0f;  // Keep angle within 0-360

    coneAngle += 1.0f;
    if (coneAngle > 360.0f) coneAngle -= 360.0f;

    prismAngle += 1.0f;
    if (prismAngle > 360.0f) prismAngle -= 360.0f;



    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}
*/

void timer(int value) {
    for (int i = 0; i < shapeCount; i++) {
        shapes[i].rotation[0] += 1.0f;  // Increment X rotation
        if (shapes[i].rotation[0] > 360) shapes[i].rotation[0] -= 360;

        shapes[i].rotation[1] += 2.0f;  // Increment Y rotation
        if (shapes[i].rotation[1] > 360) shapes[i].rotation[1] -= 360;

        shapes[i].rotation[2] += 3.0f;  // Increment Z rotation
        if (shapes[i].rotation[2] > 360) shapes[i].rotation[2] -= 360;
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}


// Reshape function to handle window resizing
void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspect, 1.0f, 20.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}