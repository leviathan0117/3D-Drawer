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
int cut_x1 = -1, cut_y1 = -1, cut_x2 = -1, cut_y2 = -1;
bool state = true, start_crop = false;

void my_mouse_callback( int event, int x, int y, int flags, void* param )
{
    if(event == CV_EVENT_LBUTTONDOWN && !ignoremouse)
    {
        if (state)
        {
            state = !state;
            cut_x1 = x;
            cut_y1 = y;
        } else
        {
            state = !state;
            cut_x2 = x;
            cut_y2 = y;
        }
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

    //String cascade_name = "output/cascade.xml";
    //String cascade_name = "ready_cascades/gen1.1/cascade_dif_2000_5.xml";
    String cascade_name = "ready_cascades/gen1.2/cascade.xml";
    if( !cascade.load( cascade_name ) )
    {
        cout << "Error loading face cascade\n";
        exit(EXIT_FAILURE);
    };

    capture.set(CAP_PROP_FRAME_WIDTH,  800);
    capture.set(CAP_PROP_FRAME_HEIGHT, 600);
    capture.set(CAP_PROP_FPS,          60);

    Mat frame;

    int counter = -1;
    cout << "Please enter start num:";
    cin >> counter;
    int mode = 0;
    cout << "Cut images?(enter 1/0, 1 for good/ 0 for bad):";
    cin >> mode;
    if (!(counter >= 0 && counter <= 10000))
    {
        counter = -1;
    }


    char filename[512];

    char key;

    cvNamedWindow("Camera", CV_WINDOW_AUTOSIZE);
    if (mode)
    {
        setMouseCallback("Camera", my_mouse_callback);
    }

    fstream out;
    out.open("./bad/bad.txt", ios::out);
    if (mode)
    {
        out.open("./good/good.txt", ios::out);
    }

    while(true)
    {
        key = waitKey(10);
        if (capture.read(frame) == false)
        {
            exit(EXIT_FAILURE);
        }

        //imshow("Camera", frame);

        if (key == 27)
        {
            out.close();
            return 0;
        } else if((key == 's' || start_crop) && counter != -1)
        {
            start_crop = false;

            Mat tmp = frame.clone();
            if (mode)
            {
                cout << "image num -" << counter << "- image cutting\n";
                ignoremouse = 0;
                cut_x1 = -1;
                cut_y1 = -1;
                cut_x2 = -1;
                cut_y2 = -1;
                state = true;

                while (true)
                {
                    key = waitKey(1);
                    tmp = frame.clone();
                    if (cut_x1 != -1 && cut_x2 != -1)
                    {
                        line(tmp, Point(cut_x1, cut_y1), Point(cut_x2, cut_y1), Scalar(255, 0, 0), 4, LINE_8);
                        line(tmp, Point(cut_x1, cut_y2), Point(cut_x2, cut_y2), Scalar(255, 0, 0), 4, LINE_8);
                        line(tmp, Point(cut_x1, cut_y1), Point(cut_x1, cut_y2), Scalar(255, 0, 0), 4, LINE_8);
                        line(tmp, Point(cut_x2, cut_y1), Point(cut_x2, cut_y2), Scalar(255, 0, 0), 4, LINE_8);
                    }
                    if (cut_x1 != -1)
                    {
                        rectangle(tmp, Point(cut_x1 - 2, cut_y1 - 2), Point(cut_x1 + 2, cut_y1 + 2), Scalar(0, 255, 0), FILLED, LINE_8 );
                    }
                    if (cut_x2 != -1)
                    {
                        rectangle(tmp, Point(cut_x2 - 2, cut_y2 - 2), Point(cut_x2 + 2, cut_y2 + 2), Scalar(0, 0, 255), FILLED, LINE_8 );
                    }
                    imshow("Camera", tmp);
                    if (key == 's')
                    {
                        break;
                    }
                    if (key == 27)
                    {
                        out.close();
                        return 0;
                    }
                }
                ignoremouse = 1;

                Rect r(cut_x1, cut_y1, cut_x2 - cut_x1, cut_y2 - cut_y1);
                tmp = frame(r);
                sprintf(filename, "./good/%d.png", counter);
                out << counter << ".png 1 0 0 " << cut_x2 - cut_x1 << " " << cut_y2 - cut_y1 << "\n";
            } else
            {
                sprintf(filename, "./bad/%d.png", counter);
                out << counter << ".png\n";
            }

            cout << "Saved image -" << counter << "- to: " << filename << "\n";
            imwrite(filename, tmp);
            counter++;

        }

        Mat frame_gray;
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
        vector<Rect> faces;

        cascade.detectMultiScale(frame_gray, faces);

        for (auto i = 0; i < faces.size(); i++ )
        {
            Point center(faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2);
            ellipse(frame, center, Size(faces[i].width/2, faces[i].height/2), 0, 0, 360, Scalar(255, 0, 255), 4);
        }

        imshow("Camera", frame);

        //printFPS();
    }
}
