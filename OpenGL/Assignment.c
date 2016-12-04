#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

// Window width and height
#define width 800
#define height 600

//
#define colorW 600
#define colorH 400


#define brightW 200
#define brightH 250

#define TW 100.0
#define TH 200.0
//gcc img2.c -o img2 glut32.lib -lopengl32 -lglu32
//http://widthw.basic4gl.net/wikifiles/2D%20Drawing%20in%20OpenGL.html
//http://widthw.lighthouse3d.com/tutorials/glut-tutorial/popup-menus/


//global variables for our windows id's
int mainWindow;
int palleteWindow = -1;
int brightnessWindow = -1;
int transWindow = -1;

GLfloat x1,x2,x3,y1,y2,y3;

float r,g,b,a = 1.0;


int polygon=0,amountOfPoints=0;
int polyArray[16]; 

void display(void)
{
	glClearColor(1.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //use this for clear functionality
	glPointSize(3.0);
	glColor3f(1.0,0.0,0.0);

	glFlush();
	return;
}

void SelectPoint(GLint button, GLint state, GLint x, GLint y){
	glPointSize(3.0);
	glColor4f(r,g,b,a);
	 glBegin(GL_POINTS);
		glVertex2f(x, height-y);
	 glEnd();
	 
	 glFlush();
	 return;
}

void CallTriangle(GLint button, GLint state, GLint x, GLint y)
{
	static int first=1;

	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		if (first == 1)
		{
			x1=x; 
			y1=height-y;
			first++;
		}
		else if(first == 2)
		{
			x2=x;
			y2=height-y;
			first++;
		}
		else
		{
			x3=x;
			y3=height-y;
			first=1;
			glColor4f(r,g,b,a);
			glBegin(GL_TRIANGLES);
			glVertex2f(x1,y1);
			glVertex2f(x2,y2);
			glVertex2f(x3,y3);
		
			glEnd();

			glFlush();
		}	
	}
	
	return;
}

void CallRectangle(GLint button, GLint state, GLint x, GLint y)
{
	static int first=1;
		printf("%d , %d , %d,%d \n",x ,y);
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		if (first)
		{
			x1=x; 
			y1=height-y;
		}
		else
		{
			x2=x;
			y2=height-y;
			printf("%d , %d , %d,%d \n",x2 ,y1,y2,x1);
			glColor4f(r,g,b,a);
			glBegin(GL_POLYGON);
				glVertex2f(x1,y1);
				glVertex2f(x1,y2);
				glVertex2f(x2,y2);
				glVertex2f(x2,y1);
			glEnd();
				
			glFlush();
			
		}
		first = !first;
	}

	return;
}

void callPolygon(GLint button, GLint state, GLint x, GLint y)
{
	static int first=0;

	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON && first!=amountOfPoints)
	{
		polyArray[first*2] = x;
		polyArray[first + 1] = y;
		printf("%d , %d , %f \n", x,y);
		first++;
	}
	else if(first==amountOfPoints)
	{
		glColor4f(r,g,b,a);
		glBegin(GL_POLYGON);
			for (int i = 0; i < amountOfPoints; ++i)
			{
		
				
				glVertex2f(width-polyArray[i * 2],height-polyArray[(i * 2)+1]);
				printf("%d , %d , %d, %d \n" ,polyArray[i*2]-(width/2))/(width/2,polyArray[i+1]);
			}
		
	glEnd();

	glFlush();
		first=0;
	}
	
	
	return;

}

