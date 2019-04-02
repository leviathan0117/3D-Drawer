#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <GL/glut.h>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <thread>
#include <numeric>

using namespace cv;
using namespace std;

fstream in, out;

VideoCapture camera_capture1;
VideoCapture camera_capture2;
Mat frame;
Mat frame2;

//stand_green
int h1 = 59;
int s1 = 59;
int v1 = 29;
int h2 = 89;
int s2 = 195;
int v2 = 195;

char opencv_window_key = 0;

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

double x_points[10000];
double y_points[10000];
double z_points[10000];
int size = 0;

bool draw_lines = 0;

int it = 0;
vector <int> detect_time(10, 0);
bool state = 1;
bool added = 0;
bool prog_shutdown = 0;

//===============================================================

double angle = M_PI / 4 + M_PI, angle2 = M_PI / 16;
double lx = 0, ly = 0, lz = 0;
double deltaAngle = 0, deltaAngle2 = 0;
double deltaMoveForw = 0, deltaMoveSide = 0, deltaMoveVert = 0;
int xOrigin = -1;
int yOrigin = -1;

bool center_object = 1;

long long time_now, timebase;
int frames = 0;

CascadeClassifier cascade;

void detect();
void printFPS();


int main( int argc, const char** argv )
{
    //String cascade_name = "output/cascade.xml";
    //String cascade_name = "ready_cascades/gen1.1/cascade_dif_2000_5.xml";
    String cascade_name = "ready_cascades/gen1.2/cascade.xml";
    if( !cascade.load( cascade_name ) )
    {
        cout << "Error loading face cascade\n";
        exit(EXIT_FAILURE);
    };
    if (camera_capture1.open(0) == false)
    {
        exit(EXIT_FAILURE);
    }

    camera_capture1.set(CAP_PROP_FRAME_WIDTH,  800);
    camera_capture1.set(CAP_PROP_FRAME_HEIGHT, 600);
    camera_capture1.set(CAP_PROP_FPS,          30);

    if (camera_capture1.read(frame) == false)
    {
        exit(EXIT_FAILURE);
    }

	detect();
}

void printFPS()
{
	frames++;
	time_now=glutGet(GLUT_ELAPSED_TIME);
	if (time_now - timebase > 1000)
	{
		cout << frames * 1000.0 / (time_now - timebase) << "\n";
		cout.flush();
		timebase = time_now;
		frames = 0;
	}
}

void detect()
{
    while (opencv_window_key != 27 && !prog_shutdown)
    {
        if (camera_capture1.read(frame) == false)
        {
            exit(EXIT_FAILURE);
        }
        opencv_window_key = waitKey(1);

        Mat frame_gray;
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
        vector<Rect> faces;

        cascade.detectMultiScale(frame_gray, faces);

        for (auto i = 0; i < faces.size(); i++ )
        {
            Point center(faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2);
            ellipse(frame, center, Size(faces[i].width/2, faces[i].height/2), 0, 0, 360, Scalar(255, 0, 255), 4);
        }

        if (faces.size() > 0)
        {
            detect_time[it] = 1;
        } else
        {
            detect_time[it] = 0;
        }

        imshow("camera_capture1 - detection", frame);

        printFPS();
    }

    exit(0);
}
