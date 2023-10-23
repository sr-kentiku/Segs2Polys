#ifdef _DEBUG
#pragma once

#include <stdint.h>
#include <vector>
#include <GL/freeglut.h>

// my
#include "Mathf.hpp"
#include "Vec3.hpp"

#if DEBUG_LOG
#include <iostream>
#include <fstream>
#endif

struct stData
{
    std::vector<Vec3> poly;
    GLfloat color[3];
};

std::vector<stData> gldata;

float angleX = 0.0f;
float angleY = 0.0f;
int prevX = 0;
int prevY = 0;
bool mousePressed = false;

GLfloat cameraX = 0.0f;
GLfloat cameraY = 0.0f;
GLfloat cameraZ = 4.0f;

void ClearGlData()
{
    gldata.clear();
}

void NormalizeGlData()
{
    Vec3 min;
    Vec3 max;
    Vec3 mid;
    Vec3 distv;
    double dist;

    min = gldata[0].poly[0];
    max = gldata[0].poly[0];
    for (size_t i = 0; i < gldata.size(); i++)
    {
        for (size_t j = 0; j < gldata[i].poly.size(); j++)
        {
            min = Vec3::min(min, gldata[i].poly[j]);
            max = Vec3::max(max, gldata[i].poly[j]);
        }
    }

    distv = Vec3(std::abs(max.x - min.x), std::abs(max.y - min.y), std::abs(max.z - min.z));

    mid = max - min;

    dist = distv.x;
    if (distv > distv.y)
        distv = distv.y;
    if (distv > distv.z)
        distv = distv.z;

    if (dist == 0)
        dist == 1;

    for (size_t i = 0; i < gldata.size(); i++)
        for (size_t j = 0; j < gldata[i].poly.size(); j++)
        {
            gldata[i].poly[j] -= mid;
            gldata[i].poly[j] /= dist;
        }
}

void AddGlData(std::vector<Vec3> poly, float color[3])
{
    stData d;
    d.poly = poly;
    for (size_t i = 0; i < 3; i++)
        d.color[i] = color[i];
    gldata.emplace_back(d);
}

void CalcVec3Normal(std::vector<Vec3> poly, float result[3])
{
    Vec3 n = Vec3::zero;
    for (size_t i = 0; i < poly.size(); i++)
        n += Vec3::cross(poly[i], poly[fmod(i + 1, poly.size())]);
    n.normalize();
    result[0] = n.x;
    result[1] = n.y;
    result[2] = n.z;
}

void DisplayGl()
{
    GLfloat normal[3];

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(cameraX, cameraY, cameraZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    GLfloat lightPosition[] = {cameraX, cameraY, cameraZ, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    for (size_t i = 0; i < gldata.size(); i++)
    {
        glBegin(GL_POLYGON);

        CalcVec3Normal(gldata[i].poly, normal);

        glNormal3fv(normal);
        glColor3fv(gldata[i].color);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, gldata[i].color);
        // glMaterialfv(GL_BACK, GL_DIFFUSE, gldata[i].color);
        for (size_t j = 0; j < gldata[i].poly.size(); j++)
            glVertex3f(gldata[i].poly[j].x, gldata[i].poly[j].y, gldata[i].poly[j].z);

        glEnd();
    }

    glutSwapBuffers();
}

void ReshapeGl(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)width / (float)height, 0.0001f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void MouseGl(int button, int state, int x, int y)
{
    if (button != GLUT_LEFT_BUTTON)
        return;

    mousePressed = state == GLUT_DOWN;
    if (mousePressed)
    {
        prevX = x;
        prevY = y;
    }
}

void MotionGl(int x, int y)
{
    if (!mousePressed)
        return;

    int deltaX = x - prevX;
    int deltaY = y - prevY;

    angleX += deltaY * 0.5;
    angleY += deltaX * 0.5;

    prevX = x;
    prevY = y;

    glutPostRedisplay();
}

void ShowGl(int_fast32_t width, int_fast32_t height, char windowtitle[])
{
    char *argv[] = {windowtitle, NULL};
    int argc = 1;

    GLfloat ambient[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow(windowtitle);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

    glutDisplayFunc(DisplayGl);
    glutReshapeFunc(ReshapeGl);
    glutMouseFunc(MouseGl);
    glutMotionFunc(MotionGl);

    glutMainLoop();
}
#endif