void CallLine(GLint button, GLint state, GLint x, GLint y){
	static int first=1;

	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	{
		if (first == 1)
		{
			x1=x; 
			y1=height-y;
			first++;
		}
		else if(first == 2)
		{
			x2=x;
			y2=height-y;
			first = 1;
			glLineWidth(3.0);
			glColor4f(r,g,b,a);
			glBegin(GL_LINES);
				glVertex2f(x1, y1);
				glVertex2f(x2, y2);
				
			glEnd();
			glFlush();
			
		}
	}
	return;
}
void colorMouseHandler(GLint button, GLint state, GLint x, GLint y)
{
//first allocate an array of bytes we can store the pixel data
	GLubyte *pixelData = malloc(4 * 1 * 1);
	glReadPixels(x, glutGet(GLUT_WINDOW_HEIGHT) - y, 1,1, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);

	//as the data is saved as a byte we need to dived it by 255 to get it
	//in a value we can pass to color4f
	r = (float)pixelData[0] / 255.0;
	g = (float)pixelData[1] / 255.0;
	b = (float)pixelData[2] / 255.0;

	//if the brightness window is open we want to update the 
	//sliders to reflect the users choice
	if(brightnessWindow != -1)
	{	
		int cur = glutGetWindow();
		glutSetWindow(brightnessWindow);
		glutPostRedisplay();
		glutSetWindow(cur);
	}	
}
void drawPolly(int x1, int y1, int x2, int y2, float h, float w)
{
	//10,250 180,250
	glBegin(GL_POLYGON);
		//glColor4f(r,g,b,a);
		glVertex2f((x1-(w/2))/(w/2), (y1-(h/2))/(h/2));
		glVertex2f((x1-(w/2))/(w/2), (y2-(h/2))/(h/2));
		glVertex2f((x2-(w/2))/(w/2), (y2-(h/2))/(h/2));
		glVertex2f((x2-(w/2))/(w/2), (y1-(h/2))/(h/2));
	glEnd();

	glFlush();

	return;
}
//dislay function for the color picker
//just crate a square window and at each point draw
//set the color to be different as OpenGL nicely enough
//handles the blending of the colors and allows for a 
//color wheel type effect
void colorSquare()
{
	glClear(GL_COLOR_BUFFER_BIT);

	
	glBegin(GL_POLYGON);
		glColor4f(0,0,1.0,1.0);
		glVertex2f(-1,-1);
		glColor4f(0,1.0,0,1);
		glVertex2f(-1,1);
		glColor4f(1.0,1.0,0,1.0);
		glVertex2f(1,1);
		glColor4f(1.0,0,0,1.0);
		glVertex2f(1,-1);
	glEnd();

	glFlush();
}
void moveRGBSliders( GLint x , GLint y )
{	

	//if the mouse is in the slider
	if(x >= 20 && x <= 60 && y >= 40 && y <= 230)
	{	
		//calculate the color value for that slider relative to its slider position
		r = ((210 - ((float)y - 30.0))/200.0);
	

		//draw our slider again to remove the old value line
		glColor4f(0.5,0.5,0.5,1.0);
		drawPolly(20,8,60,210,brightH,brightW);

		//update the line to reflect the new position on the slider
		glBegin(GL_LINES);
			glColor4f(0.0,0.0,0.0,1.0);
			glVertex2f((20-(brightW/2))/(brightW/2),((210 *  r)-(brightH/2))/(brightH/2));
			glVertex2f((60-(brightW/2))/(brightW/2),((210 *  r)-(brightH/2))/(brightH/2));
		glEnd();
		glFlush();
	}
	if(x >= 80 && x <= 120 && y >= 40 && y <= 230)
	{	
		g = ((210 - ((float)y - 30.0)) /200.0);


		glColor4f(0.5,0.5,0.5,1.0);
		drawPolly(80,8,120,210,brightH,brightW);

		glBegin(GL_LINES);
			glColor4f(0.0,0.0,0.0,1.0);
			glVertex2f((80-(brightW/2))/(brightW/2),((210 *  g)-(brightH/2))/(brightH/2));
			glVertex2f((120-(brightW/2))/(brightW/2),((210 *  g)-(brightH/2))/(brightH/2));
		glEnd();
		glFlush();
	}
	if(x >= 140 && x <= 180 && y >= 40 && y <= 230)
	{	
		b = ((210 - ((float)y - 30.0))/200.0);

		glColor4f(0.5,0.5,0.5,1.0);
		drawPolly(140,8,180,210,brightH,brightW);

		glBegin(GL_LINES);
			glColor4f(0.0,0.0,0.0,1.0);
			glVertex2f((140-(brightW/2))/(brightW/2),((210 *  b)-(brightH/2))/(brightH/2));
			glVertex2f((180-(brightW/2))/(brightW/2),((210 *  b)-(brightH/2))/(brightH/2));
		glEnd();
		glFlush();
	}
	//set our color back to be the selected RGB values
	glColor4f(r,g,b,a);
	
	//draw our color reference window box so the user can see what
	//change moving the sliders made to the current color
	drawPolly(10,220,180,250,brightH,brightW);
	return;
}

