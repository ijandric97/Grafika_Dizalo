#include <iostream>
#include <GL/glut.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

/** Function Prototypes **/
GLvoid displayFunc();
GLvoid drawCables();
GLvoid drawElevator();
GLvoid drawOutside();
GLvoid drawString(GLint x, GLint y, std::string String);
GLvoid drawWeight();
GLvoid keyboardFunc(GLubyte key, GLint x, GLint y);
GLvoid menuFunc(GLint menuID);
GLvoid reshapeFunc(GLint width, GLint height);
GLvoid specialFunc(GLint key, GLint x, GLint y);
GLvoid timerFunc(GLint value);
GLvoid toggleFullscreen();

/** Constants **/
const GLfloat elevatorHeightDefault = 0.0000000000000000000001; //Ne smije biti nula jer nema osvijetljenja onda
const GLfloat elevatorHeightMax = 10;

/** Variables **/
GLfloat elevatorHeight = elevatorHeightDefault;

GLfloat camAngle = -2.25;
GLfloat camDist = -15;

GLboolean isStarted = false;

GLint windowWidth = 800;
GLint windowHeight = 600;

/** Functions **/
GLvoid displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear screen with color (black)

    glMatrixMode(GL_MODELVIEW); //Switch to Perspective
    glLoadIdentity();           //Reset

    /** Postavljanje Svijetla **/
    glLightfv(GL_LIGHT0, GL_AMBIENT, glm::value_ptr(glm::vec4(0, 0, 0.2, 0)));
    glLightfv(GL_LIGHT0, GL_DIFFUSE, glm::value_ptr(glm::vec4(1)));
    glLightfv(GL_LIGHT0, GL_SPECULAR, glm::value_ptr(glm::vec4(1)));
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, glm::value_ptr(glm::vec3(0, 0, 0)));
    glLightfv(GL_LIGHT0, GL_POSITION, glm::value_ptr(glm::vec4(0, elevatorHeightMax+1, -1, 0)));

    /** Set Camera **/
    glm::mat4 camera = glm::lookAt(glm::vec3(camDist*sin(camAngle), (elevatorHeightMax/2), camDist*cos(camAngle)), //Eye
                                   glm::vec3(0, (elevatorHeightMax/2)+0.25, 0.0f),                                 //Center
                                   glm::vec3(0.0f, 1.0f, 0.0f));                                                   //Up
    glMultMatrixf(glm::value_ptr(camera));  //Multiply ModelView (Perspective) with camera transformation

    /** Main Scene **/
    drawElevator();
    drawWeight();
    drawOutside();
    drawCables();

    /** Help Text **/
    drawString(10, windowHeight-20, "Arrows - Camera");
    drawString(10, windowHeight-35, "Space - Start / Stop");
    drawString(10, windowHeight-50, "Esc - Exit");
    drawString(10, windowHeight-65, "F1 - Fullscreen");

    drawString(10, 10, "Angle: " + std::to_string(camAngle));
    drawString(10, 25, "Dist:" + std::to_string(camDist));
    drawString(10, 40, "Camera (radians):");

    glutSwapBuffers(); //Swap the front and back buffer
}

GLvoid drawCables() {
    glm::vec3 normalVect;
    glm::mat4 translate_matrix;

    GLfloat cableCoords[18] = {
        0, elevatorHeightMax+1, -1,     0, elevatorHeight, -1,
        0, elevatorHeightMax+1, 1,      0, elevatorHeightDefault + ((elevatorHeightMax)-elevatorHeight), 1,
        0, elevatorHeightMax+1, -1,     0, elevatorHeightMax+1, 1
    };

    GLfloat shininess = 200;                                                     //Set Material for Cable
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(glm::vec3(0.3)));   //Black base
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(glm::vec3(0.5))); //Silver color
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(glm::vec3(1)));  //White light
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);
    glLineWidth(4);

    glPushMatrix();
    glBegin(GL_LINES);
    for (int i = 0; i < 18; i += 3) {
        glm::vec3 normalVect;

        normalVect = glm::normalize(glm::vec3(cableCoords[i], cableCoords[i+1], cableCoords[i+2]));
        glNormal3f(normalVect[0], normalVect[1], normalVect[2]);
        glVertex3f(cableCoords[i], cableCoords[i+1], cableCoords[i+2]);
    }
    glEnd();
    glPopMatrix();
}

