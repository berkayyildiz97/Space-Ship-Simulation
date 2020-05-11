#include "Angel.h"
#include <time.h>

using namespace std;

const int twoToriNumVertices = 832;

const int tetrahedronNumVertices = 12;

const int sphereNumVertices = 12288;

const int planetTorusNumVertices = 416;

const int groundNumVertices = 19200;

const int NumVertices = twoToriNumVertices + tetrahedronNumVertices + sphereNumVertices + planetTorusNumVertices + groundNumVertices;

const int NumTimesToSubdivide = 5;

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

vec4 points[NumVertices];
vec3   normals[NumVertices];


GLuint  ModelView, Projection, Translation, Rotation, ScaleMatrix, AmbientProduct, DiffuseProduct, SpecularProduct, LightPosition, Shininess;

mat4 translation, rotation, scaleMatrix;

//Spaceship variables
vec3 spaceShipCenterInitial = vec3(105.0, 0.0, 15.0);

vec3 translateVector = vec3(0.0, 0.0, 0.0);

vec3 velocityVector = vec3(0.0, -1.0, 0.0);

GLfloat spaceShipTheta = 270;

GLfloat spaceShipSpeed = 0.2;

//Station variables
vec3 stationCenter = vec3(100.0, 10.0, 10.0);

GLfloat stationTheta = 0.0;

GLfloat stationRotationSpeed = 0.0;


GLfloat planetTorusTheta = 0.0;

GLfloat groundSquareSize = 5.0;


//NPC spaceship variables
vec3 translateVectorNPC = vec3(0.0, 0.0, 0.0);

vec3 velocityVectorNPC = vec3(0.0, 1.0, 0.0);

GLfloat spaceShipThetaNPC = 90;

GLfloat spaceShipSpeedNPC = 1.0;

//Planet variables
vec3 planet_coords[] = { 
vec3(30, 30, 30),
vec3(30, 170, 15),
vec3(80, 110, 25),
vec3(70, 60, 12),
vec3(90, 150, 13),
vec3(120, 80, 17),
vec3(150, 40, 15),
vec3(160, 170, 22) };

GLfloat planet_colors[][3] = { 
{0.30, 0.30, 0.30},
{1.00, 0.00, 0.00},
{0.00, 1.00, 0.00},
{0.00, 0.00, 1.00},
{1.00, 1.00, 0.00},
{1.00, 0.00, 1.00},
{0.00, 1.00, 1.00},
{1.00, 1.00, 1.00} };


color4 light_ambient(0.2, 0.2, 0.2, 1.0);
color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
color4 light_specular(1.0, 1.0, 1.0, 1.0);

point4 light_position(1000.0, 1000.0, 100.0, 1.0);


int viewMode = 1;

GLfloat zoom = 100.0;


int Index = 0;

bool isPaused = false;
bool isOneStepNeeded = false;

bool isAutoPilotEnabled = false;

//Return normal of p1 that is neighbour to p2 and p3
vec3 calcNormal(vec4 p1, vec4 p2, vec4 p3)
{
    vec4 V1 = (p2 - p1);
    vec4 V2 = (p3 - p1);
    vec3 surfaceNormal;
    surfaceNormal.x = (V1.y * V2.z) - (V1.z * V2.y);
    surfaceNormal.y = (V1.z * V2.x) - (V1.x * V2.z);
    surfaceNormal.z = (V1.x * V2.y) - (V1.y * V2.x);
    
    double length = sqrt(surfaceNormal.x * surfaceNormal.x + surfaceNormal.y * surfaceNormal.y + surfaceNormal.z * surfaceNormal.z);
    surfaceNormal.x /= length;
    surfaceNormal.y /= length;
    surfaceNormal.z /= length;
    
    return surfaceNormal;
}

