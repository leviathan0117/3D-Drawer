#if  defined (__STRICT_ANSI__)                   // Try to extend strict ANSI rules

    #ifdef __GNUC__
    #warning TXLib.h: Trying to extend strict ANSI compatibility
    #endif

    #undef  __STRICT_ANSI__
    #define __STRICT_ANSI__UNDEFINED

    #if defined (_STRING_H_) || defined (_INC_STRING) || defined (_STDIO_H_) || defined (_INC_STDIO)

    #ifdef __GNUC__
    #error
    #error --------------------------------------------------------------------------------------------------------
    #endif
    #error TXLib.h: Should include "TXLib.h" BEFORE <string.h> or <stdio.h> in Strict ANSI mode.
    #error
    #error REARRANGE your #include directives, or DISABLE ANSI-compliancy.
    #error --------------------------------------------------------------------------------------------------------
    #error

    #endif

#endif
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
//#include "../libraries/EXLib.h"
//#include "TXLib.h"

using namespace std;
using namespace cv;

fstream in, out;

VideoCapture capture;
VideoCapture capture2;
Mat frame;
Mat frame2;

//marker
/*int h1 = 47;
int s1 = 111;
int v1 = 69;
int h2 = 83;
int s2 = 255;
int v2 = 218;*/

//marker_wide
int h1 = 40;
int s1 = 79;
int v1 = 16;
int h2 = 87;
int s2 = 255;
int v2 = 255;

/*zint h1 = 2;
int s1 = 99;
int v1 = 70;
int h2 = 16;
int s2 = 204;
int v2 = 193;*/

//red
/*int h1 = 0;
int s1 = 123;
int v1 = 124;
int h2 = 9;
int s2 = 255;
int v2 = 255;*/

double angle = -45, angleSpeed = 0;
double drawSize = 100;
double cordDrawSize = 100;

int bla = 0;

char key = 0;

double firstcamx;
double secondcamx;
double firstcamz;
double a;
double b;
double c;

string new_file_name;
string old_file_name;

bool start_window_work = true;
bool create_new_file = false;
bool file_open = false;

double x[10000];
double y[10000];
double z[10000];
int size = 0;

Mat redFilter(const Mat& src)
{
    assert(src.type() == CV_8UC3);

    Mat filtered;
    inRange(src, Scalar(h1, s1, v1), Scalar(h2, s2, v2), filtered);

    return filtered;
}

