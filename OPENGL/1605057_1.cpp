#include<bits/stdc++.h>
#include <windows.h>
#include <glut.h>
#define pi (2*acos(0.0))
using namespace  std;

int drawaxes;
double qw, er, as, df;
struct point
{
    double x,y,z;
};

struct point cam_pos, u, r, l ;
double move_cam;
double rotate_cam;
double sphere_radius, half_sphere_radius, cylinder_height;
struct point points[101];
int no_of_shots;
double dist;

struct point cross_product(struct point a, struct point b)
{
    struct point res;
    res.x = a.y * b.z - a.z * b.y;
    res.y = a.z * b.x - a.x * b.z;
    res.z = a.x * b.y - a.y * b.x;
    return res;
}
struct point new_vector(struct point a, struct point temp, double angle)
{
    struct point res;
    double alpha = cos(angle * (pi / 180));
    double beta = sin(angle * (pi / 180));
    res.x = alpha * a.x + beta * temp.x;
    res.y = alpha * a.y + beta * temp.y;
    res.z = alpha * a.z + beta * temp.z;
    return res;
}
void drawAxes()
{
    if(drawaxes==1)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);{
            glVertex3f( 1000,0,0);
            glVertex3f(-1000,0,0);

            glVertex3f(0,-1000,0);
            glVertex3f(0, 1000,0);

            glVertex3f(0,0, 1000);
            glVertex3f(0,0,-1000);
        }glEnd();
    }
}
void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
    glBegin(GL_QUADS);{
        glVertex3f( a, 0,a);
        glVertex3f( a,0,-a);
        glVertex3f(-a,0,-a);
        glVertex3f(-a, 0,a);
    }glEnd();
}
struct point gunShot(){
    struct point a,b,c, temp1, temp2;
    a.x=1;
    a.y=0;
    a.z=0;
    b.x=0;
    b.y=1;
    b.z=0;
    c.x=0;
    c.y=0;
    c.z=1;

    temp1 = cross_product(b,c);
    b = new_vector(b,temp1,-qw);
    temp2 = cross_product(a,c);
    a = new_vector(a,temp2,-qw);
    temp1 = cross_product(b,a);
    b = new_vector(b,temp1,-er);

    struct point p1, p2;
    p1.x = sphere_radius*b.x;
    p1.y = sphere_radius*b.y;
    p1.z = sphere_radius*b.z;

    temp1 = cross_product(b,a);
    b = new_vector(b,temp1,-as);
    p2.x = p1.x+b.x;
    p2.y = p1.y+b.y;
    p2.z = p1.z+b.z;

