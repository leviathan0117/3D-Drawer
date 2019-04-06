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

using namespace std;
using namespace cv;

VideoCapture capture;
Mat frame;

int ignoremouse = 1;
int cut_x1 = -1, cut_y1 = -1;
bool start_crop = false;

void my_mouse_callback( int event, int x, int y, int flags, void* param )
{
    if(event == CV_EVENT_LBUTTONDOWN && !ignoremouse)
    {
        cut_x1 = x;
        cut_y1 = y;
    } else if (event == CV_EVENT_LBUTTONDOWN)
    {
        start_crop = 1;
    }
}

CascadeClassifier cascade;
VideoCapture camera_capture1;
char opencv_window_key = 0;

int main()
{
    if (capture.open(0) == false)
    {
        exit(EXIT_FAILURE);
    }

    capture.set(CAP_PROP_FRAME_WIDTH,  800);
    capture.set(CAP_PROP_FRAME_HEIGHT, 600);
    capture.set(CAP_PROP_FPS,          60);

    Mat frame;

    int counter = -1;
    cout << "Please enter start num for HOG images:";
    cin >> counter;

    int image_size = 200;

    char filename[512];

    char key;

    cvNamedWindow("Camera", CV_WINDOW_AUTOSIZE);

    setMouseCallback("Camera", my_mouse_callback);

    while(true)
    {
        skip:
        key = waitKey(10);
        if (capture.read(frame) == false)
        {
            exit(EXIT_FAILURE);
        }

        if (key == 27)
        {
            exit(0);
        } else if((key == 's' || start_crop) && counter != -1)
        {
            start_crop = false;

            Mat tmp = frame.clone();
            cout << "image num -" << counter << "- image cutting\n";
            ignoremouse = 0;
            cut_x1 = -1;
            cut_y1 = -1;


            while (true)
            {
                key = waitKey(1);
                tmp = frame.clone();
                if (cut_x1 != -1)
                {
                    rectangle(tmp, Point(cut_x1 - image_size / 2, cut_y1 - image_size / 2), Point(cut_x1 + image_size / 2, cut_y1 + image_size / 2), Scalar(0, 255, 0), 1, LINE_8 );
                }
                if (cut_x1 != -1)
                {
                    rectangle(tmp, Point(cut_x1 - 2, cut_y1 - 2), Point(cut_x1 + 2, cut_y1 + 2), Scalar(0, 255, 0), FILLED, LINE_8 );
                }
                imshow("Camera", tmp);
                if (key == 's')
                {
                    break;
                }
                if (key == 27)
                {
                    cout << "abort image cutting\n";
                    goto skip;
                }
            }
            ignoremouse = 1;

            Rect r(cut_x1 - image_size / 2, cut_y1 - image_size / 2, image_size, image_size);
            tmp = frame(r);
            sprintf(filename, "./good/%d.png", counter);
            cout << "Saved image -" << counter << "- to: " << filename << "\n";
            imwrite(filename, tmp);
            counter++;
        }

        imshow("Camera", frame);
    }
}