GLvoid drawElevator() {
    GLfloat elevatorCoords[72] = {
        0.25,0.5,0.2,    -0.25,0.5,0.2,   -0.25,-0.5,0.2,   0.25,-0.5,0.2,   //Prednja
        0.25,0.5,0.2,     0.25,-0.5,0.2,   0.25,-0.5,-0.2,  0.25,0.5,-0.2,   //Desna
        0.25,0.5,0.2,     0.25,0.5,-0.2,  -0.25,0.5,-0.2,  -0.25,0.5,0.2,    //Gornja
        -0.25,-0.5,-0.2, -0.25,0.5,-0.2,   0.25,0.5,-0.2,   0.25,-0.5,-0.2,  //Straznja
        -0.25,-0.5,-0.2, -0.25,-0.5,0.2,  -0.25,0.5,0.2,   -0.25,0.5,-0.2,   //Lijeva
        -0.25,-0.5,-0.2,  0.25,-0.5,-0.2,  0.25,-0.5,0.2,  -0.25,-0.5,0.2    //Donja
    };

    GLfloat shininess = 50;                                                          //Set Material for Elevator
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(glm::vec3(0.3)));     //Black base
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(glm::vec3(0, 0, 1))); //Blue color
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(glm::vec3(1)));      //White light
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);

    glPushMatrix();
    glm::mat4 translate_matrix = glm::translate(glm::mat4(1), glm::vec3(0, elevatorHeight, -1));
    glMultMatrixf(glm::value_ptr(translate_matrix));
    glBegin(GL_QUADS);
    for (int i = 0; i < 72; i += 3) {
        glm::vec3 normalVect;

        normalVect = glm::normalize(glm::vec3(elevatorCoords[i], elevatorCoords[i+1], elevatorCoords[i+2]));
        glNormal3f(normalVect[0], normalVect[1], normalVect[2]);
        glVertex3f(elevatorCoords[i], elevatorCoords[i+1], elevatorCoords[i+2]);
    }
    glEnd();
    glPopMatrix();
}

GLvoid drawOutside() {
    GLfloat outsideCoords[72] = {
        4,0.2,1,    -4,0.2,1,   -4,-0.2,1,   4,-0.2,1,   //Prednja
        4,0.2,1,     4,-0.2,1,   4,-0.2,-1,  4,0.2,-1,   //Desna
        4,0.2,1,     4,0.2,-1,  -4,0.2,-1,  -4,0.2,1,    //Gornja
        -4,-0.2,-1, -4,0.2,-1,   4,0.2,-1,   4,-0.2,-1,  //Straznja
        -4,-0.2,-1, -4,-0.2,1,  -4,0.2,1,   -4,0.2,-1,   //Lijeva
        -4,-0.2,-1,  4,-0.2,-1,  4,-0.2,1,  -4,-0.2,1    //Donja
    };

    GLfloat shininess = 5;                                                           //Set Material for Elevator
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(glm::vec3(0.05)));    //Black base
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(glm::vec3(0, 1, 1))); //Grass? color
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(glm::vec3(1)));      //White light
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);

    glPushMatrix();
    glm::mat4 translate_matrix = glm::translate(glm::mat4(1), glm::vec3(0, -0.7, 2));
    glMultMatrixf(glm::value_ptr(translate_matrix));
    glBegin(GL_QUADS);
    for (int i = 0; i < 72; i += 3) {
        glm::vec3 normalVect;

        normalVect = glm::normalize(glm::vec3(outsideCoords[i], outsideCoords[i+1], outsideCoords[i+2]));
        glNormal3f(normalVect[0], normalVect[1], normalVect[2]);
        glVertex3f(outsideCoords[i], outsideCoords[i+1], outsideCoords[i+2]);
    }
    glEnd();
    glPopMatrix();

    glPushMatrix();
    translate_matrix = glm::translate(glm::mat4(1), glm::vec3(0, -0.7, -2));
    glMultMatrixf(glm::value_ptr(translate_matrix));
    glBegin(GL_QUADS);
    for (int i = 0; i < 72; i += 3) {
        glm::vec3 normalVect;

        normalVect = glm::normalize(glm::vec3(outsideCoords[i], outsideCoords[i+1], outsideCoords[i+2]));
        glNormal3f(normalVect[0], normalVect[1], normalVect[2]);
        glVertex3f(outsideCoords[i], outsideCoords[i+1], outsideCoords[i+2]);
    }
    glEnd();
    glPopMatrix();

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(glm::vec3(1, 1, 0))); //Sand? color

    glPushMatrix();
    translate_matrix = glm::translate(glm::mat4(1), glm::vec3(0, -0.7, 0));
    glMultMatrixf(glm::value_ptr(translate_matrix));
    glBegin(GL_QUADS);
    for (int i = 0; i < 72; i += 3) {
        glm::vec3 normalVect;

        normalVect = glm::normalize(glm::vec3(outsideCoords[i], outsideCoords[i+1], outsideCoords[i+2]));
        glNormal3f(normalVect[0], normalVect[1], normalVect[2]);
        glVertex3f(outsideCoords[i], outsideCoords[i+1], outsideCoords[i+2]);
    }
    glEnd();
    glPopMatrix();
}

