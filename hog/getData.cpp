#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <GL/glut.h>
#include <bits/stdc++.h>

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

bool image_resize_to_cascade = false;

class Detector
{
    HOGDescriptor hog;
public:
    Detector() : hog()
    {
        hog.load("detector.yml");
    }
    vector <pair<Rect, double> > detect(InputArray img)
    {
        vector<Rect> found;
        vector<double> values;
        if (image_resize_to_cascade)
        {
            hog.detectMultiScale(img, found, values, 0, Size(8, 8), Size(32, 32), 1.05, 15, false);
        } else
        {
            hog.detectMultiScale(img, found, values, 0, Size(8, 8), Size(32, 32), 1.00, 15, false);
        }

        vector <pair<Rect, double> > out;

        for (int i = 0; i < found.size(); i++)
        {
            out.push_back(pair <Rect, double> (found[i], values[i]));
        }

        return out;
    }
    void adjustRect(Rect & r) const
    {
        // The HOG detector returns slightly larger rectangles than the real objects,
        // so we slightly shrink the rectangles to get a nicer output.
        // this is because of image resize to 1.05 in detectMultiScale
        r.x += cvRound(r.width * 0.1);
        r.width = cvRound(r.width * 0.8);
        r.y += cvRound(r.height * 0.07);
        r.height = cvRound(r.height * 0.8);
    }
};

CascadeClassifier cascade;
VideoCapture camera_capture1;
char opencv_window_key = 0;

//16

int main()
{
    if (capture.open(2) == false)
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

    Detector detector;

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
                    rectangle(tmp, Point(cut_x1 - image_size / 2, cut_y1 - image_size / 2), Point(cut_x1 + image_size / 2, cut_y1 + image_size / 2), Scalar(0, 255, 0), 3, LINE_8 );
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
                    ignoremouse = 1;
                    goto skip;
                }
            }
            ignoremouse = 1;

            Rect r(cut_x1 - image_size / 2, cut_y1 - image_size / 2, image_size, image_size);
            tmp = frame(r);
            if (counter < 10)
            {
                sprintf(filename, "./good/000%d.png", counter);
            } else if (counter < 100)
            {
                sprintf(filename, "./good/00%d.png", counter);
            } else if (counter < 1000)
            {
                sprintf(filename, "./good/0%d.png", counter);
            }
            cout << "Saved image -" << counter << "- to: " << filename << "\n";
            imwrite(filename, tmp);
            counter++;
        }

        imshow("Camera", frame);


        vector <pair<Rect, double> > found;

        int64 t = getTickCount();
        found = detector.detect(frame);
        t = getTickCount() - t;

        double max_value = -1;
        double max_i = 0;
        for (int i = 0; i < found.size(); i++)
        {
            if (found[i].second > max_value)
            {
                max_value = found[i].second;
                max_i = i;
            }
        }

        if (max_value != -1)
        {
            Rect r = found[max_i].first;
            Rect r2 = r;
            if (image_resize_to_cascade)
            {
                detector.adjustRect(r);
            }
            r2.x += r2.width / 4;
            r2.y += r2.height / 4;
            r2.width *= 0.5;
            r2.height *= 0.5;
            rectangle(frame, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
            rectangle(frame, r2.tl(), r2.br(), cv::Scalar(0, 0, 255), 2);
        }



        // show the window
        {
            ostringstream buf;
            buf << "FPS: " << fixed << setprecision(2) << (getTickFrequency() / (double)t);
            putText(frame, buf.str(), Point(10, 30), FONT_HERSHEY_PLAIN, 2.0, Scalar(0, 0, 255), 2, LINE_AA);
        }


        imshow("Camera - detect", frame);
    }
}
