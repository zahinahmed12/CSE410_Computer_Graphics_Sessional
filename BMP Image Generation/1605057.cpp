#include<bits/stdc++.h>
#include "bitmap_image.hpp"
using namespace std;

#define pi (2*acos(0.0))
const int row=4, col=4;
int scr_w, scr_h;
double front, rear, pos_y, neg_y, neg_x, pos_x;
double dx, dy, top_y, bottom_y, left_x, right_x;
const double eps=0.000000001;

struct point{
    double x, y, z;

    point()
    {
        x=0.0;
        y=0.0;
        z=0.0;
    }
    void set_point(double i, double  j, double k)
    {
        x = i;
        y = j;
        z = k;
    }
    void scale_point(double s)
    {
        x*=s;
        y*=s;
        z*=s;
    }
};

struct triangle{

    point points[3];
    int color[3]{};

    triangle(){
        for(int & i : color)
            i=0;
//    color[0]=0;
//    color[1]=0;
//    color[2]=0;
    }
    void set_color()
    {
        color[0] = rand()%255;
        color[1] = 1+rand()%254;
        color[2] = rand()%255;

//        default_random_engine generator(time(nullptr));
//        uniform_int_distribution<int> distribution(0,255);
//        color[0] = distribution(generator);
//        color[1] = 1+distribution(generator);
//        color[2] = distribution(generator);

    }
};

struct point cross_product(struct point a, struct point b)
{
    struct point res;
    res.x = a.y * b.z - a.z * b.y;
    res.y = a.z * b.x - a.x * b.z;
    res.z = a.x * b.y - a.y * b.x;
    return res;
}

double dot_product(struct point a, struct point b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vector<vector<double>> matrix_multiplication(vector<vector<double>> a, vector<vector<double>> b)
{
    vector<vector<double>> res={{0.0, 0.0, 0.0, 0.0},
                                {0.0, 0.0, 0.0, 0.0},
                                {0.0, 0.0, 0.0, 0.0},
                                {0.0, 0.0, 0.0, 0.0}};
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            double temp;
            for(int k=0;k<row;k++)
            {
                temp=a[i][k]*b[k][j];
                res[i][j]+=temp;
            }
        }
    }
    return res;
}

struct point transformed_point(vector<vector<double>> m, vector<vector<double>> n)
{
    struct point q;
    vector<vector<double>> res= {{0.0},{0.0},{0.0},{0.0}};

    for(int i=0;i<row;i++)
    {
        for(int j=0;j<1;j++)
        {
            double temp;
            for(int k=0;k<row;k++)
            {
                temp=m[i][k]*n[k][j];
                res[i][j]+=temp;
            }
        }
    }
    if(res[3][0]!=1)
    {
        res[0][0]/=res[3][0];
        res[1][0]/=res[3][0];
        res[2][0]/=res[3][0];
    }
    q.set_point(res[0][0], res[1][0], res[2][0]);
    return q;
}

bool compare_matrix(vector<vector<double>> a, vector<vector<double>> b)
{
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            if(a[i][j]!=b[i][j])
                return false;
        }
    }
    return true;
}

struct point normalize(struct point p)
{
    double t= 1.0 * sqrt(p.x*p.x+p.y*p.y+p.z*p.z);
    struct point q;
    q.set_point(p.x/t, p.y/t, p.z/t);
    return q;
}

struct point rodrigues_formula(struct point a, struct point x, double theta)
{
    double rad= (theta * pi)/180.0;
    double cosT = cos(rad);
    double sinT = sin(rad);

    struct point res, t1, t2, t3;
    t1 = x;
    t1.scale_point(cosT);
    t2 = a;
    t2.scale_point((1-cosT)*dot_product(a,x));
    t3 = cross_product(a, x);
    t3.scale_point(sinT);
    res.set_point(t1.x+t2.x+t3.x,t1.y+t2.y+t3.y,t1.z+t2.z+t3.z);

    //cout<<res.x<<" "<<res.y<<" "<<res.z<<endl;

    return res;
}

void print_matrix(vector<vector<double>> m)
{
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<col;j++)
        {
            cout<<m[i][j]<<" ";
        }
        cout<<"\n";
    }
    cout<<"\n";
}

vector<double> split_line(const string& str)
{
    string word;
    istringstream ss(str);
    vector<double> aux;
    while(ss >> word)
    {
        aux.push_back(stod(word));
    }
    return aux;
}