GLvoid drawString(GLint x, GLint y, std::string String) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, windowWidth, 0, windowHeight, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glRasterPos2i(x,y);
    for (GLuint i = 0; i < String.size(); i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, String[i]);
    }
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

GLvoid drawWeight() {
    GLfloat weightCoords[72] = {
        0.125,0.175,0.15,    -0.125,0.175,0.15,   -0.125,-0.175,0.15,   0.125,-0.175,0.15,   //Prednja
        0.125,0.175,0.15,     0.125,-0.175,0.15,   0.125,-0.175,-0.15,  0.125,0.175,-0.15,   //Desna
        0.125,0.175,0.15,     0.125,0.175,-0.15,  -0.125,0.175,-0.15,  -0.125,0.175,0.15,    //Gornja
        -0.125,-0.175,-0.15, -0.125,0.175,-0.15,   0.125,0.175,-0.15,   0.125,-0.175,-0.15,  //Straznja
        -0.125,-0.175,-0.15, -0.125,-0.175,0.15,  -0.125,0.175,0.15,   -0.125,0.175,-0.15,   //Lijeva
        -0.125,-0.175,-0.15,  0.125,-0.175,-0.15,  0.125,-0.175,0.15,  -0.125,-0.175,0.15    //Donja
    };

    GLfloat shininess = 50;                                                          //Set Material for Elevator
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(glm::vec3(0.3)));     //Black base
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(glm::vec3(1, 0, 0))); //Red color
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(glm::vec3(1)));      //White light
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);

    glPushMatrix();
    glm::mat4 translate_matrix = glm::translate(glm::mat4(1), glm::vec3(0, elevatorHeightDefault + ((elevatorHeightMax)-elevatorHeight), 1));
    glMultMatrixf(glm::value_ptr(translate_matrix));
    glBegin(GL_QUADS);
    for (int i = 0; i < 72; i += 3) {
        glm::vec3 normalVect;

        normalVect = glm::normalize(glm::vec3(weightCoords[i], weightCoords[i+1], weightCoords[i+2]));
        glNormal3f(normalVect[0], normalVect[1], normalVect[2]);
        glVertex3f(weightCoords[i], weightCoords[i+1], weightCoords[i+2]);
    }
    glEnd();
    glPopMatrix();
}

GLvoid keyboardFunc(GLubyte key, GLint x, GLint y) {
    switch (key) {
        case 27: //ESC
            exit(EXIT_SUCCESS);
            break;
        case 32: //SPACE
            isStarted = !isStarted;
            break;
    }
}

GLvoid menuFunc(GLint menuID) {
    switch (menuID) {
        case 0: //Start / Stop
            isStarted = !isStarted;
            break;
        case 1: //Exit
            exit(EXIT_SUCCESS);
            break;
        case 2: //Fullscreen
            toggleFullscreen();
            break;
    }
}

