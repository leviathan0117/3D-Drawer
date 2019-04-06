// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>
#include <iomanip>

using namespace cv;
using namespace std;

bool image_resize_to_cascade = false;

class Detector
{
    HOGDescriptor hog;
public:
    Detector() : hog()
    {
        hog.load("detector.yml");
    }
    vector<Rect> detect(InputArray img)
    {
        vector<Rect> found;
        if (image_resize_to_cascade)
        {
            hog.detectMultiScale(img, found, 0, Size(8, 8), Size(32, 32), 1.05, 2, false);
        } else
        {
            hog.detectMultiScale(img, found, 0, Size(8, 8), Size(32, 32), 1.0, 2, false);
        }
        return found;
    }
    void adjustRect(Rect & r) const
    {
        // The HOG detector returns slightly larger rectangles than the real objects,
        // so we slightly shrink the rectangles to get a nicer output.
        // this is because of image resize to 1.05 in detectMultiScale
        r.x += cvRound(r.width*0.1);
        r.width = cvRound(r.width*0.8);
        r.y += cvRound(r.height*0.07);
        r.height = cvRound(r.height*0.8);
    }
};

int main()
{

    VideoCapture camera_capture;

    if (camera_capture.open(0) == false)
    {
        cout << "Can not open video stream:(\n";
        exit(1);
    }

    Detector detector;
    Mat frame;
    while (true)
    {
        camera_capture >> frame;
        if (frame.empty())
        {
            cout << "Finished reading: empty frame" << endl;
            break;
        }
        int64 t = getTickCount();
        vector<Rect> found = detector.detect(frame);
        t = getTickCount() - t;

        // show the window
        {
            ostringstream buf;
            buf << "FPS: " << fixed << setprecision(2) << (getTickFrequency() / (double)t);
            putText(frame, buf.str(), Point(10, 30), FONT_HERSHEY_PLAIN, 2.0, Scalar(0, 0, 255), 2, LINE_AA);
        }
        for (vector<Rect>::iterator i = found.begin(); i != found.end(); ++i)
        {
            Rect &r = *i;
            if (image_resize_to_cascade)
            {
                detector.adjustRect(r);
            }
            rectangle(frame, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
        }
        imshow("People detector", frame);



        const char key = (char)waitKey(1);
        if (key == 27 || key == 'q')
        {
            cout << "Exit requested" << endl;
            break;
        }
    }
    return 0;
}