//Initializes the torus of the planet
void planetTorus(int numc, int numt, float radius, float tubeRadius) {
    int i, j, k;
    double s, t, x, y, z, twopi, sangle, tangle, tx, ty, tz, sx, sy, sz, nx, ny, nz, length;

    twopi = 2 * M_PI;
    for (i = 0; i < numc; i++) {
        for (j = 0; j <= numt; j++) {
            for (k = 1; k >= 0; k--) {
                s = (i + k) % numc + 0.5;
                t = j % numt;

                double sangle = s * twopi / numc;
                double tangle = t * twopi / numt;

                x = (radius + tubeRadius * cos(sangle)) * cos(tangle);
                y = (radius + tubeRadius * cos(sangle)) * sin(tangle);
                z = tubeRadius * sin(sangle);

                points[Index] = vec4(x, y, z, 1.0);

                tx = -sin(tangle);
                ty = cos(tangle);
                tz = 0;

                sx = cos(tangle) * (-sin(sangle));
                sy = sin(tangle) * (-sin(sangle));
                sz = cos(sangle);

                nx = ty * sz - tz * sy;
                ny = tz * sx - tx * sz;
                nz = tx * sy - ty * sx;
                length = sqrt(nx * nx + ny * ny + nz * nz);
                nx /= length;
                ny /= length;
                nz /= length;
                normals[Index] = vec3(nx, ny, nz);
                Index++;

            }
        }
    }
}

//Initializes vertices of the space ship
void initSpaceShip(vec3 center, int numc, int numt, float radius, float tubeRadius)
{
    int i, j, k;
    double s, t, x, y, z, twopi, sangle, tangle, tx, ty, tz, sx, sy, sz, nx, ny, nz, length;

    twopi = 2 * M_PI;
    for (i = 0; i < numc; i++) {
        for (j = 0; j <= numt; j++) {
            for (k = 1; k >= 0; k--) {
                s = (i + k) % numc + 0.5;
                t = j % numt;

                double sangle = s * twopi / numc;
                double tangle = t * twopi / numt;

                x = (radius + tubeRadius * cos(sangle)) * cos(tangle);
                y = (radius + tubeRadius * cos(sangle)) * sin(tangle);
                z = tubeRadius * sin(sangle);

                points[Index] = vec4(x + center.x, y + center.y, z + center.z, 1.0);

                tx = -sin(tangle);
                ty = cos(tangle);
                tz = 0;

                sx = cos(tangle) * (-sin(sangle));
                sy = sin(tangle) * (-sin(sangle));
                sz = cos(sangle);

                nx = ty * sz - tz * sy;
                ny = tz * sx - tx * sz;
                nz = tx * sy - ty * sx;
                length = sqrt(nx * nx + ny * ny + nz * nz);
                nx /= length;
                ny /= length;
                nz /= length;
                normals[Index] = vec3(nx, ny, nz);
                Index++;

            }
        }
    }
    for (i = 0; i < numc; i++) {
        for (j = 0; j <= numt; j++) {
            for (k = 1; k >= 0; k--) {
                s = (i + k) % numc + 0.5;
                t = j % numt;

                double sangle = s * twopi / numc;
                double tangle = t * twopi / numt;

                x = (radius + tubeRadius * cos(sangle)) * cos(tangle);
                y = (radius + tubeRadius * cos(sangle)) * sin(tangle);
                z = tubeRadius * sin(sangle);
                points[Index] = vec4(x + center.x, z + center.y, y + center.z, 1.0);

                tx = -sin(tangle);
                ty = cos(tangle);
                tz = 0;

                sx = cos(tangle) * (-sin(sangle));
                sy = sin(tangle) * (-sin(sangle));
                sz = cos(sangle);

                nx = ty * sz - tz * sy;
                ny = tz * sx - tx * sz;
                nz = tx * sy - ty * sx;
                length = sqrt(nx * nx + ny * ny + nz * nz);
                nx /= length;
                ny /= length;
                nz /= length;
                normals[Index] = vec3(nx, nz, ny);
                Index++;

            }
        }
    }

    vec4 tempVec1 = vec4(radius + tubeRadius + center.x, 0.2 + center.y, 0.0 + center.z, 1.0);
    vec4 tempVec2 = vec4(radius + tubeRadius + center.x, -0.2 + center.y, -0.2 + center.z, 1.0);
    vec4 tempVec3 = vec4(radius + tubeRadius + center.x, -0.2 + center.y, 0.2 + center.z, 1.0);
    vec4 tempVec4 = vec4(radius + tubeRadius * 4 + center.x, 0.0 + center.y, 0.0 + center.z, 1.0);
    vec3 normal;

    normal = calcNormal(tempVec1, tempVec2, tempVec3);
    points[Index] = tempVec1; normals[Index] = normal; Index++;

    normal = calcNormal(tempVec2, tempVec3, tempVec1);
    points[Index] = tempVec2; normals[Index] = normal; Index++;
    
    normal = calcNormal(tempVec3, tempVec1, tempVec2);
    points[Index] = tempVec3; normals[Index] = normal; Index++;


    normal = calcNormal(tempVec1, tempVec4, tempVec2);
    points[Index] = tempVec1; normals[Index] = normal; Index++;

    normal = calcNormal(tempVec2, tempVec1, tempVec4);
    points[Index] = tempVec2; normals[Index] = normal; Index++;

    normal = calcNormal(tempVec4, tempVec2, tempVec1);
    points[Index] = tempVec4; normals[Index] = normal; Index++;


    normal = calcNormal(tempVec4, tempVec3, tempVec2);
    points[Index] = tempVec4; normals[Index] = normal; Index++;

    normal = calcNormal(tempVec2, tempVec4, tempVec3);
    points[Index] = tempVec2; normals[Index] = normal; Index++;

    normal = calcNormal(tempVec3, tempVec2, tempVec4);
    points[Index] = tempVec3; normals[Index] = normal; Index++;


    normal = calcNormal(tempVec1, tempVec3, tempVec4);
    points[Index] = tempVec1; normals[Index] = normal; Index++;

    normal = calcNormal(tempVec3, tempVec4, tempVec1);
    points[Index] = tempVec3; normals[Index] = normal; Index++;

    normal = calcNormal(tempVec4, tempVec1, tempVec3);
    points[Index] = tempVec4; normals[Index] = normal; Index++;

}

