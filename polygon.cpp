#include <windows.h>
#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 640, 0, 480);
    glClearColor(0, 0, 0, 1);
}

class Point
{
    public:
        int x, y;
        Point (int x1=0, int y1=0) {
            x = x1;
            y = y1;
        }
};

class FColour {
    public:
    float r, g, b;

    FColour() {
        r = 0;
        g = 0;
        b = 0;
    }

    FColour(float rr, float gg, float bb) {
        r = rr;
        g = gg;
        b = bb;
    }

    FColour(float pixel[3]) {
        r = pixel[0];
        g = pixel[1];
        b = pixel[2];
    }

    void display() {
        cout<<"Red:"<<r<<"  Green:"<<g<<"  Blue:"<<b<<"\n";
    }
};

bool window = true, bFill = false, fFill = false, yFill = false;
int xMin, xMax, yMin, yMax;
vector<Point> points, yxPoints;
vector<Point> clip;

void drawWindow(Point p1, Point p2)
{
    xMin = p1.x;
    yMin = p1.y;
    xMax = p2.x;
    yMax = p2.y;
    glColor3d(0.024, 0.839, 0.627);
    glLineWidth(2);
    glBegin(GL_LINE_LOOP);
        glVertex2d(xMin, yMin);
        glVertex2d(xMin, yMax);
        glVertex2d(xMax, yMax);
        glVertex2d(xMax, yMin);
    glEnd();
    glFlush();
    points.clear();
}

void drawPolygon(vector<Point> points, int i)
{
    if (i==1)
        glColor3d(0.933, 0.424, 0.302);
    else if (i==2)
        glColor3d(0, 0.247, 0.533);
    else
        ;

    glBegin(GL_LINE_LOOP);
        while(points.size()) {
            glVertex2d(points[0].x, points[0].y);
            points.erase(points.begin());
        }
    glEnd();
    glFlush();
}

void drawFilledPolygon(vector<Point> points, int i)
{
        if (i==1)
            glColor3d(0.933, 0.424, 0.302);
        else if (i==2)
            glColor3d(0, 0.247, 0.533);
        else
            ;
        glBegin(GL_POLYGON);
            while(points.size()) {
                glVertex2d(points[0].x, points[0].y);
                points.erase(points.begin());
        }
        glEnd();
        glFlush();
}

vector<Point> leftSide(vector<Point> points)
{
    Point first = points[0], p1, p2;
    vector<Point> result;
    while(points.size()) {
        if (points.size()==1) {
            p1 = points[0];
            p2 = first;
        }
        else {
            p1 = points[0];
            p2 = points[1];
        }

        float x1=p1.x, x2=p2.x, y1=p1.y, y2=p2.y;
        //First outside & Second inside
        if (x1<xMin && x2>=xMin) {
            float slope = (y2 - y1) / (x2 - x1);
            int y = y1 + (xMin - x1)*slope;
            Point p = Point(xMin, y);
            result.push_back(p);
            result.push_back(p2);
        }
        //Both inside
        else if (x1>=xMin && x2>=xMin) {
            result.push_back(p2);
        }
        //First inside & second outside
        else if (x1>=xMin && x2<xMin) {
            float slope = (y2 - y1) / (x2 - x1);
            int y = y1 + (xMin - x1)*slope;
            Point p = Point(xMin, y);
            result.push_back(p);
        }
        //Both outside
        else {
            ;
        }
        points.erase(points.begin());
    }
    return result;
}

