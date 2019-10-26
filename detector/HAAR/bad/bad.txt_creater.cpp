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
    cout << "Please enter last picture num:";
    cin >> endnum;
    endnum++;

    fstream out;
    out.open("bad.txt", ios::out);

    for (int i = 0; i < endnum; i++)
    {
        //out << "F:\\Projects\\3D_Drawer\\V0.3.0-alpha\\HAAR\\bad\\" << i << ".png\n";
        out << "/root/HAAR/bad/" << i << ".png\n";
    }

}