//Initializes a square object of the ground which has a vertex on (i, j, 0) that is the closest vertex of the square to the origin.
void groundSquare(GLfloat i, GLfloat j, GLfloat xSign, GLfloat ySign) {

    points[Index] = vec4(i, j, 0.0, 1.0); Index++;
    points[Index] = vec4(i + groundSquareSize * xSign, j, 0.0, 1.0); Index++;
    points[Index] = vec4(i, j + groundSquareSize * ySign, 0.0, 1.0); Index++;

    points[Index] = vec4(i + groundSquareSize * xSign, j + groundSquareSize * ySign, 0.0, 1.0); Index++;
    points[Index] = vec4(i + groundSquareSize * xSign, j, 0.0, 1.0); Index++;
    points[Index] = vec4(i, j + groundSquareSize * ySign, 0.0, 1.0); Index++;
}

//Initializes checkerboard ground
void initGround() {
    int index1 = Index;
    for (GLfloat i = 0.0; i < 200.0; i += groundSquareSize) {
        for (GLfloat j = 0.0; j < 200.0; j += groundSquareSize) {
            if (((int)(i/ groundSquareSize + j/ groundSquareSize)) % 2 == 0) {
                groundSquare(i, -j, 1.0, -1.0);
                groundSquare(-i, j, -1.0, 1.0);
            }
            else {
                groundSquare(i, j, 1.0, 1.0);
                groundSquare(-i, -j, -1.0, -1.0);
            }
        }
    }
}

void
triangle(const point4& a, const point4& b, const point4& c)
{
    normals[Index] = vec3(a.x, a.y, a.z);  points[Index] = a;  Index++;
    normals[Index] = vec3(b.x, b.y, b.z);  points[Index] = b;  Index++;
    normals[Index] = vec3(c.x, c.y, c.z);  points[Index] = c;  Index++;
}

point4
unit(const point4& p)
{
    float len = p.x * p.x + p.y * p.y + p.z * p.z;

    point4 t;
    if (len > DivideByZeroTolerance) {
        t = p / sqrt(len);
        t.w = 1.0;
    }

    return t;
}

void
divide_triangle(const point4& a, const point4& b,
    const point4& c, int count)
{
    if (count > 0) {
        point4 v1 = unit(a + b);
        point4 v2 = unit(a + c);
        point4 v3 = unit(b + c);
        divide_triangle(a, v1, v2, count - 1);
        divide_triangle(c, v2, v3, count - 1);
        divide_triangle(b, v3, v1, count - 1);
        divide_triangle(v1, v3, v2, count - 1);
    }
    else {
        triangle(a, b, c);
    }
}