void displayRGBSlider()
{
	glClearColor(1.0,1.0,1.0,1.0);

	glClear(GL_COLOR_BUFFER_BIT);
	glColor4f(0.0,0.0,0.0,1.0);
	glLineWidth(3.0);

	glColor4f(0.5,0.5,0.5,1.0);

	//draw 3 rectangeles as the base for our slider
	drawPolly(20,8,60,210,brightH,brightW);
	drawPolly(80,8,120,210,brightH,brightW);
	drawPolly(140,8,180,210,brightH,brightW);
	glColor4f(0.0,0.0,0.0,1.0);

	//draw a line to display the current set value by calcuating the y value relative to the R,G or B value
	glBegin(GL_LINES);
		glVertex2f((20-(brightW/2))/(brightW/2),((210 * r)-(brightH/2))/(brightH/2));
		glVertex2f((60-(brightW/2))/(brightW/2),((210 * r)-(brightH/2))/(brightH/2));
	glEnd();

	glBegin(GL_LINES);
		glVertex2f((80-(brightW/2))/(brightW/2),((210 * g)-(brightH/2))/(brightH/2));
		glVertex2f((120-(brightW/2))/(brightW/2),((210 * g)-(brightH/2))/(brightH/2));
	glEnd();

	glBegin(GL_LINES);
		glVertex2f((140-(brightW/2))/(brightW/2),((210 * b)-(brightH/2))/(brightH/2));
		glVertex2f((180-(brightW/2))/(brightW/2),((210 * b)-(brightH/2))/(brightH/2));
	glEnd();

	glColor4f(r,g,b,a);
	drawPolly(10,220,180,250,brightH,brightW);

	glFlush();
	return;
}

void selectColor(int item)
{
	switch(item)
	{
		case 0:
		{
			r = 0.0,g = 1.0,b = 0.0;
		} break;
		case 1: 
		{
			r = 1.0,g = 0.0,b = 0.0;
		} break;
		case 2:
		{
			r = 0.0,g = 0.0,b = 0.0;
		} break;
		case 3:
		{
			r = 0.0,g = 0.0,b = 1.0;
		} break;
		case 4:
		{
			r = 0.90 ,g =  0.91,b = 0.98;
		} break;
		case 5:
		{
			r = 1.0,g = 1.0,b = 0.0;
		} break;
		case 6:
		{
			r = 0.258824,g =  0.435294,b =0.258824;
		} break;
		case 7:
		{
			r = 1.0,g = 0.5,b = 0.0;
		} break;
		default: {} break;
	}
	if(brightnessWindow != -1)
	{	
		glutSetWindow(brightnessWindow);
		glutPostRedisplay();
		glutSetWindow(mainWindow);
	}	
	return;
}


void SelectShape(int option) {

	switch (option) {
		case 1 :
			glutMouseFunc(SelectPoint);
			break;
		case 2 :
			glutMouseFunc(CallLine);
			break;
		case 3 :
			glutMouseFunc(CallTriangle);
			 break;
		case 4 :
			glutMouseFunc(CallRectangle);
			break;
		default:
			glutMouseFunc(SelectPoint);
		break;
	}
	return;
}

