#include<bits/stdc++.h>
#include <windows.h>
#include <glut.h>
#define pi (2*acos(0.0))
using namespace  std;

struct point
{
    double x,y,z;
};

bool paused = false;
double box_side;
double circ_radius;
double bubble_radius;
const int no_of_bubbles = 5;
struct point bubble_pos[no_of_bubbles];
struct point bubble_speed[no_of_bubbles];
struct point temp_speed[no_of_bubbles];

int one_one=0;
int inside[no_of_bubbles];
int bubbles_inside[no_of_bubbles][no_of_bubbles];

void drawSquare(double a)
{
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINE_LOOP);{
        glVertex3f( 0, 0,0);
        glVertex3f( a,0,0);
        glVertex3f(a,a,0);
        glVertex3f(0, a,0);
    }glEnd();
}
void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    //glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}
double distance_btn_points(struct point a, struct point b)
{
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)+(a.z-b.z)*(a.z-b.z));
}
bool is_inside_circle(struct point a)
{
    struct point circle;
    circle.x = box_side/2.0;
    circle.y = box_side/2.0;
    circle.z = 0;
    double dist = distance_btn_points(a, circle);
    //cout<<dist<<endl;
    return dist <= circ_radius-bubble_radius;
}
void reflect_inside_circle()
{
    struct point circle;
    circle.x = box_side/2.0;
    circle.y = box_side/2.0;
    circle.z = 0;

    for(int i=0;i<no_of_bubbles;i++)
    {
        if(inside[i]!=1)
        {
            if(is_inside_circle(bubble_pos[i]))
            {
                inside[i]=1;
                if(distance_btn_points(circle,bubble_pos[i])+bubble_radius>=circ_radius)
                {
                    struct point n;
                    n.x=circle.x-bubble_pos[i].x;
                    n.y=circle.y-bubble_pos[i].y;
                    n.z=0;
                    double lnl= (n.x*n.x)+(n.y*n.y)+(n.z*n.z);
                    double num = (2*(n.x*bubble_speed[i].x+n.y*bubble_speed[i].y))/lnl;
                    n.x *= num;
                    n.y *= num;
                    bubble_speed[i].x -= n.x;
                    bubble_speed[i].y -= n.y;
                }
            }
        }
        else
        {
            if(distance_btn_points(circle,bubble_pos[i])+bubble_radius>=circ_radius)
            {
                struct point n;
                n.x=circle.x-bubble_pos[i].x;
                n.y=circle.y-bubble_pos[i].y;
                n.z=0;
                double lnl= (n.x*n.x)+(n.y*n.y)+(n.z*n.z);
                double num = (2*(n.x*bubble_speed[i].x+n.y*bubble_speed[i].y))/lnl;
                n.x *= num;
                n.y *= num;
                bubble_speed[i].x -= n.x;
                bubble_speed[i].y -= n.y;
            }

        }
    }
}
void reflect_bubbles()
{
    for(int i=0;i<no_of_bubbles-1;i++)
    {
        for(int j=i+1;j<no_of_bubbles;j++)
        {
            if(inside[i]==1 && inside[j]==1)
            {
                if(bubbles_inside[i][j]!=1 || bubbles_inside[j][i]!=1)
                {
                    if(distance_btn_points(bubble_pos[i],bubble_pos[j])>2*bubble_radius)
                    {
                        bubbles_inside[i][j]=1;
                        bubbles_inside[j][i]=1;
                    }
                }
                else
                {
                    if(distance_btn_points(bubble_pos[i],bubble_pos[j])<=2*bubble_radius)
                    {
                        struct point n,m;
                        n.x=bubble_pos[i].x-bubble_pos[j].x;
                        n.y=bubble_pos[i].y-bubble_pos[j].y;
                        n.z=0;
                        double lnl= (n.x*n.x)+(n.y*n.y)+(n.z*n.z);
                        double num = (2*(n.x*bubble_speed[i].x+n.y*bubble_speed[i].y))/lnl;
                        n.x *= num;
                        n.y *= num;
                        bubble_speed[i].x -= n.x;
                        bubble_speed[i].y -= n.y;
                        m.x=bubble_pos[j].x-bubble_pos[i].x;
                        m.y=bubble_pos[j].y-bubble_pos[i].y;
                        m.z=0;
                        lnl= (m.x*m.x)+(m.y*m.y)+(m.z*m.z);
                        num = (2*(m.x*bubble_speed[j].x+m.y*bubble_speed[j].y))/lnl;
                        m.x *= num;
                        m.y *= num;
                        bubble_speed[j].x -= m.x;
                        bubble_speed[j].y -= m.y;
                    }
                }
            }
        }
    }
}
void drawSS()
{
    drawSquare(box_side);
    glPushMatrix();
    glColor3f(1,0,0);
    glTranslatef(box_side/2,box_side/2,0);
    drawCircle(circ_radius,50);
    glPopMatrix();

    glColor3f(0.9,0.7,0.7);

    for(int i=0;i<no_of_bubbles;i++)
    {
        glPushMatrix();
        glTranslatef(bubble_pos[i].x,bubble_pos[i].y,0);
        if(one_one>(i+1)*5000)
            drawCircle(bubble_radius,50);
        glPopMatrix();
    }
}
void idle(){

    if(one_one<=25050)
        one_one++;
    for(int i=0;i<no_of_bubbles;i++)
    {
        if(one_one>(i+1)*5000)
        {
            if(bubble_pos[i].x+bubble_radius>=box_side || bubble_pos[i].x-bubble_radius<=0.0)
            {
                bubble_speed[i].x *= (-1);
            }
            if(bubble_pos[i].y+bubble_radius>=box_side || bubble_pos[i].y-bubble_radius<=0.0)
            {
                bubble_speed[i].y *= (-1);
            }
            bubble_pos[i].x = bubble_pos[i].x + bubble_speed[i].x/100.0;
            bubble_pos[i].y = bubble_pos[i].y + bubble_speed[i].y/100.0;
        }
    }
    reflect_inside_circle();
    reflect_bubbles();
    glutPostRedisplay();
}
void keyboardListener(unsigned char key, int x,int y){
    switch(key){

        case 'p':
            paused = !paused;
            if(paused)
            {
                for(int i=0;i<no_of_bubbles;i++)
                {
                    temp_speed[i]=bubble_speed[i];
                    bubble_speed[i].x =0.0;
                    bubble_speed[i].y =0.0;
                }
            }
            else
            {
                for(int i=0;i<no_of_bubbles;i++)
                {
                    bubble_speed[i]=temp_speed[i];
                }
            }
            break;
        default:
            break;
    }
}
void specialKeyListener(int key, int x, int y){
    double inc = 0.4, dec= 0.2;
    double high = 10.0, low = 0.2;
    switch(key){
        case GLUT_KEY_DOWN:

            for(int i=0;i<no_of_bubbles;i++)
            {
                if(bubble_speed[i].x < 0.0)
                {
                    if(-1*bubble_speed[i].x-dec>low)
                        bubble_speed[i].x+=dec;
                }
                else{
                    if(bubble_speed[i].x-dec>low)
                        bubble_speed[i].x-=dec;
                }
                if(bubble_speed[i].y <0.0)
                {
                    if(-1*bubble_speed[i].y-dec>low)
                        bubble_speed[i].y+=dec;
                }
                else{
                    if(bubble_speed[i].y-dec>low)
                        bubble_speed[i].y-=dec;
                }
            }
            break;
        case GLUT_KEY_UP:

            for(int i=0;i<no_of_bubbles;i++)
            {
                if(bubble_speed[i].x < 0.0)
                {
                    if(-1*bubble_speed[i].x+inc<high)
                        bubble_speed[i].x-=inc;
                }
                else{
                    if(bubble_speed[i].x+inc<high)
                        bubble_speed[i].x+=inc;
                }
                if(bubble_speed[i].y < 0.0)
                {
                    if(-1*bubble_speed[i].y+inc<high)
                        bubble_speed[i].y-=inc;
                }
                else{
                    if(bubble_speed[i].y+inc<high)
                        bubble_speed[i].y+=inc;
                }
            }
            break;
        default:
            break;
    }
}
void display(){

    //clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);	//color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /********************
    / set-up camera here
    ********************/
    //load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);

    //initialize the matrix
    glLoadIdentity();

    //now give three info
    //1. where is the camera (viewer)?
    //2. where is the camera looking?
    //3. Which direction is the camera's UP direction?

    //gluLookAt(100,100,100,	0,0,0,	0,0,1);
    //gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
    gluLookAt(box_side/2,box_side/2,200,	box_side/2,box_side/2,0,	0,1,0);

    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    //add objects

    drawSS();

    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}
void init(){
    //codes for initialization
    box_side = 260;
    circ_radius = 88;
    bubble_radius = 12.0;

    for(int i=0;i<no_of_bubbles;i++)
    {
        bubble_pos[i].x = 12.1;
        bubble_pos[i].y = 12.1;
        bubble_pos[i].z = 0;

        float s = (float) rand() /RAND_MAX;
        s == 0.0 ? s = 0.234567 : s;
        bubble_speed[i].x = s;
        s = (float) rand() /RAND_MAX;
        s == 0.0 ? s = 0.856543 : s;
        bubble_speed[i].y = s;
        bubble_speed[i].z =0;

        cout<<bubble_speed[i].x << " "<<bubble_speed[i].y<< endl;
    }

    //clear the screen
    glClearColor(0,0,0,0);

    /************************
    / set-up projection here
    ************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    //initialize the matrix
    glLoadIdentity();

    //give PERSPECTIVE parameters
    gluPerspective(80,	1,	1,	1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}
int main(int argc, char **argv){

    srand(time(nullptr));
    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("My OpenGL Program 2");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutIdleFunc(idle);
    glutMainLoop();		//The main loop of OpenGL

    return 0;
}