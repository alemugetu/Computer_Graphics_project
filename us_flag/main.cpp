#include <GL/glut.h>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const float FLAG_W = 1.9f;
const float FLAG_H = 1.0f;

const int WIN_W = 1000;
const int WIN_H = 600;

const float OX = 2.5f;
const float OY = 1.5f;

const int WAVE_COLS = 80;

const float WAVE_AMP = FLAG_H * 0.034f;
const float WAVE_FREQ = 2.5f;

float wavePhase = 0.0f;

inline float waveY(float x)
{
    float t = x / FLAG_W;
    return WAVE_AMP * sinf(wavePhase + t * 2.f * (float)M_PI * WAVE_FREQ);
}

void setColor(int r, int g, int b)
{
    glColor3f(r / 255.f, g / 255.f, b / 255.f);
}
void drawStar(float x, float y, float r)
{
    const float inner = r * 0.382f;
    const float startAngle = (float)(M_PI / 2.0);

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(x, y);

    for (int i = 0; i <= 10; ++i)
    {
        float angle = startAngle - i * (float)(M_PI / 5.0);

        float radius = (i % 2 == 0)
            ? r
            : inner;

        glVertex2f(
            x + radius * cosf(angle),
            y + radius * sinf(angle)
        );
    }

    glEnd();
}
void drawWaveQuadStrip(float x0, float x1, float yBot, float yTop)
{
    float dx = (x1 - x0) / WAVE_COLS;

    glBegin(GL_QUADS);

    for (int c = 0; c < WAVE_COLS; ++c)
    {
        float lx = x0 + c * dx;
        float rx = lx + dx;

        float dyL = waveY(lx);
        float dyR = waveY(rx);

        glVertex2f(lx, yBot + dyL);
        glVertex2f(rx, yBot + dyR);
        glVertex2f(rx, yTop + dyR);
        glVertex2f(lx, yTop + dyL);
    }

    glEnd();
}

void drawAnimatedBorder()
{
    const float x0 = 0.f;
    const float x1 = FLAG_W;

    const float y0 = 0.f;
    const float y1 = FLAG_H;

    const int N = WAVE_COLS;
    const float dx = (x1 - x0) / N;

    glColor3f(0.f, 0.f, 0.f);
    glLineWidth(1.8f);

    glBegin(GL_LINE_STRIP);

    for (int i = 0; i <= N; ++i)
    {
        float x = x0 + i * dx;
        glVertex2f(x, y0 + waveY(x));
    }

    glEnd();

    glBegin(GL_LINE_STRIP);

    for (int i = 0; i <= N; ++i)
    {
        float x = x0 + i * dx;
        glVertex2f(x, y1 + waveY(x));
    }

    glEnd();

    glBegin(GL_LINE_STRIP);

    for (int i = 0; i <= N; ++i)
    {
        float y = y0 + i * (y1 - y0) / N;
        glVertex2f(x0, y + waveY(x0));
    }

    glEnd();

    glBegin(GL_LINE_STRIP);

    for (int i = 0; i <= N; ++i)
    {
        float y = y0 + i * (y1 - y0) / N;
        glVertex2f(x1, y + waveY(x1));
    }

    glEnd();
}

void drawFlagpole(float flagX, float flagY)
{
    float poleX = flagX - 0.08f;

    float poleBottom = flagY - 1.0f;
    float poleTop = flagY + FLAG_H + 0.15f;

    float poleRadius = 0.03f;

    setColor(70, 74, 77);

    glBegin(GL_QUADS);

    glVertex2f(poleX - poleRadius, poleBottom);
    glVertex2f(poleX + poleRadius, poleBottom);
    glVertex2f(poleX + poleRadius, poleTop);
    glVertex2f(poleX - poleRadius, poleTop);

    glEnd();

    setColor(160, 164, 166);

    const float ballRadius = 0.07f;
    const int segments = 30;

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(poleX, poleTop);

    for (int i = 0; i <= segments; ++i)
    {
        float angle =
            2.0f * (float)M_PI * i / segments;

        glVertex2f(
            poleX + ballRadius * cosf(angle),
            poleTop + ballRadius * sinf(angle)
        );
    }

    glEnd();
}

void drawUSFlag()
{
    float fx = -0.95f;
    float fy = -0.5f;

    drawFlagpole(fx, fy);

    glPushMatrix();

    glTranslatef(fx, fy, 0.f);

    const float stripeH = FLAG_H / 13.f;
    const float cantonH = stripeH * 7.f;
    const float cantonW = FLAG_W * 0.40f;

    for (int i = 0; i < 13; ++i)
    {
        if (i % 2 == 0)
            setColor(191, 10, 48);
        else
            setColor(255, 255, 255);

        drawWaveQuadStrip(
            0.f,
            FLAG_W,
            i * stripeH,
            (i + 1) * stripeH);
    }

    setColor(0, 40, 104);

    drawWaveQuadStrip(
        0.f,
        cantonW,
        FLAG_H - cantonH,
        FLAG_H);

    setColor(255, 255, 255);

    const float xGap = cantonW / 12.f;
    const float yGap = cantonH / 10.f;

    const float starR = 0.022f;

    for (int row = 0; row < 9; ++row)
    {
        int cols = (row % 2 == 0) ? 6 : 5;

        float baseY = FLAG_H - yGap - row * yGap;

        for (int col = 0; col < cols; ++col)
        {
            float baseX;

            if (cols == 6)
                baseX = xGap + col * 2.f * xGap;
            else
                baseX = 2.f * xGap + col * 2.f * xGap;

            float sy = baseY + waveY(baseX);

            drawStar(baseX, sy, starR);
        }
    }

    drawAnimatedBorder();

    glPopMatrix();
}
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    drawUSFlag();

    glutSwapBuffers();
}

void timer(int)
{
    wavePhase -= 0.07f;

    glutPostRedisplay();

    glutTimerFunc(16, timer, 0);
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);

    glutInitWindowSize(WIN_W, WIN_H);

    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGB |
        GLUT_MULTISAMPLE);

    glutCreateWindow("Animated US Flag");

    glClearColor(0.96f, 0.96f, 0.96f, 1.f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(-OX, OX, -OY, OY);

    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glutDisplayFunc(display);

    glutTimerFunc(16, timer, 0);

    glutMainLoop();

    return 0;
}