//Initializes sphere (planet)
void
tetrahedron(int count)
{
    point4 v[4] = {
    vec4(0.0, 0.0, 1.0, 1.0),
    vec4(0.0, 0.942809, -0.333333, 1.0),
    vec4(-0.816497, -0.471405, -0.333333, 1.0),
    vec4(0.816497, -0.471405, -0.333333, 1.0)
    };

    divide_triangle(v[0], v[1], v[2], count);
    divide_triangle(v[3], v[2], v[1], count);
    divide_triangle(v[0], v[3], v[1], count);
    divide_triangle(v[0], v[2], v[3], count);
}

//OpenGL initialization
void
init()
{
    initSpaceShip(spaceShipCenterInitial, 8, 25, 2.0, 0.4);

    tetrahedron(NumTimesToSubdivide);

    planetTorus(8, 25, 0.5, 0.1);

    initGround();

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals),
        NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(points),
        sizeof(normals), normals);

    GLuint program = InitShader("src/vshader.glsl", "src/fshader.glsl");
    glUseProgram(program);

    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(0));

    GLuint vNormal = glGetAttribLocation(program, "vNormal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(sizeof(points)));

    AmbientProduct = glGetUniformLocation(program, "AmbientProduct");
    DiffuseProduct = glGetUniformLocation(program, "DiffuseProduct");
    SpecularProduct = glGetUniformLocation(program, "SpecularProduct");
    LightPosition = glGetUniformLocation(program, "LightPosition");
    Shininess = glGetUniformLocation(program, "Shininess");

    ModelView = glGetUniformLocation(program, "ModelView");
    Projection = glGetUniformLocation(program, "Projection");

    Translation = glGetUniformLocation(program, "Translation");
    Rotation = glGetUniformLocation(program, "Rotation");
    ScaleMatrix = glGetUniformLocation(program, "ScaleMatrix");

    glEnable(GL_DEPTH_TEST);

    glClearColor(1.0, 1.0, 1.0, 1.0);
}

//Turns randomly NPC ship
void randomlyTurnNPC() {
    int turn = rand() % 100;
    if (turn != 0) {
        return;
    }
    int direction = rand() % 2;
    if (direction == 0) {
        velocityVectorNPC -= cross(velocityVectorNPC, vec3(0.0, 0.0, 1.0));
        velocityVectorNPC = normalize(velocityVectorNPC);
        spaceShipThetaNPC += 45;
        if (spaceShipThetaNPC >= 360) {
            spaceShipThetaNPC -= 360;
        }
    }
    else {
        velocityVectorNPC += cross(velocityVectorNPC, vec3(0.0, 0.0, 1.0));
        velocityVectorNPC = normalize(velocityVectorNPC);
        spaceShipThetaNPC -= 45;
        if (spaceShipThetaNPC >= 360) {
            spaceShipThetaNPC -= 360;
        }
    }
}

//Turns NPC ship 190 degree
void turnNPC180() {
    for (int i = 0; i < 4; i++) {
        velocityVectorNPC += cross(velocityVectorNPC, vec3(0.0, 0.0, 1.0));
        velocityVectorNPC = normalize(velocityVectorNPC);
        spaceShipThetaNPC -= 45;
        if (spaceShipThetaNPC >= 360) {
            spaceShipThetaNPC -= 360;
        }
    }
}

bool equals(double a, double b)
{
    return fabs(a - b) < 0.001;
}

//Turns ship 45 degree unti velocity vector equals to given vec
void turnShipUntilVelocityEqualsTo(vec3 vec) {
    while(true) {
        if (equals(velocityVector.x, vec.x) && equals(velocityVector.y, vec.y)) {
            break;
        }
        velocityVector += cross(velocityVector, vec3(0.0, 0.0, 1.0));
        velocityVector = normalize(velocityVector);
        spaceShipTheta -= 45;
        if (spaceShipTheta >= 360) {
            spaceShipTheta -= 360;
        }
    }
}