void setupPolygon(int pol)
{
	amountOfPoints=pol;
	glutMouseFunc(callPolygon);
}
void displayTransSlider()
{
	glClearColor(1.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glLineWidth(3.0);

	glColor4f(0.5,0.5,0.5,1.0);
	glBegin(GL_POLYGON);
		glVertex2f(1,-1);
		glVertex2f(1,1);
		glVertex2f(-1,1);
		glVertex2f(-1,-1);
		
	glEnd();
	
	glColor4f(0.0,0.0,0.0,1.0);
	glBegin(GL_LINES);
		glVertex2f((1-(TW/2))/(TW/2),((201 - (200 * a))-(TH/2))/(TH/2));
		glVertex2f((100-(TW/2))/(TW/2),((201 - (200 * a))-(TH/2))/(TH/2));
	glEnd();

	glFlush();

	return;
}
//function to move our transparancy slider
void moveTransSlider( GLint x , GLint y )
{	

	//simply calculate our alpha value relative to the slider position
	//and then just call the display function again
	a = (float)y / 200.0;

	displayTransSlider();

	//printf("%d , %d , %f \n", x, y,a);
}	

void menu(int item)
{
	switch(item)
	{	
		//if the user choose to clear the window just call the redisplay function
		case 0:
		{
			glutPostRedisplay();
		}break;
		case 1:
		{
			if(brightnessWindow == -1)
			{
				glutInitWindowSize(brightW, brightH);
				glutInitWindowPosition(500 +(brightW+400),200);
				brightnessWindow = glutCreateWindow("Brightness");

				glutDisplayFunc(displayRGBSlider);
				glutMotionFunc(moveRGBSliders);
			}
			else
			{
				glutDestroyWindow(brightnessWindow);
				brightnessWindow = -1;
			}
		}break;
		case 2:
		{
			//if the color picker is not open 
			if(palleteWindow == 1)
			{
				glutDestroyWindow(palleteWindow);
				palleteWindow = -1;
			}
			else
			{
		
				//create the window 
				glutInitWindowSize(colorW,colorH);
				//we want to position it to the left of our draw window
				glutInitWindowPosition(500 - (colorW+200),100);
				palleteWindow = glutCreateWindow("color picker");

				//assigne the display function and the color picker mouse fucntion
				glutDisplayFunc(colorSquare);
				glutMouseFunc(colorMouseHandler);
			}
		

		}break;
		case 3:
		{

			if(transWindow == -1)
			{
				glutInitWindowSize(brightW, brightH);
				//the left side is getting clutered so we will put it on the right side
				glutInitWindowPosition(500 + brightW + 20,100);
				transWindow = glutCreateWindow("Transparency");

				glutDisplayFunc(displayTransSlider);
				glutMotionFunc(moveTransSlider);
			}
			else
			{
				glutDestroyWindow(transWindow);
				transWindow = -1;
			}
		}break;
		default:{}break;
	}
	return;
}

void MyReshape(GLsizei w, GLsizei h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0,(GLfloat)w,0.0,(GLfloat)h);
	glViewport(0,0,width,height);
	
	return;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(width,height);
	glutInitWindowPosition(500,200);
	glutCreateWindow("mouse");
	glutDisplayFunc(display);
	glutReshapeFunc(MyReshape);
	
	

	// create the menu and
	// tell glut that "processMenuEvents" will
	// handle the events
	int colorSubMenu = glutCreateMenu(selectColor);
	glutAddMenuEntry("Green",0);
	glutAddMenuEntry("Red",1);
	glutAddMenuEntry("Black",2);
	glutAddMenuEntry("Blue",3);
	glutAddMenuEntry("Silver",4);
	glutAddMenuEntry("Yellow",5);
	glutAddMenuEntry("MediumSeaGreen  ",6);
	glutAddMenuEntry("Orange",7);
	
	int polyMenu = glutCreateMenu(setupPolygon);
	glutAddMenuEntry(" 3 ",3);
	glutAddMenuEntry(" 5 ",5);
	glutAddMenuEntry(" 6 ",6);
	glutAddMenuEntry(" 7 ",7);
	glutAddMenuEntry(" 8 ",8);
	
	//add entries to our menu
	int shapeMenu=glutCreateMenu(SelectShape);
	glutAddMenuEntry("Point",1);
	glutAddMenuEntry("Line",2);
	glutAddMenuEntry("Triangle",3);
	glutAddMenuEntry("Rectangle",4);
	glutAddSubMenu("Polygon",polyMenu);
	//glutAddMenuEntry("Polygon",5);
	
	int mainMenu = glutCreateMenu(menu);
	glutAddSubMenu("Shape",shapeMenu);
	glutAddSubMenu("Color",colorSubMenu);
	glutAddMenuEntry("Clear",0);
	glutAddMenuEntry("Brightness",1);
	glutAddMenuEntry("Color Pallete",2);
	glutAddMenuEntry("Transparency",3);
	//numbered for menu method
	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutMainLoop();
}