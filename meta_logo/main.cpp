#include <GL/freeglut.h>
#include <cmath>
#include <vector>

struct Point
{
    float x, y;
};

Point drawBezier(Point p0, Point p1, Point p2, Point p3, float t)
{
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    Point p;
    p.x = uuu * p0.x + 3 * uu * t * p1.x + 3 * u * tt * p2.x + ttt * p3.x;
    p.y = uuu * p0.y + 3 * uu * t * p1.y + 3 * u * tt * p2.y + ttt * p3.y;
    return p;
}

void sampleBezier(std::vector<Point> &out,
                  Point p0, Point p1, Point p2, Point p3,
                  int segments = 60)
{
    for (int j = 0; j <= segments; ++j)
    {
        float t = (float)j / (float)segments;
        out.push_back(drawBezier(p0, p1, p2, p3, t));
    }
}

void drawFilledPolygon(const std::vector<Point> &pts)
{
    if (pts.empty())
        return;

    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);

    // Pass 1: rasterise into stencil with INVERT (even-odd fill)
    glStencilFunc(GL_ALWAYS, 0, 0);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(pts[0].x, pts[0].y);
    for (const auto &p : pts)
        glVertex2f(p.x, p.y);
    glVertex2f(pts[0].x, pts[0].y); // explicit close
    glEnd();

    // Pass 2: paint the current colour everywhere stencil != 0
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    glBegin(GL_QUADS);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(32.0f, 0.0f);
    glVertex2f(32.0f, 30.0f);
    glVertex2f(0.0f, 30.0f);
    glEnd();

    glDisable(GL_STENCIL_TEST);
}

void display()
{
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

void init()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0.0, 32.0, 30.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(600, 500);
    glutCreateWindow("Meta Logo Render - OpenGL");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}