//Timer function for changing simulation values
void myTimer(int te)
{
    if (!isPaused || (isPaused && isOneStepNeeded)) {

        randomlyTurnNPC();
        vec3 candidateSpaceShipCenterNPC = spaceShipCenterInitial + translateVectorNPC + (velocityVectorNPC * spaceShipSpeedNPC);
        if (candidateSpaceShipCenterNPC.x >= 180.0 || candidateSpaceShipCenterNPC.x <= 0.0 || candidateSpaceShipCenterNPC.y >= 180.0 || candidateSpaceShipCenterNPC.y <= 0.0) {
            turnNPC180();
        }
        translateVectorNPC = translateVectorNPC + (velocityVectorNPC * spaceShipSpeedNPC);

        if (isAutoPilotEnabled) {
            vec3 distanceVector = stationCenter - (spaceShipCenterInitial + translateVector);
            if (distanceVector.x > 1.0) {
                distanceVector.x = 1.0;
            }
            else if (distanceVector.x < -1.0) {
                distanceVector.x = -1.0;
            }
            else {
                distanceVector.x = 0.0;
            }
            if (distanceVector.y > 1.0) {
                distanceVector.y = 1.0;
            }
            else if (distanceVector.y < -1.0) {
                distanceVector.y = -1.0;
            }
            else if (distanceVector.y != 0.0) {
                distanceVector.y = 0.0;
                if (distanceVector.x == 0.0) {
                    spaceShipSpeed = 0.0;
                    isAutoPilotEnabled = false;
                }
            }
            if (isAutoPilotEnabled) {
                turnShipUntilVelocityEqualsTo(normalize(vec3(distanceVector.x, distanceVector.y, 0.0)));
                translateVector = translateVector + (velocityVector * spaceShipSpeed);
            }
        }
        else {
            translateVector = translateVector + (velocityVector * spaceShipSpeed);
        }

        stationTheta += stationRotationSpeed;
        if (stationTheta >= 360.0) {
            stationTheta -= 360.0;
        }

        planetTorusTheta += 2.0;
        if (planetTorusTheta >= 360.0) {
            planetTorusTheta -= 360.0;
        }
        
        if (isOneStepNeeded) {
            cout << "Space Ship Location :" << spaceShipCenterInitial + translateVector << "\n";
            cout << "Space Station Location :" << stationCenter << "\n";
            for (vec3 planetCenter : planet_coords) {
                cout << "Space Station Location :" << planetCenter << "\n";
            }
            cout << "Space Ship Velocity Vector :" << velocityVector << "\n";
            cout << "Space Ship Speed :" << spaceShipSpeed << "\n";
            cout << "Space Ship Theta :" << spaceShipTheta << "\n";
            cout << "Space Station Center :" << stationCenter << "\n";
            cout << "Space Station Rotation Speed :" << stationRotationSpeed << "\n";
            cout << "Space Station Theta :" << stationTheta << "\n";
            cout << "STEP FINISHED\n";
        }
        isOneStepNeeded = false;
    }

    glutPostRedisplay();
    glutTimerFunc(1000.0/60.0, myTimer, 0);

}