vector<vector<double>> view_transformation_matrix(struct point eye, struct point look, struct point up)
{
    struct point l,r,u;
    l.set_point(look.x-eye.x,look.y-eye.y,look.z-eye.z);
    l= normalize(l);
    r= cross_product(l, up);
    r= normalize(r);
    u= cross_product(r, l);

    vector<vector<double>> T= {{1,0,0,-eye.x},
                               {0,1,0,-eye.y},
                               {0,0,1,-eye.z},
                               {0,0,0,1}};
    vector<vector<double>> R= {{r.x,r.y,r.z,0},
                               {u.x,u.y,u.z,0},
                               {-l.x,-l.y,-l.z,0},
                               {0,0,0,1}};

    return matrix_multiplication(R,T);
}

vector<vector<double>> projection_matrix(double fov_y, double ar, double near, double far)
{
    double fov_x, t, r;
    fov_x = fov_y * ar;
    t = near * tan(fov_y*pi/360.0);
    r = near * tan(fov_x*pi/360.0);

    vector<vector<double>> P= {{near/r, 0, 0, 0},
                               {0, near/t, 0, 0},
                               {0, 0, -(far+near)/(far-near), -(2*far*near)/(far-near)},
                               {0, 0, -1, 0}};

    return P;
}

bool is_point_valid(point a, point b, point c)
{
    double max_x, min_x, max_y, min_y, max_z, min_z;
    max_x=max(a.x,b.x);
    max_y=max(a.y,b.y);
    max_z=max(a.z,b.z);
    min_x=min(a.x,b.x);
    min_y=min(a.y,b.y);
    min_z=min(a.z,b.z);

    return ((min_x<=c.x+eps && c.x<=max_x+eps) && (min_y<=c.y+eps && c.y<=max_y+eps) && (min_z<=c.z+eps && c.z<=max_z+eps));
}

