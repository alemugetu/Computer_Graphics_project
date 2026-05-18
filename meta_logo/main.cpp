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

// ---------------------------------------------------------------
// OUTER CONTOUR — full figure-eight silhouette
// Traced from the original SVG PATH 3 (M16.4,11 ...)
// ---------------------------------------------------------------
std::vector<Point> buildOuterContour()
{
    std::vector<Point> pts;

    // M16.4,11  c-2.2,-2.9 -4.1,-4 -6.3,-4
    sampleBezier(pts,
                 {16.4f, 11.0f},
                 {14.2f, 8.1f},
                 {12.3f, 7.0f},
                 {10.1f, 7.0f});

    // C5.5,7 2,13.1 2,19.5
    sampleBezier(pts,
                 {10.1f, 7.0f},
                 {5.5f, 7.0f},
                 {2.0f, 13.1f},
                 {2.0f, 19.5f});

    // c0,4 1.9,6.5 5.1,6.5
    sampleBezier(pts,
                 {2.0f, 19.5f},
                 {2.0f, 23.5f},
                 {3.9f, 26.0f},
                 {7.1f, 26.0f});

    // c2.3,0 3.9,-1.1 6.9,-6.3
    sampleBezier(pts,
                 {7.1f, 26.0f},
                 {9.4f, 26.0f},
                 {11.0f, 24.9f},
                 {14.0f, 19.7f});

    // c0,0 1.2,-2.2 2.1,-3.7
    sampleBezier(pts,
                 {14.0f, 19.7f},
                 {14.0f, 19.7f},
                 {15.2f, 17.5f},
                 {16.1f, 16.0f});

    // c0.3,0.5 0.6,1 0.9,1.6
    sampleBezier(pts,
                 {16.1f, 16.0f},
                 {16.4f, 16.5f},
                 {16.7f, 17.0f},
                 {17.0f, 17.6f});

    // l1.4,2.4
    pts.push_back({18.4f, 20.0f});

    // c2.7,4.6 4.2,6.1 6.9,6.1
    sampleBezier(pts,
                 {18.4f, 20.0f},
                 {21.1f, 24.6f},
                 {22.6f, 26.1f},
                 {25.3f, 26.1f});

    // c3.1,0 4.8,-2.6 4.8,-6.7
    sampleBezier(pts,
                 {25.3f, 26.1f},
                 {28.4f, 26.1f},
                 {30.1f, 23.5f},
                 {30.1f, 19.4f});

    // C30,12.6 26.4,7 22.1,7
    sampleBezier(pts,
                 {30.1f, 19.4f},
                 {30.0f, 12.6f},
                 {26.4f, 7.0f},
                 {22.1f, 7.0f});

    // C19.8,7 18,8.8 16.4,11  (closes back to start)
    sampleBezier(pts,
                 {22.1f, 7.0f},
                 {19.8f, 7.0f},
                 {18.0f, 8.8f},
                 {16.4f, 11.0f});

    return pts;
}

// ---------------------------------------------------------------
// LEFT INNER HOLE — oval gap inside the left loop
// Traced from SVG PATH 1 (M5,19.5 ...) forward, closed.
// ---------------------------------------------------------------
std::vector<Point> buildLeftInnerHole()
{
    std::vector<Point> pts;

    // M5,19.5  c0,-4.6 2.3,-9.4 5,-9.4
    sampleBezier(pts,
                 {5.0f, 19.5f},
                 {5.0f, 14.9f},
                 {7.3f, 10.1f},
                 {10.0f, 10.1f});

    // c1.5,0 2.7,0.9 4.6,3.6
    sampleBezier(pts,
                 {10.0f, 10.1f},
                 {11.5f, 10.1f},
                 {12.7f, 11.0f},
                 {14.6f, 13.7f});

    // c-1.8,2.8 -2.9,4.5 -2.9,4.5
    sampleBezier(pts,
                 {14.6f, 13.7f},
                 {12.8f, 16.5f},
                 {11.7f, 18.2f},
                 {11.7f, 18.2f});

    // c-2.4,3.8 -3.2,4.6 -4.5,4.6
    sampleBezier(pts,
                 {11.7f, 18.2f},
                 {9.3f, 22.0f},
                 {8.5f, 22.8f},
                 {7.2f, 22.8f});

    // c-1.3,0 -2.2,-1.2 -2.2,-3.3  (closes back to M5,19.5)
    sampleBezier(pts,
                 {7.2f, 22.8f},
                 {5.9f, 22.8f},
                 {5.0f, 21.6f},
                 {5.0f, 19.5f});

    return pts;
}

// ---------------------------------------------------------------
// RIGHT INNER HOLE — oval gap inside the right loop
// Traced from SVG PATH 2 (M20.7,17.8 L19,15 ...) forward, closed.
// Mirrors the left hole structure exactly — each segment corresponds
// to its counterpart in buildLeftInnerHole().
// ---------------------------------------------------------------
std::vector<Point> buildRightInnerHole()
{
    std::vector<Point> pts;

    // M20.7,17.8  — entry at the crossing bridge (top of right hole)
    // L19,15      — straight line to the bottom-left of the right loop
    pts.push_back({20.7f, 17.8f});
    pts.push_back({19.0f, 15.0f});

    // c-0.4,-0.7 -0.9,-1.4 -1.3,-2
    // curves left+down toward the inner bottom of the right loop
    sampleBezier(pts,
                 {19.0f, 15.0f},
                 {18.6f, 14.3f},
                 {18.1f, 13.6f},
                 {17.7f, 13.0f});

    // c1.5,-2.3 2.7,-3.5 4.2,-3.5
    // sweeps up-right to the top-left inner edge of the right oval
    sampleBezier(pts,
                 {17.7f, 13.0f},
                 {19.2f, 10.7f},
                 {20.4f, 9.5f},
                 {21.9f, 9.5f});

    // c3,0 5.4,4.5 5.4,10.1
    // arcs across the top and right side of the inner right oval
    sampleBezier(pts,
                 {21.9f, 9.5f},
                 {24.9f, 9.5f},
                 {27.3f, 14.0f},
                 {27.3f, 19.6f});

    // c0,2.1 -0.7,3.3 -2.1,3.3
    // curves down to the bottom-right of the inner oval
    sampleBezier(pts,
                 {27.3f, 19.6f},
                 {27.3f, 21.7f},
                 {26.6f, 22.9f},
                 {25.2f, 22.9f});

    // s-2.8,-1.2 -5.4,-5.4
    // smooth curve back up-left, closing to the entry point M20.7,17.8
    // The reflected control point comes from the previous segment's end tangent
    sampleBezier(pts,
                 {25.2f, 22.9f},
                 {23.8f, 22.9f},
                 {22.4f, 21.2f},
                 {20.7f, 17.8f});

    return pts;
}

void drawFilledPolygon(const std::vector<Point> &pts)
{
    if (pts.empty())
        return;

    glEnable(GL_STENCIL_TEST);

    glClear(GL_STENCIL_BUFFER_BIT);

    glStencilFunc(GL_ALWAYS, 0, 0);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INVERT);

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(pts[0].x, pts[0].y);

    for (const auto &p : pts)
        glVertex2f(p.x, p.y);

    glVertex2f(pts[0].x, pts[0].y);

    glEnd();

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