    double X,Y,Z;
    Y = sphere_radius+half_sphere_radius+cylinder_height+dist;
    X = (((Y-p1.y)*(p2.x-p1.x))/(p2.y-p1.y))+p1.x;
    Z = (((Y-p1.y)*(p2.z-p1.z))/(p2.y-p1.y))+p1.z;
    struct point res;
    res.x=X;
    res.y=0;
    res.z=Z;
    return res;
}
void drawSphere(double radius,int slices,int stacks)
{
    struct point points[100][100];
    int i,j;
    double h,r;
    //generate points
    for(i=0;i<=stacks;i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        r=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0;j<=slices;j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);;
            points[i][j].y=-1*h;
            points[i][j].z=r*sin(((double)j/(double)slices)*2*pi);
        }
    }
    //draw quads using generated points
    for(i=0;i<stacks;i++)
    {
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);

        for(j=0;j<slices;j++)
        {
            if(j%2!=1)
            {
                glColor3f(1.0,1.0,1.0);
            }
            else
            {
                glColor3f(0,0,0);
            }

            glBegin(GL_QUADS);{
                //upper hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,-points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,-points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,-points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,-points[i+1][j].y,points[i+1][j].z);
            }glEnd();
        }
    }
}
void drawCurvature(double radius,int slices,int stacks)
{
    struct point points[100][100];
    int i,j;
    double h,r;
    //generate points
    for(i=0;i<=stacks;i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        r=radius*cos(((double)i/(double)stacks)*(pi/2));
        r = 2*radius - r;
        for(j=0;j<=slices;j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);;
            points[i][j].y=-1*h;
            points[i][j].z=r*sin(((double)j/(double)slices)*2*pi);
        }
    }
    //draw quads using generated points
    for(i=0;i<stacks;i++)
    {
        for(j=0;j<slices;j++)
        {
            if(j%2!=1)
            {
                glColor3f(1.0,1.0,1.0);
            }
            else
            {
                glColor3f(0,0,0);
            }
            glBegin(GL_QUADS);{
                glVertex3f(points[i][j].x,-points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,-points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,-points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,-points[i+1][j].y,points[i+1][j].z);
            }glEnd();
        }
    }
}
void drawCylinder(double radius,int slices,int stacks)
{
    struct point points[500][100];
    int i,j;
    double h,r;
    r=radius;
    //generate points
    for(i=0;i<=stacks;i++)
    {
        h=i;
        for(j=0;j<=slices;j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);;
            points[i][j].y=-1*h;
            points[i][j].z=r*sin(((double)j/(double)slices)*2*pi);
        }
    }
    //draw quads using generated points
    for(i=0;i<stacks;i++)
    {
        for(j=0;j<slices;j++)
        {
            if(j%2!=1)
            {
                glColor3f(1.0,1.0,1.0);
            }
            else
            {
                glColor3f(0,0,0);
            }

            glBegin(GL_QUADS);{
                //upper hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
//                glVertex3f(points[i][j].x,-points[i][j].y,points[i][j].z);
//                glVertex3f(points[i][j+1].x,-points[i][j+1].y,points[i][j+1].z);
//                glVertex3f(points[i+1][j+1].x,-points[i+1][j+1].y,points[i+1][j+1].z);
//                glVertex3f(points[i+1][j].x,-points[i+1][j].y,points[i+1][j].z);
            }glEnd();
        }
    }
}
void upperHemisphere(double radius,int slices,int stacks)
{
    struct point points[100][100];
    int i,j;
    double h,r;
    //generate points
    for(i=0;i<=stacks;i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        r=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0;j<=slices;j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);;
            points[i][j].y=-1*h;
            points[i][j].z=r*sin(((double)j/(double)slices)*2*pi);
        }
    }
    //draw quads using generated points
    for(i=0;i<stacks;i++)
    {
        for(j=0;j<slices;j++)
        {
            if(j%2!=1)
            {
                glColor3f(1.0,1.0,1.0);
            }
            else
            {
                glColor3f(0,0,0);
            }
            glBegin(GL_QUADS);{
                //upper hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
            }glEnd();
        }
    }
}
void lowerHemisphere(double radius,int slices,int stacks)
{
    struct point points[100][100];
    int i,j;
    double h,r;
    //generate points
    for(i=0;i<=stacks;i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        r=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0;j<=slices;j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);;
            points[i][j].y=-1*h;
            points[i][j].z=r*sin(((double)j/(double)slices)*2*pi);
        }
    }
    //draw quads using generated points
    for(i=0;i<stacks;i++)
    {
        for(j=0;j<slices;j++)
        {
            if(j%2!=1)
            {
                glColor3f(1.0,1.0,1.0);
            }
            else
            {
                glColor3f(0,0,0);
            }
            glBegin(GL_QUADS);{
                //lower hemisphere
                glVertex3f(points[i][j].x,-points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,-points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,-points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,-points[i+1][j].y,points[i+1][j].z);
            }glEnd();
        }
    }
}
void drawSS()
{
    glPushMatrix();
    drawAxes();
    glRotatef(qw,0,0,1);

    upperHemisphere(sphere_radius,90,40);
    glRotatef(er,1,0,0);
    lowerHemisphere(sphere_radius,90,40);
    glTranslatef(0,sphere_radius,0);
    glRotatef(as,1,0,0);
    glRotatef(df,0,1,0);
    glTranslatef(0,half_sphere_radius,0);
    upperHemisphere(half_sphere_radius,90,40);
    glTranslatef(0,(int)cylinder_height,0);
    drawCylinder(half_sphere_radius,90,(int)cylinder_height);
    drawCurvature(half_sphere_radius,90,40);

    glPopMatrix();
    glTranslatef(0,sphere_radius+half_sphere_radius+(int)cylinder_height+dist,0);
    glColor3f(0.6,0.6,0.6);
    drawSquare(140);

    glTranslatef(0,-2,0);
    glColor3f(1,0,0);

    for(int i=0;i<no_of_shots;i++)
    {
        glBegin(GL_QUADS);{
            glVertex3f( points[i].x, points[i].y,points[i].z);
            glVertex3f( points[i].x+6,points[i].y,points[i].z);
            glVertex3f(points[i].x+6,points[i].y,points[i].z+6);
            glVertex3f(points[i].x, points[i].y,points[i].z+6);
        }glEnd();
        //cout<<points[i].x<<" "<<points[i].y<<" "<<points[i].z<<endl;
    }
}