GLvoid reshapeFunc(GLint width, GLint height) {
    if(height == 0) {
        height = 1;
    }

    windowWidth = width;
    windowHeight = height;

    const GLfloat aspectRatio = width / (GLfloat)height;
    const GLfloat fieldOfViewY = 45;
    const GLfloat zNear = 0.0001;
    const GLfloat zFar  = 1000.0;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(fieldOfViewY), aspectRatio, zNear, zFar);
    glMultMatrixf(glm::value_ptr(projectionMatrix));

    glMatrixMode(GL_MODELVIEW);
}

GLvoid specialFunc(GLint key, GLint x, GLint y) {
    const GLfloat camDistSpeed = 0.1f;
    const GLfloat camYawSpeed = 0.05f;

    switch (key) {
        case GLUT_KEY_F1:
            toggleFullscreen();
            break;
        case GLUT_KEY_UP:    //Camera Zoom In
            camDist += camDistSpeed;
            break;
        case GLUT_KEY_DOWN:  //Camera Zoom Out
            camDist -= camDistSpeed;
            break;
        case GLUT_KEY_LEFT:  //Camera Yaw Left
            camAngle += camYawSpeed;
            if (camAngle > 6.28) {
                camAngle = camYawSpeed;
            }
            break;
        case GLUT_KEY_RIGHT: //Camera Yaw Right
            camAngle -= camYawSpeed;
            if (camAngle < -6.28) {
                camAngle = -camYawSpeed;
            }
            break;
    }

    glutPostRedisplay();     //Always Trigger a repaint
}

GLvoid timerFunc(GLint value) {
    const GLfloat elevatorSlowSpeed = 0.025f;
    const GLfloat elevatorFastSpeed = 0.1f;

    if (isStarted) {
        if ((elevatorHeight+elevatorSlowSpeed) < elevatorHeightMax) {
            if (elevatorHeight < elevatorHeightDefault+1 || elevatorHeight > elevatorHeightMax-1) {
                elevatorHeight += elevatorSlowSpeed;
            } else {
                elevatorHeight += elevatorFastSpeed;
            }
        } else {
            elevatorHeight = elevatorHeightMax;
        }
    } else {
        elevatorHeight = elevatorHeightDefault;
    }

    glutPostRedisplay();             //Step thru animation (repaint)
    glutTimerFunc(25, timerFunc, 0); //Keep timer func forever registered (infinite loop)
}

GLvoid toggleFullscreen() {
    static GLboolean isFullscreen = false;
    static GLint oldWidth = 0, oldHeight = 0;

    if (!isFullscreen) {          //Switch to fullscreen
        oldWidth = windowWidth;
        oldHeight = windowHeight;
        glutFullScreen();
    } else {                      //Revert back to windowed
        glutReshapeWindow(oldWidth, oldHeight);
    }
    isFullscreen = !isFullscreen;
}

GLint main(GLint argc, GLchar* argv[]) {
    /** Initialization **/
    glutInit(&argc,argv);                                     //Start GLUT
    glutInitWindowPosition(100, 100);                         //Starting Position (top left)
    glutInitWindowSize(windowWidth, windowHeight);            //Starting resolution (800x600)
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); //Color, Double Buffered, 3D

    /** Window Management **/
    glutCreateWindow("Elevator Animation"); //Returns identifier but we got only one window

    /** Menu Management **/
    glutCreateMenu(menuFunc);            //Create Menu with its callback
    glutAddMenuEntry("Start / Stop", 0); //Create first entry with ID 0
    glutAddMenuEntry("Exit", 1);
    glutAddMenuEntry("Fullscreen", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);   //Append to right-click

    /** Lightning **/
    glEnable(GL_DEPTH_TEST); //Enable the depth testing
    glEnable(GL_LIGHTING);   //Enable the lighting
    glEnable(GL_LIGHT0);     //Enable LIGHT0, our Diffuse Light
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH); //Set the shader to smooth shader

    /** Callback Registration **/
    glutDisplayFunc(displayFunc);    //Repaint
    glutReshapeFunc(reshapeFunc);    //Window size change
    glutTimerFunc(25, timerFunc, 0); //Animation
    glutKeyboardFunc(keyboardFunc);  //ASCII Keys
    glutSpecialFunc(specialFunc);    //Non-ASCII Keys

    glutMainLoop(); //Process events
    return 0;
}
