#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <iostream>

#include "kernel.h"
#include "Mouse.h"
#include "Panel.h"

#include <GL/glew.h>
#include <GL/freeglut.h>


int winw = 640;
int winh = 480;

Panel theWindow;

float rotate_x = 45.f;

Mouse theMouse;

void Init()
{
	glEnable(GL_LIGHT0);

	runCUDA();
}

void SetUpWindow()
{
	int leftPanelWidth(100);
	theWindow.m_rectInt_abs = RectInt(100, 100, winw, winh);
	theWindow.RectIntAbsToRectFloatAbs();

	theWindow.CreateSubPanel(RectInt(0, 0, 100, winh), Panel::DEF_ABS, "CDV", Color(Color::LIGHT_GRAY));
	theWindow.m_subPanels[0]->CreateButton(RectFloat(-0.9f, 0.f, 1.8f, 0.2f), Panel::DEF_REL, "Button1", Color(Color::RED));
	theWindow.m_subPanels[0]->CreateButton(RectFloat(-0.9f,-0.5f, 1.8f, 0.2f), Panel::DEF_REL, "Button2", Color(Color::DARK_GRAY));

}

/*----------------------------------------------------------------------------------------
 *	This function will be used to draw the 3D scene
 */
void Draw3D()
{
	gluLookAt(0,1,5,0,0,0,0,1,0);
	glutSolidTeapot(1);
}

void DrawRectangle(RectInt rect)
{
	glBegin(GL_QUADS);
		glVertex2f(rect.m_x         ,rect.m_y+rect.m_h);
		glVertex2f(rect.m_x         ,rect.m_y         );
		glVertex2f(rect.m_x+rect.m_w,rect.m_y         );
		glVertex2f(rect.m_x+rect.m_w,rect.m_y+rect.m_h);
	glEnd();
}


void Draw2D()
{
	theWindow.m_subPanels[0]->Draw();
	theWindow.m_subPanels[0]->m_buttons[0]->Draw();
	theWindow.m_subPanels[0]->m_buttons[1]->Draw();
}


void MouseButton(int button, int state, int x, int y)
{
	theMouse.Update(x, y, button, state);

}

void MouseMotion(int x, int y)
{
	int dx, dy;

	theMouse.GetChange(x, y);

	theMouse.Update(x, y);

}



void Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	/*
	 *	Set perspective viewing transformation
	 */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45,(winh==0)?(1):((float)winw/winh),1,100);
	glOrtho(-10,10,-10,10,0,100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(rotate_x,1.f,0.f,0.f);

	/*
	 *	Draw the 3D elements in the scene
	 */
	Draw3D();

	/*
	 *	Disable depth test and lighting for 2D elements
	 */
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	/*
	 *	Set the orthographic viewing transformation
	 */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(0,winw,winh,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*
	 *	Draw the 2D overlay
	 */
	Draw2D();

	/*
	 *	Bring the back buffer to the front and vice-versa.
	 */
	glutSwapBuffers();

}

int main(int argc,char **argv)
{

	SetUpWindow();

	glutInit(&argc,argv);


	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowSize(winw,winh);
	glutInitWindowPosition(200,100);
	glutCreateWindow("03 - Mouse Motion");

	glutDisplayFunc(Draw);
//	glutReshapeFunc(Resize);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
//	glutPassiveMotionFunc(MousePassiveMotion);

	Init();

	glutMainLoop();

	return 0;
}