vector<Point> rightSide(vector<Point> points)
{
    Point first = points[0], p1, p2;
    vector<Point> result;
    while(points.size()) {
        if (points.size()==1) {
            p1 = points[0];
            p2 = first;
        }
        else {
            p1 = points[0];
            p2 = points[1];
        }

        float x1=p1.x, x2=p2.x, y1=p1.y, y2=p2.y;
        //First outside & Second inside
        if (x1>xMax && x2<=xMax) {
            float slope = (y2 - y1) / (x2 - x1);
            int y = y1 + (xMax - x1)*slope;
            Point p = Point(xMax, y);
            result.push_back(p);
            result.push_back(p2);
        }
        //Both inside
        else if (x1<=xMax && x2<=xMax) {
            result.push_back(p2);
        }
        //First inside & second outside
        else if (x1<=xMax && x2>xMax) {
            float slope = (y2 - y1) / (x2 - x1);
            int y = y1 + (xMax - x1)*slope;
            Point p = Point(xMax, y);
            result.push_back(p);
        }
        //Both are outside
        else {
            ;
        }
        points.erase(points.begin());
    }
    return result;
}

vector<Point> topSide(vector<Point> points)
{
    Point first = points[0], p1, p2;
    vector<Point> result;
    while(points.size()) {
        if (points.size()==1) {
            p1 = points[0];
            p2 = first;
        }
        else {
            p1 = points[0];
            p2 = points[1];
        }

        float x1=p1.x, x2=p2.x, y1=p1.y, y2=p2.y;
        //First outside & Second inside
        if (y1>yMax && y2<=yMax) {
            float slope = (yMax - y1) / (y2 - y1);
            int x = x1 + (x2 - x1) * slope;
            Point p = Point(x, yMax);
            result.push_back(p);
            result.push_back(p2);
        }
        //Both are inside
        else if (y1<=yMax && y2<=yMax) {
            result.push_back(p2);
        }
        //First inside & Second outside
        else if (y1<=yMax && y2>yMax) {
            float slope = (yMax - y1) / (y2 - y1);
            int x = x1 + (x2 - x1) * slope;
            Point p = Point(x, yMax);
            result.push_back(p);
        }
        //Both are outside
        else {
            ;
        }
        points.erase(points.begin());
    }
    return result;
}

vector<Point> bottomSide(vector<Point> points)
{
    Point first = points[0], p1, p2;
    vector<Point> result;
    while(points.size()) {
        if (points.size()==1) {
            p1 = points[0];
            p2 = first;
        }
        else {
            p1 = points[0];
            p2 = points[1];
        }

        float x1=p1.x, x2=p2.x, y1=p1.y, y2=p2.y;
        //First outside & Second inside
        if (y1<yMin && y2>=yMin) {
            float slope = (yMin - y1) / (y2 - y1);
            int x = x1 + (x2 - x1) * slope;
            Point p = Point(x, yMin);
            result.push_back(p);
            result.push_back(p2);
        }
        //Both are inside
        else if (y1>=yMin && y2>=yMin) {
            result.push_back(p2);
        }
        //First inside & Second outside
        else if (y1>=yMin && y2<yMin) {
            float slope = (yMin - y1) / (y2 - y1);
            int x = x1 + (x2 - x1) * slope;
            Point p = Point(x, yMin);
            result.push_back(p);
        }
        //Both are outside
        else {
            ;
        }
        points.erase(points.begin());
    }
    return result;
}

void sutherlandHodgeman(vector<Point> points)
{
    vector<Point> clipped = topSide(bottomSide(rightSide(leftSide(points))));
    clip = clipped;
    //drawFilledPolygon(clipped, 2);
    drawPolygon(clipped, 2);
}

bool sameColour(FColour c1, FColour c2) {
    if(fabs(c1.r - c2.r) < 0.05 && fabs(c1.g - c2.g) < 0.05 && (c1.b - c2.b) < 0.05) {
        return true;
    } else {
        return false;
    }
}

void boundaryFill(int x, int y, FColour fillColour, FColour boundary)
{
    float pixel[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &pixel);
    FColour current(pixel);

    if(!sameColour(current, boundary) && !sameColour(current, fillColour)) {
        glBegin(GL_POINTS);
            glColor3f(fillColour.r, fillColour.g, fillColour.b);
            glVertex2d(x, y);
        glEnd();
        glFlush();

        boundaryFill(x+1, y, fillColour, boundary);
        boundaryFill(x-1, y, fillColour, boundary);
        boundaryFill(x, y+1, fillColour, boundary);
        boundaryFill(x, y-1, fillColour, boundary);
    }
}

