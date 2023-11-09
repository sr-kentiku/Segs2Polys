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
    GLfloat color[4];
    GLfloat normal[3];
    bool isEmission;
};

std::vector<stData> gldata;

bool mousePressed = false;
Vec2 prev = Vec2::zero;
Vec2 cameraAngle = Vec2::zero;
Vec3 cameraPos = Vec3(0.0, 0.0, 4.0);

#define GL_ZOOM_SPEED   0.1
#define GL_MOVE_SPEED   0.1

void ClearGlData()
{
    gldata.clear();
}

Vec3 CalcVec3Normal(std::vector<Vec3> poly)
{
    Vec3 n = Vec3::zero;
    for (size_t i = 0; i < poly.size(); i++)
        n += Vec3::cross(poly[i], poly[fmod(i + 1, poly.size())]);
    return Vec3::normalize(n);
}

void NormalizeGlData()
{
    Vec3 min;
    Vec3 max;
    Vec3 mid;
    Vec3 distv;
    double dist;
    Vec3 n;

    min = gldata[0].poly[0];
    max = gldata[0].poly[0];
    for (size_t i = 0; i < gldata.size(); i++)
        for (size_t j = 0; j < gldata[i].poly.size(); j++)
        {
            min = Vec3::min(min, gldata[i].poly[j]);
            max = Vec3::max(max, gldata[i].poly[j]);
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
            gldata[i].poly[j] = gldata[i].poly[j].glgrid();
        }

    for (size_t i = 0; i < gldata.size(); i++)
    {
        if (gldata[i].isEmission)
            continue;
        n = CalcVec3Normal(gldata[i].poly);
        for (size_t j = 0; j < 3; j++)
            gldata[i].normal[j] = (GLfloat)n[j];
    }

    for (size_t i = 0; i < gldata.size(); i++)
    {
        std::cout << i << std::endl;
        std::cout << "\tcolor      : [" << std::to_string(gldata[i].color[0]) << ", " << std::to_string(gldata[i].color[1]) << ", " << std::to_string(gldata[i].color[2]) << ", " << std::to_string(gldata[i].color[3]) << "]" << std::endl;
        if (gldata[i].isEmission)
        {
            std::cout << "\tisEmission : " << "true" << std::endl;
            std::cout << "\tnormal     : [0, 0, 0]" << std::endl;
        }
        else
        {
            std::cout << "\tisEmission : " << "false" << std::endl;
            std::cout << "\tnormal     : [" << std::to_string(gldata[i].normal[0]) << ", " << std::to_string(gldata[i].normal[1]) << ", " << std::to_string(gldata[i].normal[2]) << "]" << std::endl;
        }
        std::cout << "\tpoly       : [" << std::endl;
        for (size_t j = 0; j < gldata[i].poly.size(); j++)
            std::cout << "\t\t" << gldata[i].poly[j].ToString() << "," << std::endl;
        std::cout << "\t]" << std::endl;
    }
}

void AddGlData(std::vector<Vec3> poly, GLfloat color[], bool isEmission = false)
{
    stData d;
    d.poly = poly;
    for (size_t i = 0; i < 4; i++)
        d.color[i] = color[i];
    d.isEmission = isEmission;
    gldata.emplace_back(d);
}

void OnRenderScene()
{
    GLfloat black[] = {0, 0, 0};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt((GLfloat)cameraPos.x, (GLfloat)cameraPos.y, (GLfloat)cameraPos.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    GLfloat lightPosition[] = {(GLfloat)cameraPos.x, (GLfloat)cameraPos.y, (GLfloat)cameraPos.z, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glRotatef((GLfloat)cameraAngle.x, 1.0f, 0.0f, 0.0f);
    glRotatef((GLfloat)cameraAngle.y, 0.0f, 1.0f, 0.0f);

    for (size_t i = 0; i < gldata.size(); i++)
    {
        if (gldata[i].isEmission)
            glEnable(GL_EMISSION);

        glColor4fv(gldata[i].color);
        glBegin(GL_POLYGON);
            glNormal3fv(gldata[i].normal);

            glMaterialfv(GL_FRONT, GL_DIFFUSE, gldata[i].color);
            glMaterialfv(GL_BACK, GL_DIFFUSE, gldata[i].color);

            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, gldata[i].isEmission ? gldata[i].color : black);
            
            for (size_t j = 0; j < gldata[i].poly.size(); j++)
                glVertex3f(gldata[i].poly[j].x, gldata[i].poly[j].y, gldata[i].poly[j].z);
        glEnd();

        if (gldata[i].isEmission)
            glDisable(GL_EMISSION);
    }

    glutSwapBuffers();
}

void OnResizeScene(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)width / (float)height, 0.0001f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void OnMouseMove(int x, int y)
{
    Vec2 mouse = Vec2(y, x);
    Vec2 delta = mouse - prev;
    prev = mouse;
    if (!mousePressed)
        return;
    cameraAngle += delta * 0.5;
    cameraAngle.x = Mathf::clamp(cameraAngle.x, -90, 90);
    glutPostRedisplay();
}

void OnMouseClick(int button, int state, int x, int y)
{
    mousePressed = button == GLUT_LEFT_BUTTON && state == GLUT_DOWN;
}

void OnMouseWheel(int wheel, int direction, int x, int y)
{
    cameraPos.z += direction < 0 ? GL_ZOOM_SPEED : -GL_ZOOM_SPEED;
    cameraPos.z = Mathf::clamp(cameraPos.z, 0, 500);
    glutPostRedisplay();
}

void ShowGl(int_fast32_t width, int_fast32_t height, char windowtitle[])
{
    char *argv[] = {windowtitle, NULL};
    int argc = 1;

    GLfloat emission[] = {1.0f, 0.0f, 0.0f, 1.0f};
    GLfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat specular[] = {0.0f, 0.0f, 0.0f, 1.0f};

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow(windowtitle);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_EMISSION, emission);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    glutDisplayFunc(OnRenderScene);
    glutReshapeFunc(OnResizeScene);

    glutMouseFunc(OnMouseClick);
    glutMouseWheelFunc(OnMouseWheel);
    glutMotionFunc(OnMouseMove);
    glutPassiveMotionFunc(OnMouseMove);

    glutMainLoop();
}
#endif
