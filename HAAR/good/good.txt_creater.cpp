#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/cuda.hpp>

using namespace std;
using namespace cv;

int main()
{
    int endnum = 0;
    int startnum = 0;
    cout << "Please enter first/last picture num:";
    cin >> startnum >> endnum;
    endnum++;
    char filename[500];

    fstream out;
    out.open("good.txt", ios::out);

    for (int i = startnum; i < endnum; i++)
    {
        sprintf(filename, "./%d.png", i);
        Mat img = imread(filename, 0);
        out << i << ".png 1 0 0 " << img.cols << " " << img.rows << "\n";
    }

}