void imageProssesing()
{
    if (capture.open(4) == false)
    {
        exit(EXIT_FAILURE);
    }
    if (capture2.open(2) == false)
    {
        exit(EXIT_FAILURE);
    }

    capture.set(CAP_PROP_FRAME_WIDTH,  1280);
    capture.set(CAP_PROP_FRAME_HEIGHT, 720);
    capture.set(CAP_PROP_FPS,          60);

    capture2.set(CAP_PROP_FRAME_WIDTH,  1280);
    capture2.set(CAP_PROP_FRAME_HEIGHT, 720);
    capture2.set(CAP_PROP_FPS,          60);
    while (key != 'z')
    {
        //exProcessMessages ();
        key = waitKey(1);
        //=============================================================================
        //      CAMERA 1
        if (capture.read(frame) == false)
        {
            exit(EXIT_FAILURE);
        }

        Mat source = frame.clone();
        //colour filter
        cvtColor(frame, frame, COLOR_BGR2HSV);
        frame = redFilter(frame);

        imshow("Camera1", frame);

        //bluring image
        blur(frame, frame, cv::Size(10, 10));

        //detecting contours

        vector<vector<Point> > contours;
        Mat contourOutput = frame.clone();
        findContours( contourOutput, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
        Mat contourframe(frame.size(), CV_8UC3, Scalar(0,0,0));
        Scalar colors[3];
        colors[2] = Scalar(255, 0, 0);
        colors[1] = Scalar(0, 255, 0);
        colors[0] = Scalar(0, 0, 255);

        for (vector<vector<Point> >::iterator it = contours.begin(); it!=contours.end(); )
        {
            if (it->size()<50)
                it=contours.erase(it);
            else
                ++it;
        }

        float maxr1 = -1, maxr1i = 0;
        Point2f point1;
        float radius1;
        for (int i = 0; i < contours.size(); i++)
        {
            minEnclosingCircle(contours[i], point1, radius1);
            if (radius1 > maxr1)
            {
                maxr1 = radius1;
                maxr1i = i;
            }
        }

        if (contours.size() > 0)
        {
            drawContours(source, contours, maxr1i, Scalar(0, 0, 255), 3);
            minEnclosingCircle(contours[maxr1i], point1, radius1);
        }
        //imshow("Camera1", source);

        if (key == 'q')
            h1++;
        else if (key == 'a')
            h1--;
        else if (key == 'w')
            s1++;
        else if (key == 's')
            s1--;
        else if (key == 'e')
            v1++;
        else if (key == 'd')
            v1--;
        else if (key == 'r')
            h2++;
        else if (key == 'f')
            h2--;
        else if (key == 't')
            s2++;
        else if (key == 'g')
            s2--;
        else if (key == 'y')
            v2++;
        else if (key == 'h')
            v2--;

        cout << h1 << " " << s1 << " " << v1 << " " << h2 << " " << s2 << " " << v2 << " --- ";


        //=============================================================================
        //      CAMERA 2

        if (capture2.read(frame2) == false)
        {
            exit(EXIT_FAILURE);
        }

        Mat source2 = frame2.clone();

        //colour filter
        cvtColor(frame2, frame2, COLOR_BGR2HSV);
        frame2 = redFilter(frame2);

        imshow("Camera2", frame2);

        //bluring image
        blur(frame2, frame2, cv::Size(10, 10));

        //detecting contours

        vector<vector<Point> > contours2;
        Mat contourOutput2 = frame2.clone();
        findContours( contourOutput2, contours2, CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
        Mat contourframe2(frame2.size(), CV_8UC3, Scalar(0,0,0));
        Scalar colors2[3];
        colors2[2] = Scalar(255, 0, 0);
        colors2[1] = Scalar(0, 255, 0);
        colors2[0] = Scalar(0, 0, 255);

        for (vector<vector<Point> >::iterator it = contours2.begin(); it!=contours2.end(); )
        {
            if (it->size()<50)
                it=contours2.erase(it);
            else
                ++it;
        }

        float maxr2 = -1, maxr2i = 0;
        Point2f point2;
        float radius2;
        for (int i = 0; i < contours2.size(); i++)
        {
            minEnclosingCircle(contours2[i], point2, radius2);
            if (radius2 > maxr2)
            {
                maxr2 = radius2;
                maxr2i = i;
            }
        }

        if (contours2.size() > 0)
        {
            drawContours(source2, contours2, maxr2i, Scalar(0, 0, 255), 3);
            minEnclosingCircle(contours2[maxr2i], point2, radius2);
        }
        //imshow("Camera2", source2);


        //=============================================================================
        //      IMAGE PROSESSING

        //cout << point1.x << endl << point1.y << endl << point2.x << endl << point2.y << "\n\n\n";

        firstcamx=(point2.x-640)/40;
        secondcamx=(point1.x-640)/40;
        firstcamz=(360-point1.y)/40;
        double kinda23 = 22.6697;
        int now = size + 1;
        if (firstcamx!=0)
        {
            a = kinda23/(firstcamx);
        }

        if (secondcamx!=0)
        {
            b = kinda23/(secondcamx);
        }

        if (firstcamz!=0)
        {
            c = kinda23/(firstcamz);
        }
        if (firstcamx!=0 && secondcamx!=0 && firstcamz!=0)
        {
            x[now]=(a+b)/(a-b);
            y[now]=(2*a*b)/(a-b);
            z[now]=y[now]/c;
        }
        if (firstcamz==0 && firstcamx!=0 && secondcamx!=0)
        {
            x[now]=(a+b)/(a-b);
            y[now]=(2*a*b)/(a-b);
            z[now]=0;
        }
        if (firstcamx==0 && firstcamz!=0)
        {
            x[now]=1;
            y[now]=2*b;
            z[now]=y[now]/c;
        }

        if (secondcamx==0 && firstcamz!=0)
        {
            x[now]=-1;
            y[now]=-2*a;
            z[now]=y[now]/c;
        }
        if (firstcamx==0 && firstcamz==0)
        {
            x[now]=1;
            y[now]=2*b;
            z[now]=0;
        }
        if (secondcamx==0 && firstcamz==0)
        {
            x[now]=1;
            y[now]=-2*a;
            z[now]=0;
        }

        y[now] = abs(y[now]);// - 6;

        if (y[now] >= -10 && y[now] <= 10)
        {
            size++;
            if (size > 10)
            {
                /*if (abs(y[now] - y[now - 1]) >= 2 || abs(z[now] - z[now - 1]) >= 2 || (contours.size() == 0 || contours2.size() == 0))
                {
                    size--;
                }*/
                if (contours.size() == 0 || contours2.size() == 0 || abs(point1.y - point2.y) >= 30)
                {
                    size--;
                }
            }
        }



        //size++;


        cout << x[now] << " " << y[now] << " " << z[now] << "\n";

        /*if (abs(point1.y - point2.y) >= 40)
        {
            size--;
        }*/
    }
}
int main()
{
    imageProssesing();
}