void floodFill(int x, int y, FColour fillColour, FColour oldColour)
{
    float pixel[3];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &pixel);
    FColour current(pixel);

    if (sameColour(current, oldColour)) {
        glBegin(GL_POINTS);
            glColor3f(fillColour.r, fillColour.g, fillColour.b);
            glVertex2d(x, y);
        glEnd();
        glFlush();

        floodFill(x+1, y, fillColour, oldColour);
        floodFill(x-1, y, fillColour, oldColour);
        floodFill(x, y+1, fillColour, oldColour);
        floodFill(x, y-1, fillColour, oldColour);
    }

}

void yx(vector<Point> points)
{
    FColour bg = FColour(0, 0, 0);
    for (int y=0; y<=480; y++) {
        vector<int> intersections;
        int n = points.size();
        for (int i=0, j=1; i<n; i++, j=(++j)%n) {
            int y1 = clip[i].y;
            int y2 = clip[j].y;

            if (y1<y2) {
                if (y>=y1 && y<y2) {
                    int x = clip[i].x + (y - y1) * (clip[j].x - clip[i].x) / (y2 - y1);
                    intersections.push_back(x);
                }
            }
            else {
                if (y>=y2 && y<y1) {
                    int x = clip[i].x + (y - y1) * (clip[j].x - clip[i].x) / (y2 - y1);
                    intersections.push_back(x);
                }
            }
        }
        sort(intersections.begin(), intersections.end());
        int m = intersections.size();

        glColor3f(1, 1, 0);
        for (int i=0; i<m; i+=2) {
            int x1 = intersections[i];
            int x2 = intersections[i+1];
            glBegin(GL_LINES);
                glVertex2d(x1, y);
                glVertex2d(x2, y);
            glEnd();
            glFlush();
        }
    }
    clip.clear();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key=='c') {
        sutherlandHodgeman(points);
        points.clear();
    }
    if (key=='b') {
        bFill = true;
    }
    if (key=='f') {
        fFill = true;
    }
    if (key=='y') {
        yx(clip);
    }
//    if (key=='n') {
//        yFill = true;
//    }
}

void mouse(int button, int action, int x, int y)
{
    if (button==GLUT_LEFT_BUTTON && action==GLUT_DOWN) {
        Point p = Point(x, 480-y);
        points.push_back(p);
        if (window && points.size()==2) {
            drawWindow(points[0], points[1]);
            window = false;
        }
        if (bFill) {
            FColour c1 = FColour(1, 1, 1);
            FColour c2 = FColour(0, 0.247, 0.533);
            //FColour c2 = FColour(0.024, 0.839, 0.627);
            boundaryFill(x, 480-y, c1, c2);
            bFill = false;
        }
        if (fFill) {
            //Fcolour c1 = FColour(0.024, 0.839, 0.627);
            FColour c1 = FColour(1, 0, 1);
            FColour c2 = FColour(0, 0, 0);
            floodFill(x, 480-y, c1, c2);
            fFill = false;
        }
//        if (yFill) {
//            Point p = Point(x, 480-y);
//            yxPoints.push_back(p);
//        }
    }
    if (button==GLUT_RIGHT_BUTTON && action==GLUT_DOWN) {
        //drawFilledPolygon(points, 1);
        drawPolygon(points, 1);
//        if (yFill) {
//            drawPolygon(points, 2);
//            yx(yxPoints);
//            yxPoints.clear();
//            yFill = false;
//        }
    }
}

static void display()
{
    //glClearColor(GL_COLOR_BUFFER_BIT);
    //glColor3d(0, 0, 0);
    //glFlush();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
    glutCreateWindow("Polygon Clipping and Filling");
    glutDisplayFunc(display);

    init();
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