int main()
{
    srand(time(nullptr));

    double theta;
    struct point eye, look, up;
    double fov_y, aspect_ratio, near, far;

    vector<vector<double>> I= {{1.0, 0.0, 0.0, 0.0},
                               {0.0, 1.0, 0.0, 0.0},
                               {0.0, 0.0, 1.0, 0.0},
                               {0.0, 0.0, 0.0, 1.0}};
    stack <vector<vector<double>>> S;
    S.push(I);

    vector<triangle> tri;
    vector<int> push_pop;
    int push=0,pop=0;
    string line,word;
    vector<string> lines;
    int itr=0;
    //ifstream scene_file(R"(C:\Users\Asus\CLionProjects\Graphics2\scene.txt)");
    ifstream scene_file(R"(E:\GRAPHICS_SESSIONAL\Test Cases (Updated)\1\scene.txt)");
    while(getline(scene_file, line))
    {
        if(itr<4)
        {
            vector<double> aux = split_line(line);

            if(itr==0)
            {
                eye.set_point(aux[0],aux[1],aux[2]);
            }
            else if(itr==1)
            {
                look.set_point(aux[0],aux[1],aux[2]);
            }
            else if(itr==2)
            {
                up.set_point(aux[0],aux[1],aux[2]);
            }
            else
            {
                fov_y=aux[0];
                aspect_ratio=aux[1];
                near=aux[2];
                far=aux[3];
            }
            itr++;
        }
        else
        {
            if(line=="end")
            {
                break;
            }
            lines.push_back(line);
        }
    }
    scene_file.close();

    unsigned int v_size=lines.size();
    for(int i=0;i<v_size;i++)
    {
        if(lines[i]=="triangle")
        {
            triangle T;
            for(int m=1;m<=3;m++)
            {
                vector<double> aux=split_line(lines[i+m]);
                T.points[m-1].set_point(aux[0],aux[1],aux[2]);
            }
            i+=3;

            for(auto & point : T.points)
            {
                vector<vector<double>> t= {{point.x},
                                           {point.y},
                                           {point.z},
                                           {1}};
                point = transformed_point(S.top(),t);
            }
            tri.push_back(T);
        }
        else if(lines[i]=="push")
        {
            push_pop.push_back(0);
            push++;
        }
        else if(lines[i]=="pop")
        {
            if(push>pop)
            {
                push--;
                int t;
                unsigned int pp=push_pop.size(),s=S.size();
                if(pp!=0)
                {
                    t=push_pop[pp-1];
                    while(t>0)
                    {
                        if(s>1) S.pop();
                        t--;
                    }
                    push_pop.pop_back();
                }
            }
            else{
                cout<< "Error: pop can't operate without a previous push operation\n";
            }
        }
        else if(lines[i]=="translate")
        {
            unsigned int ps = push_pop.size();
            if(ps>0)
            {
                push_pop[ps-1]+=1;
            }
            vector<double> aux = split_line(lines[i+1]);

            vector<vector<double>> T= {{1, 0, 0, aux[0]},
                                       {0, 1, 0, aux[1]},
                                       {0, 0, 1, aux[2]},
                                       {0, 0, 0, 1}};
            i++;
            vector<vector<double>> res= matrix_multiplication(S.top(),T);
            S.push(res);
        }
        else if(lines[i]=="scale")
        {
            unsigned int ps = push_pop.size();
            if(ps>0)
            {
                push_pop[ps-1]+=1;
            }
            vector<double> aux = split_line(lines[i+1]);

            vector<vector<double>> s= {{aux[0], 0, 0, 0},
                                       {0, aux[1], 0, 0},
                                       {0, 0, aux[2], 0},
                                       {0, 0, 0, 1}};
            i++;
            vector<vector<double>> res= matrix_multiplication(S.top(),s);
            S.push(res);
        }
        else if(lines[i]=="rotate")
        {
            unsigned int ps = push_pop.size();
            if(ps>0)
            {
                push_pop[ps-1]+=1;
            }
            vector<double> aux = split_line(lines[i+1]);

            theta = aux[0];
            struct point a, c1, c2, c3, m, n, p;
            a.set_point(aux[1], aux[2], aux[3]);
            m.set_point(1,0,0);
            n.set_point(0,1,0);
            p.set_point(0,0,1);
            a=normalize(a);

            c1=rodrigues_formula(a,m, theta);
            c2=rodrigues_formula(a,n, theta);
            c3=rodrigues_formula(a,p, theta);

            vector<vector<double>> R= {{c1.x, c2.x, c3.x, 0},
                                       {c1.y, c2.y, c3.y, 0},
                                       {c1.z, c2.z, c3.z, 0},
                                       {0, 0, 0, 1}};
            i++;
            vector<vector<double>> res= matrix_multiplication(S.top(),R);
            S.push(res);
        }
    }

    unsigned int triangles = tri.size();
    ofstream stage1("stage1.txt");

    for(int i=0; i<triangles; i++)
    {
        for(auto & point : tri[i].points)
        {
            stage1<< setprecision(7)<<fixed<< point.x<<" "<< point.y<< " "<< point.z<< "\n";
        }
        stage1<< "\n";
    }
    stage1.close();

    //vector<triangle> tri2;
    vector<vector<double>> V = view_transformation_matrix(eye, look, up);

    //print_matrix(V);
    for(int i=0; i<triangles; i++)
    {
        for(auto & point : tri[i].points)
        {
            vector<vector<double>> t= {{point.x},
                                       {point.y},
                                       {point.z},
                                       {1}};
            point = transformed_point(V,t);
        }
    }

    ofstream stage2("stage2.txt");
    for(int i=0; i<triangles; i++)
    {
        for(auto & point : tri[i].points)
        {
            stage2<< setprecision(7)<<fixed<< point.x<<" "<< point.y<< " "<< point.z<< "\n";
        }
        stage2<< "\n";
    }
    stage2.close();

    vector<vector<double>> P = projection_matrix(fov_y,aspect_ratio,near,far);

    for(int i=0; i<triangles; i++)
    {
        for(auto & point : tri[i].points)
        {
            vector<vector<double>> t= {{point.x},
                                       {point.y},
                                       {point.z},
                                       {1}};
            point = transformed_point(P,t);
        }
    }
    ofstream stage3("stage3.txt");
    for(int i=0; i<triangles; i++)
    {
        for(auto & point : tri[i].points)
        {
            stage3<< setprecision(7)<<fixed<< point.x<<" "<< point.y<< " "<< point.z<< "\n";
        }
        stage3<< "\n";
    }
    stage3.close();
    //print_matrix(P);
    ifstream config_file(R"(E:\GRAPHICS_SESSIONAL\Test Cases (Updated)\1\config.txt)");
    itr=0;
    while(getline(config_file, line))
    {
        if(itr==0)
        {
            vector<double> aux = split_line(line);
            scr_w= (int)aux[0];
            scr_h= (int)aux[1];
        }
        else if(itr==1)
        {
            neg_x = stod(line);
            pos_x = -neg_x;
        }
        else if(itr==2)
        {
            neg_y = stod(line);
            pos_y = -neg_y;
        }
        else if(itr==3)
        {
            vector<double> aux = split_line(line);
            front= aux[0];
            rear= aux[1];
        }
        itr++;
    }
    config_file.close();
    //cout<<scr_w<<" "<<scr_h<<" "<<neg_x<<" "<<pos_x<<" "<<neg_y<<" "<<pos_y<<" "<<front<<" "<<rear<<" ";
    ifstream stage3_file(R"(C:\\Users\\Asus\\CLionProjects\\Graphics2\\cmake-build-debug\\stage3.txt)");
    vector<string> staged;
    while(getline(stage3_file, line))
    {
        staged.push_back(line);
    }
    stage3_file.close();
//    for(int i=0; i<19;i++)
//    {
//        cout<<staged[i]<<endl;
//    }
    vector<triangle> new_tri;
    unsigned int vl= staged.size();
    for(int i=0;i<vl;i=i+4)
    {
        triangle t;
        for(int j=0; j<3; j++)
        {
            vector<double> aux = split_line(staged[i+j]);
            t.points[j].set_point(aux[0],aux[1],aux[2]);
        }
        new_tri.push_back(t);
    }
//    for(int i=0; i<triangles; i++)
//    {
//        for(auto & point : new_tri[i].points)
//        {
//            cout<<point.x<<" "<<point.y<<" "<<point.z<<endl;
//        }
//        cout<<"\n";
//    }
    for(int i=0; i<triangles; i++)
    {
        new_tri[i].set_color();
        //cout<<new_tri[i].color[0]<<" "<<new_tri[i].color[1]<<" "<<new_tri[i].color[2]<<"\n";
    }

    dx=(pos_x-neg_x)/scr_w, dy=(pos_y-neg_y)/scr_h;
    top_y=pos_y-dy/2, left_x=neg_x+dx/2, bottom_y=-top_y, right_x=-left_x;

    //cout<<top_y<<" "<<left_x;
    vector<vector<double> > z_buffer (scr_h, vector<double> (scr_w, rear+eps));

//    for(const auto& it: z_buffer)
//    {
//        for(auto ij: it)
//        {
//            cout<< ij<< " ";
//        }
//        cout<< endl;
//    }
    bitmap_image image(scr_w, scr_h);
    image.clear();

    for(auto tr : new_tri)
    {
        double max_y, min_y;
        max_y = max(max(tr.points[0].y,tr.points[1].y),tr.points[2].y);
        min_y = min(min(tr.points[0].y,tr.points[1].y),tr.points[2].y);

        if(max_y>top_y) max_y= top_y;
        max_y=(top_y-max_y)/dy;
        if(min_y<bottom_y) min_y= bottom_y;
        min_y=(top_y-min_y)/dy;

        max_y= round(max_y), min_y= round(min_y);
        int ty= (int)(max_y), by= (int)(min_y);

        if(ty<0) ty=0;
        if(by>scr_h-1) by=scr_h-1;

        for(int i=ty; i<=by; i++)
        {
            bool flags[3] = {false, false, false};
            double max_x, min_x;
            double ys=top_y-i*dy;
            point p;
            p.y=ys;

            double xa= tr.points[0].x+(ys-tr.points[0].y)*(tr.points[1].x-tr.points[0].x)/(tr.points[1].y-tr.points[0].y);
            p.x=xa;
            double za= tr.points[0].z+(ys-tr.points[0].y)*(tr.points[1].z-tr.points[0].z)/(tr.points[1].y-tr.points[0].y);
            p.z=za;
            if(is_point_valid(tr.points[0], tr.points[1], p)) flags[0]=true;

            double xb= tr.points[1].x+(ys-tr.points[1].y)*(tr.points[2].x-tr.points[1].x)/(tr.points[2].y-tr.points[1].y);
            p.x=xb;
            double zb= tr.points[1].z+(ys-tr.points[1].y)*(tr.points[2].z-tr.points[1].z)/(tr.points[2].y-tr.points[1].y);
            p.z=zb;
            if(is_point_valid(tr.points[1], tr.points[2], p)) flags[1]=true;

            double xc= tr.points[0].x+(ys-tr.points[0].y)*(tr.points[2].x-tr.points[0].x)/(tr.points[2].y-tr.points[0].y);
            p.x=xc;
            double zc= tr.points[0].z+(ys-tr.points[0].y)*(tr.points[2].z-tr.points[0].z)/(tr.points[2].y-tr.points[0].y);
            p.z=zc;
            if(is_point_valid(tr.points[0], tr.points[2], p)) flags[2]=true;

            if(flags[0] && flags[1] && !flags[2])
            {
                if(xa<xb)
                {
                    max_x = xb;
                    min_x = xa;
                }
                else
                {
                    max_x = xa;
                    min_x = xb;
                    xb = max_x;
                    xa = min_x;
                    double temp=zb;
                    zb = za;
                    za = temp;
                }
            }
            else if(!flags[0] && flags[1] && flags[2])
            {
                if(xc<xb)
                {
                    max_x = xb;
                    min_x = xc;
                    xa = xc;
                    za = zc;
                }
                else
                {
                    max_x = xc;
                    min_x = xb;
                    xb = max_x;
                    xa = min_x;
                    double temp=zb;
                    zb = zc;
                    za = temp;
                }
//                else
//                {
//                    max_x = xc;
//                    min_x = xc;
//                    xa = xc;
//                    xb = xc;
//                    za = zc;
//                    zb = zc;
//                }
            }
            else if(flags[0] && !flags[1] && flags[2])
            {
                if(xa<xc)
                {
                    max_x = xc;
                    min_x = xa;
                    xb = xc;
                    zb = zc;
                }
                else
                {
                    max_x = xa;
                    min_x = xc;
                    xb = max_x;
                    xa = min_x;
                    zb = za;
                    za = zc;
                }
//                else
//                {
//                    max_x = xc;
//                    min_x = xc;
//                    xa = xc;
//                    xb = xc;
//                    za = zc;
//                    zb = zc;
//                }
            }
            else if(flags[0] && flags[1] && flags[2])
            {
                if(abs(xa-xb)<=eps)
                {
                    if(xa<xc)
                    {
                        max_x = xc;
                        min_x = xa;
                        xb = xc;
                        zb = zc;
                    }
                    else
                    {
                        max_x = xc;
                        min_x = xa;
                        xb = max_x;
                        xa = min_x;
                        zb = zc;
                    }

                }
                else
                {
                    if(xa<xb)
                    {
                        max_x = xb;
                        min_x = xa;
                    }
                    else
                    {
                        max_x = xa;
                        min_x = xb;
                        xb = max_x;
                        xa = min_x;
                        double temp=zb;
                        zb = za;
                        za = temp;
                    }
                }
            }
            else
            {
                continue;
            }

            if(min_x<left_x) min_x= left_x;
            min_x=(min_x-left_x)/dx;
            if(max_x>right_x) max_x= right_x;
            max_x=(max_x-left_x)/dx;

            max_x= round(max_x), min_x= round(min_x);
            int lx= (int)(min_x), rx= (int)(max_x);

            if(lx<0) lx=0;
            if(rx>scr_w-1) rx=scr_w-1;

            for(int j=lx; j<=rx; j++)
            {
                double xp = left_x+j*dx;
                double zp;
                if(xb-xa<eps) continue;
                else zp = za + (xp - xa)*(zb - za)/(xb - xa);

                if(zp+eps>front && z_buffer[i][j]>zp)
                {
                    z_buffer[i][j]=zp;
                    image.set_pixel(j,i,tr.color[0],tr.color[1],tr.color[2]);
                }
            }
        }
    }
    image.save_image("output.bmp");

    ofstream z_buffer_file("z_buffer.txt");
    z_buffer_file << setprecision(6) << fixed;

    for (int i = 0; i < scr_h; i++)
    {
        for (int j = 0; j < scr_w; j++)
        {
            if (z_buffer[i][j] < rear + eps)
            {
                z_buffer_file << z_buffer[i][j] << '\t';
            }
        }
        z_buffer_file << '\n';
    }

    z_buffer_file.close();
}