//Display function
void
display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    color4 material_ambient(0.633, 0.727811, 0.633, 1.0);
    color4 material_diffuse(1.0, 0.0, 0.0, 1.0);
    color4 material_specular(0.0215, 0.1745, 0.0215, 1.0);
    float  material_shininess = 960.8;

    color4 ambient_product = light_ambient * material_ambient;
    color4 diffuse_product = light_diffuse * material_diffuse;
    color4 specular_product = light_specular * material_specular;

    glUniform4fv(AmbientProduct, 1, ambient_product);
    glUniform4fv(DiffuseProduct, 1, diffuse_product);
    glUniform4fv(SpecularProduct, 1, specular_product);
    glUniform4fv(LightPosition, 1, light_position);
    glUniform1f(Shininess, material_shininess);

    vec3 spaceShipControlDesk = spaceShipCenterInitial + translateVector + velocityVector * 4;
    point4 eye, at, up;
    switch (viewMode) {
        case 1:
            eye = point4(spaceShipControlDesk.x, spaceShipControlDesk.y, spaceShipControlDesk.z, 1.0);
            at = point4(spaceShipControlDesk.x + velocityVector.x, spaceShipControlDesk.y + velocityVector.y, spaceShipControlDesk.z + velocityVector.z, 1.0);
            up = point4(0.0, 0.0, 1.0, 0.0);
            break;
        case 2:
            eye = point4(stationCenter.x + cos(M_PI * (stationTheta + 90.0) / 180.0) * 11.0, stationCenter.y + sin(M_PI * (stationTheta + 90.0) / 180.0) * 11.0, stationCenter.z, 1.0);
            at = point4(stationCenter.x + cos(M_PI * (stationTheta + 90.0) / 180.0) * 12.0, stationCenter.y + sin(M_PI * (stationTheta + 90.0) / 180.0) * 12.0, stationCenter.z, 1.0);
            up = point4(0.0, 0.0, 1.0, 0.0);
            break;
        case 3:
            eye = point4(spaceShipControlDesk.x - velocityVector.x * 12, spaceShipControlDesk.y - velocityVector.y * 12, spaceShipControlDesk.z + 8.0, 1.0);
            at = point4(spaceShipControlDesk.x, spaceShipControlDesk.y, spaceShipControlDesk.z, 1.0);
            up = point4(0.0, 0.0, 1.0, 0.0);
            break;
        case 4:
            eye = point4(0.0, 0.0, 330.0, 1.0);
            at = point4(0.0, 0.0, 0.0, 1.0);
            up = point4(0.0, 1.0, 0.0, 0.0);
            break;
        case 5:
            eye = point4(spaceShipControlDesk.x, spaceShipControlDesk.y, spaceShipControlDesk.z + zoom, 1.0);
            at = point4(spaceShipControlDesk.x, spaceShipControlDesk.y, spaceShipControlDesk.z, 1.0);
            up = point4(0.0, 1.0, 0.0, 0.0);
            break;
    }

    mat4 model_view = LookAt(eye, at, up);
    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);

    //Scapeship
    translation = Translate(translateVector);
    rotation = Translate(spaceShipCenterInitial) * RotateZ(spaceShipTheta) * Translate(-spaceShipCenterInitial);
    scaleMatrix = Scale(1.0, 1.0, 1.0);
    
    
    glUniformMatrix4fv(Translation, 1, GL_TRUE, translation);
    glUniformMatrix4fv(Rotation, 1, GL_TRUE, rotation);
    glUniformMatrix4fv(ScaleMatrix, 1, GL_TRUE, scaleMatrix);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, twoToriNumVertices);
    glDrawArrays(GL_TRIANGLE_STRIP, twoToriNumVertices, tetrahedronNumVertices);

    //Scapeship NPC
    material_ambient = point4(0.633, 0.727811, 0.633, 1.0);
    material_diffuse = point4(0.0, 0.0, 1.0, 1.0);
    material_specular = point4(0.0215, 0.1745, 0.0215, 1.0);
    material_shininess = 960.8;

    ambient_product = light_ambient * material_ambient;
    diffuse_product = light_diffuse * material_diffuse;
    specular_product = light_specular * material_specular;

    glUniform4fv(AmbientProduct, 1, ambient_product);
    glUniform4fv(DiffuseProduct, 1, diffuse_product);
    glUniform4fv(SpecularProduct, 1, specular_product);
    glUniform4fv(LightPosition, 1, light_position);
    glUniform1f(Shininess, material_shininess);

    translation = Translate(translateVectorNPC);
    rotation = Translate(spaceShipCenterInitial) * RotateZ(spaceShipThetaNPC) * Translate(-spaceShipCenterInitial);
    scaleMatrix = Scale(1.0, 1.0, 1.0);

    glUniformMatrix4fv(Translation, 1, GL_TRUE, translation);
    glUniformMatrix4fv(Rotation, 1, GL_TRUE, rotation);
    glUniformMatrix4fv(ScaleMatrix, 1, GL_TRUE, scaleMatrix);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, twoToriNumVertices);
    glDrawArrays(GL_TRIANGLE_STRIP, twoToriNumVertices, tetrahedronNumVertices);

    //Station
    material_ambient = point4(0.633, 0.727811, 0.633, 1.0);
    material_diffuse = point4(0.6, 0.6, 0.6, 1.0);
    material_specular = point4(0.0215, 0.1745, 0.0215, 1.0);
    material_shininess = 960.8;

    ambient_product = light_ambient * material_ambient;
    diffuse_product = light_diffuse * material_diffuse;
    specular_product = light_specular * material_specular;

    glUniform4fv(AmbientProduct, 1, ambient_product);
    glUniform4fv(DiffuseProduct, 1, diffuse_product);
    glUniform4fv(SpecularProduct, 1, specular_product);
    glUniform4fv(LightPosition, 1, light_position);
    glUniform1f(Shininess, material_shininess);

    scaleMatrix = Scale(10.0, 10.0, 10.0);
    translation = Translate(stationCenter);
    rotation = RotateZ(0.0);
    glUniformMatrix4fv(Translation, 1, GL_TRUE, translation);
    glUniformMatrix4fv(Rotation, 1, GL_TRUE, rotation);
    glUniformMatrix4fv(ScaleMatrix, 1, GL_TRUE, scaleMatrix);
    glDrawArrays(GL_TRIANGLES, twoToriNumVertices + tetrahedronNumVertices, sphereNumVertices);

    //The front of station
    scaleMatrix = Scale(1.0, 1.0, 1.0);
    translation = Translate(stationCenter);
    rotation = RotateZ(stationTheta) * Translate(vec3(0.0, 10.0, 0.0));
    glUniformMatrix4fv(Translation, 1, GL_TRUE, translation);
    glUniformMatrix4fv(Rotation, 1, GL_TRUE, rotation);
    glUniformMatrix4fv(ScaleMatrix, 1, GL_TRUE, scaleMatrix);
    glDrawArrays(GL_TRIANGLES, twoToriNumVertices + tetrahedronNumVertices, sphereNumVertices);

    //Planets
    for (int i = 0; i < sizeof(planet_coords)/sizeof(*planet_coords); i++) {
        vec3 planetCenter = planet_coords[i];
        scaleMatrix = Scale(3.0, 3.0, 3.0);
        translation = Translate(planetCenter);
        rotation = RotateZ(0.0);

        color4 material_ambient(1.0, 1.0, 1.0, 1.0);
        color4 material_diffuse(planet_colors[i][0], planet_colors[i][1], planet_colors[i][2], 1.0);
        color4 material_specular(1.0, 1.0, 1.0, 1.0);
        float  material_shininess = 960.8;

        color4 ambient_product = light_ambient * material_ambient;
        color4 diffuse_product = light_diffuse * material_diffuse;
        color4 specular_product = light_specular * material_specular;

        glUniform4fv(AmbientProduct, 1, ambient_product);
        glUniform4fv(DiffuseProduct, 1, diffuse_product);
        glUniform4fv(SpecularProduct, 1, specular_product);
        glUniform4fv(LightPosition, 1, light_position);
        glUniform1f(Shininess, material_shininess);

        glUniformMatrix4fv(Translation, 1, GL_TRUE, translation);
        glUniformMatrix4fv(Rotation, 1, GL_TRUE, rotation);
        glUniformMatrix4fv(ScaleMatrix, 1, GL_TRUE, scaleMatrix);
        glDrawArrays(GL_TRIANGLES, twoToriNumVertices + tetrahedronNumVertices, sphereNumVertices);

        //Planet torus
        scaleMatrix = Scale(9.0, 9.0, 9.0);
        translation = Translate(planetCenter);
        rotation = RotateY(planetTorusTheta);
        glUniformMatrix4fv(Translation, 1, GL_TRUE, translation);
        glUniformMatrix4fv(Rotation, 1, GL_TRUE, rotation);
        glUniformMatrix4fv(ScaleMatrix, 1, GL_TRUE, scaleMatrix);

        glDrawArrays(GL_TRIANGLE_STRIP, twoToriNumVertices + tetrahedronNumVertices + sphereNumVertices, planetTorusNumVertices);
    }

    translation = Translate(0.0, 0.0, 0.0);
    rotation = RotateZ(0.0);
    scaleMatrix = Scale(1.0, 1.0, 1.0);
    glUniformMatrix4fv(Translation, 1, GL_TRUE, translation);
    glUniformMatrix4fv(Rotation, 1, GL_TRUE, rotation);
    glUniformMatrix4fv(ScaleMatrix, 1, GL_TRUE, scaleMatrix);

    glDrawArrays(GL_TRIANGLES, twoToriNumVertices + tetrahedronNumVertices + sphereNumVertices + planetTorusNumVertices, groundNumVertices);
    glutSwapBuffers();
}