void keyboardListener(unsigned char key, int x,int y){
    struct point temp1;
    struct point temp2;
    double angle=50.0, step=2.0;
    switch(key){
        case '1':
            temp1 = cross_product(u,l);
            l = new_vector(l,temp1, rotate_cam);
            temp2 = cross_product(u,r);
            r = new_vector(r,temp2, rotate_cam);

            break;
        case '2':
            temp1 = cross_product(l,u);
            l = new_vector(l,temp1, rotate_cam);
            temp2 = cross_product(r,u);
            r = new_vector(r,temp2,rotate_cam);

            break;
        case '3':
            temp1 = cross_product(r,l);
            l = new_vector(l,temp1,rotate_cam);
            temp2 = cross_product(r,u);
            u = new_vector(u,temp2,rotate_cam);

            break;
        case '4':
            temp1 = cross_product(l,r);
            l = new_vector(l,temp1,rotate_cam);
            temp2 = cross_product(u,r);
            u = new_vector(u,temp2,rotate_cam);

            break;
        case '5':
            temp1 = cross_product(u,l);
            u = new_vector(u,temp1,rotate_cam);
            temp2 = cross_product(r,l);
            r = new_vector(r,temp2,rotate_cam);

            break;
        case '6':
            temp1 = cross_product(l,u);
            u = new_vector(u,temp1,rotate_cam);
            temp2 = cross_product(l,r);
            r = new_vector(r,temp2,rotate_cam);
            break;
        case 'q':
            if(qw<=angle)
                qw+=step;
            break;
        case 'w':
            if(qw>=-angle)
                qw-=step;
            break;
        case 'e':
            if(er<=angle)
                er+=step;
            break;
        case 'r':
            if(er>=-angle)
                er-=step;
            break;
        case 'a':
            if(as<=angle)
                as+=step;
            break;
        case 's':
            if(as>=-angle)
                as-=step;
            break;
        case 'd':
            if(df<=angle)
                df+=step;
            break;
        case 'f':
            if(df>=-angle)
                df-=step;
            break;
        default:
            break;
    }
}
void specialKeyListener(int key, int x, int y){
    switch(key){
        case GLUT_KEY_DOWN:		//down arrow key
            cam_pos.z -= move_cam * l.z;
            cam_pos.y -= move_cam * l.y;
            cam_pos.x -= move_cam * l.x;
            break;
        case GLUT_KEY_UP:		// up arrow key
            cam_pos.z += move_cam * l.z;
            cam_pos.y += move_cam * l.y;
            cam_pos.x += move_cam * l.x;
            break;
        case GLUT_KEY_RIGHT:
            cam_pos.z += move_cam * r.z;
            cam_pos.y += move_cam * r.y;
            cam_pos.x += move_cam * r.x;
            break;
        case GLUT_KEY_LEFT:
            cam_pos.z -= move_cam * r.z;
            cam_pos.y -= move_cam * r.y;
            cam_pos.x -= move_cam * r.x;
            break;
        case GLUT_KEY_PAGE_UP:
            cam_pos.z += move_cam * u.z;
            cam_pos.y += move_cam * u.y;
            cam_pos.x += move_cam * u.x;
            break;
        case GLUT_KEY_PAGE_DOWN:
            cam_pos.z -= move_cam * u.z;
            cam_pos.y -= move_cam * u.y;
            cam_pos.x -= move_cam * u.x;
            break;
        default:
            break;
    }
}
void mouseListener(int button, int state, int x, int y){
    switch(button){
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN){
                struct point a=gunShot();
                if(a.x<140 && a.z<140 && a.x>-140 && a.z>-140)
                {
                    points[no_of_shots]=a;
                    no_of_shots++;
                }
            }
            break;

        case GLUT_RIGHT_BUTTON:
            //........
            if(state == GLUT_DOWN){
                drawaxes=1-drawaxes;
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
    //gluLookAt(0,0,200,	0,0,0,	0,1,0);
    gluLookAt(cam_pos.x,cam_pos.y,cam_pos.z,	cam_pos.x+l.x,cam_pos.y+l.y,cam_pos.z+l.z,	u.x,u.y,u.z);


    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/

    drawSS();

    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}

void animate(){
    //angle+=0.05;
    //codes for any changes in Models, Camera
    glutPostRedisplay();
}

void init(){
    //codes for initialization
    drawaxes=1;

    cam_pos.x = 100;
    cam_pos.y = 100;
    cam_pos.z = 0;
    u.x = 0;
    u.y = 0;
    u.z = 1;
    r.x = -1.0/sqrt(2);
    r.y = 1.0/sqrt(2);
    r.z = 0;
    l.x = -1.0/sqrt(2);
    l.y = -1.0/sqrt(2);
    l.z = 0;

    move_cam=5.0;
    rotate_cam=3.0;

    sphere_radius=30.0;
    half_sphere_radius=10.5;
    cylinder_height = 110.0;

    qw=0;
    er=0;
    as=0;
    df=0;
    dist = 150;
    no_of_shots=0;
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
    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("My OpenGL Program 1");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop();		//The main loop of OpenGL

    return 0;
}