#include <GL/glut.h>
#include <cmath>

const float FLAG_W = 1.9f;
const float FLAG_H = 1.0f;

void setColor(int r, int g, int b)
{
    glColor3f(r / 255.f, g / 255.f, b / 255.f);
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