//Mouse input function
void myMouseInput(int button, int state, int x, int y) {
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        if (isPaused) {
            isOneStepNeeded = true;
        }
        else {
            isPaused = true;
        }
    }
    glutPostRedisplay();
}

//Keyboard input function
void
keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 033: // Escape Key
        case 'q': case 'Q':
            exit(EXIT_SUCCESS);
            break;
        case 'c': case 'C':
            viewMode = 1;
            break;
        case 's': case 'S':
            viewMode = 2;
            break;
        case 't': case 'T':
            viewMode = 3;
            break;
        case 'w': case 'W':
            viewMode = 4;
            break;
        case 'z': case 'Z':
            viewMode = 5;
            break;
        case '+':
            if (viewMode == 5) {
                zoom -= 5.0;
            }
            if (zoom <= 20.0) {
                zoom = 20.0;
            }
            break;
        case '-':
            if (viewMode == 5) {
                zoom += 5.0;
            }
            if (zoom >= 320.0) {
                zoom = 320.0;
            }
            break;
        case 'a': case 'A':
            if (viewMode != 2) {
                spaceShipSpeed += 0.05;
            }
            break;
        case 'd': case 'D':
            if (viewMode != 2) {
                spaceShipSpeed -= 0.05;
            }
            if (spaceShipSpeed < 0.0) {
                spaceShipSpeed = 0.0;
            }
            break;
        case 'j': case 'J':
            stationRotationSpeed += 1.0;
            break;
        case 'k': case 'K':
            stationRotationSpeed -= 1.0;
            if (stationRotationSpeed < 0) {
                stationRotationSpeed = 0;
            }
            break;
        case 'p': case 'P':
            if (isPaused) {
                isPaused = false;
            }
            else {
                isPaused = true;
            }
            break;
        case 'o': case 'O':
            if (isAutoPilotEnabled) {
                isAutoPilotEnabled = false;
            }
            else {
                isAutoPilotEnabled = true;
            }
            break;
          
    }


}

