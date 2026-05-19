#include <GL/freeglut.h>
#include <cmath>
#include <vector>


struct Point {
    float x, y;
};

Point drawBezier(Point p0, Point p1, Point p2, Point p3, float t) {
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    Point p;
    
    // Bezier 
    p.x = uuu * p0.x + 3 * uu * t * p1.x + 3 * u * tt * p2.x + ttt * p3.x;
    p.y = uuu * p0.y + 3 * uu * t * p1.y + 3 * u * tt * p2.y + ttt * p3.y;
    return p;
}


void sampleBezier(std::vector<Point>& out,
                  Point p0, Point p1, Point p2, Point p3,
                  int segments = 60) {
    for (int j = 0; j <= segments; ++j) {
        float t = (float)j / (float)segments;
        out.push_back(drawBezier(p0, p1, p2, p3, t));
    }
}






void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();

    
    glColor3f(0.0f, 0.506f, 0.984f);
    drawFilledPolygon(buildOuterContour());

    
    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledPolygon(buildLeftInnerHole());

    
    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledPolygon(buildRightInnerHole());

    glutSwapBuffers();
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluOrtho2D(0.0, 32.0, 30.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(600, 500);
    glutCreateWindow("Meta Logo Render - OpenGL");
    init();
    glutDisplayFunc(display);
      // Run program
    glutMainLoop();
    return 0;
}