//Special input function for arrow inputs
void mySpecialInput(int key, int x, int y)
{

    if (isAutoPilotEnabled) {
        return;
    }
    switch (key)
    {
    case GLUT_KEY_LEFT:
        velocityVector -= cross(velocityVector, vec3(0.0, 0.0, 1.0));
        velocityVector = normalize(velocityVector);
        spaceShipTheta += 45;
        if (spaceShipTheta >= 360) {
            spaceShipTheta -= 360;
        }
        break;
    case GLUT_KEY_RIGHT:
        velocityVector += cross(velocityVector, vec3(0.0, 0.0, 1.0));
        velocityVector = normalize(velocityVector);
        spaceShipTheta -= 45;
        if (spaceShipTheta >= 360) {
            spaceShipTheta -= 360;
        }
    }
}

//Reshape function 
void
reshape(int width, int height)
{
    glViewport(0, 0, width, height);

    GLfloat aspect = GLfloat(width) / height;

    mat4  projection = Perspective(60.0, aspect, 0.5, 340.0);

    glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);


}

//Main function
int
main(int argc, char** argv)
{

    glutInit(&argc, argv);

#ifdef __APPLE__
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_RGBA | GLUT_DEPTH);
#else
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
#endif

    glutInitWindowSize(512, 512);

    glutCreateWindow("SpaceShipSimulation");
    glewExperimental = GL_TRUE;
    glewInit();

    init();

    srand(time(NULL));
    glutTimerFunc(1000.0/60.0, myTimer, 0);

    glutDisplayFunc(display);
    glutSpecialFunc(mySpecialInput);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(myMouseInput);

    glutMainLoop();
    return 0;
}