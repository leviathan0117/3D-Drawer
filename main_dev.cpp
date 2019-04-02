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
int s2 = 200;
int v2 = 200;

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

vector <vector <double> > x_points;
vector <vector <double> > y_points;
vector <vector <double> > z_points;
int size = 0;

int show_camera_windows = 1;

int it = 0;
vector <int> detect_time(10, 0);
bool state = 0;

string file_name;

//===============================================================

double angle = M_PI / 4 + M_PI, angle2 = M_PI / 16;
double lx = 0, ly = 0, lz = 0;
double deltaAngle = 0, deltaAngle2 = 0;
double angle_speed = 0;
int xOrigin = -1;
int yOrigin = -1;

bool center_object = 0;
bool draw_lines = 1;

long long time_now, timebase;
int frames = 0;

int line_x_length = 20;//RED
int line_y_length = 20;//GREEN
int line_z_length = 20;//BLUE

Mat colorFilter(const Mat& src);
void imageProssesing();
void changeWindSize(int w, int h);
void drawSphere(double x_pos, double y_pos, double z_pos, double rad, double qual, double r_col, double g_col, double b_col);
void drawLine(double x1_pos, double y1_pos, double z1_pos, double x2_pos, double y2_pos, double z2_pos, double r_col, double g_col, double b_col);
void printFPS();
void renderScene();
void pressKey(unsigned char key, int unkown_param_1, int unkown_param_2);
void mouseMove(int mouse_x_now, int mouse_y_now);
void mouseButton(int button, int state, int mouse_x_now, int mouse_y_now);
void launchGLUT();

static Scalar randomColor(RNG& rng)
{
    int icolor = (unsigned)rng;
    return Scalar(icolor&255, (icolor>>8)&255, (icolor>>16)&255);
}

int main(int argc, char **argv)
{
	cout << "enter file name";
	cin >> s;



	/*char wndname[] = "3Δ Drawer";
    const int NUMBER = 100;
    const int DELAY = 5;
    int lineType = LINE_AA; // change it to LINE_8 to see non-antialiased graphics
    int i, width = 1280, height = 720;
    int x1 = -width/2, x2 = width*3/2, y1 = -height/2, y2 = height*3/2;
    RNG rng(0xFFFFFFFF);

    Mat image = Mat::zeros(height, width, CV_8UC3);
    imshow(wndname, image);
    waitKey(DELAY);

    char key = ' ';

    while (key != 27)
    {
		Point pt[2][3];
        pt[0][0].x = rng.uniform(x1, x2);
        pt[0][0].y = rng.uniform(y1, y2);
        pt[0][1].x = rng.uniform(x1, x2);
        pt[0][1].y = rng.uniform(y1, y2);
        pt[0][2].x = rng.uniform(x1, x2);
        pt[0][2].y = rng.uniform(y1, y2);
        pt[1][0].x = rng.uniform(x1, x2);
        pt[1][0].y = rng.uniform(y1, y2);
        pt[1][1].x = rng.uniform(x1, x2);
        pt[1][1].y = rng.uniform(y1, y2);
        pt[1][2].x = rng.uniform(x1, x2);
        pt[1][2].y = rng.uniform(y1, y2);
        const Point* ppt[2] = {pt[0], pt[1]};
        int npt[] = {3, 3};

        fillPoly(image, ppt, npt, 2, randomColor(rng), lineType);

        imshow(wndname, image);
		key = waitKey(DELAY);
    }
    waitKey(100);
    destroyAllWindows();

    /*for (i = 0; i < NUMBER * 2; i++)
    {
        Point pt1, pt2;
        pt1.x = rng.uniform(x1, x2);
        pt1.y = rng.uniform(y1, y2);
        pt2.x = rng.uniform(x1, x2);
        pt2.y = rng.uniform(y1, y2);

        int arrowed = rng.uniform(0, 6);

        if( arrowed < 3 )
            line( image, pt1, pt2, randomColor(rng), rng.uniform(1,10), lineType );
        else
            arrowedLine(image, pt1, pt2, randomColor(rng), rng.uniform(1, 10), lineType);

        imshow(wndname, image);
        if(waitKey(DELAY) >= 0)
            return 0;
    }

    for (i = 0; i < NUMBER * 2; i++)
    {
        Point pt1, pt2;
        pt1.x = rng.uniform(x1, x2);
        pt1.y = rng.uniform(y1, y2);
        pt2.x = rng.uniform(x1, x2);
        pt2.y = rng.uniform(y1, y2);
        int thickness = rng.uniform(-3, 10);
        int marker = rng.uniform(0, 10);
        int marker_size = rng.uniform(30, 80);

        if (marker > 5)
            rectangle(image, pt1, pt2, randomColor(rng), MAX(thickness, -1), lineType);
        else
            drawMarker(image, pt1, randomColor(rng), marker, marker_size );

        imshow(wndname, image);
        if(waitKey(DELAY) >= 0)
            return 0;
    }

    for (i = 0; i < NUMBER; i++)
    {
        Point center;
        center.x = rng.uniform(x1, x2);
        center.y = rng.uniform(y1, y2);
        Size axes;
        axes.width = rng.uniform(0, 200);
        axes.height = rng.uniform(0, 200);
        double angle = rng.uniform(0, 180);

        ellipse( image, center, axes, angle, angle - 100, angle + 200,
                 randomColor(rng), rng.uniform(-1,9), lineType );

        imshow(wndname, image);
        if(waitKey(DELAY) >= 0)
            return 0;
    }

    for (i = 0; i< NUMBER; i++)
    {
        Point pt[2][3];
        pt[0][0].x = rng.uniform(x1, x2);
        pt[0][0].y = rng.uniform(y1, y2);
        pt[0][1].x = rng.uniform(x1, x2);
        pt[0][1].y = rng.uniform(y1, y2);
        pt[0][2].x = rng.uniform(x1, x2);
        pt[0][2].y = rng.uniform(y1, y2);
        pt[1][0].x = rng.uniform(x1, x2);
        pt[1][0].y = rng.uniform(y1, y2);
        pt[1][1].x = rng.uniform(x1, x2);
        pt[1][1].y = rng.uniform(y1, y2);
        pt[1][2].x = rng.uniform(x1, x2);
        pt[1][2].y = rng.uniform(y1, y2);
        const Point* ppt[2] = {pt[0], pt[1]};
        int npt[] = {3, 3};

        polylines(image, ppt, npt, 2, true, randomColor(rng), rng.uniform(1,10), lineType);

        imshow(wndname, image);
        if(waitKey(DELAY) >= 0)
            return 0;
    }

    for (i = 0; i< NUMBER; i++)
    {
        Point pt[2][3];
        pt[0][0].x = rng.uniform(x1, x2);
        pt[0][0].y = rng.uniform(y1, y2);
        pt[0][1].x = rng.uniform(x1, x2);
        pt[0][1].y = rng.uniform(y1, y2);
        pt[0][2].x = rng.uniform(x1, x2);
        pt[0][2].y = rng.uniform(y1, y2);
        pt[1][0].x = rng.uniform(x1, x2);
        pt[1][0].y = rng.uniform(y1, y2);
        pt[1][1].x = rng.uniform(x1, x2);
        pt[1][1].y = rng.uniform(y1, y2);
        pt[1][2].x = rng.uniform(x1, x2);
        pt[1][2].y = rng.uniform(y1, y2);
        const Point* ppt[2] = {pt[0], pt[1]};
        int npt[] = {3, 3};

        fillPoly(image, ppt, npt, 2, randomColor(rng), lineType);

        imshow(wndname, image);
        if(waitKey(DELAY) >= 0)
            return 0;
    }

    for (i = 0; i < NUMBER; i++)
    {
        Point center;
        center.x = rng.uniform(x1, x2);
        center.y = rng.uniform(y1, y2);

        circle(image, center, rng.uniform(0, 300), randomColor(rng),
               rng.uniform(-1, 9), lineType);

        imshow(wndname, image);
        if(waitKey(DELAY) >= 0)
            return 0;
    }

    for (i = 1; i < NUMBER; i++)
    {
        Point org;
        org.x = rng.uniform(x1, x2);
        org.y = rng.uniform(y1, y2);

        putText(image, "Testing text rendering", org, rng.uniform(0,8),
                rng.uniform(0,100)*0.05+0.1, randomColor(rng), rng.uniform(1, 10), lineType);

        imshow(wndname, image);
        if(waitKey(DELAY) >= 0)
            return 0;
    }

    Size textsize = getTextSize("3D Drawer forever!", FONT_HERSHEY_COMPLEX, 3, 5, 0);
    Point org((width - textsize.width)/2, (height - textsize.height)/2);

    Mat image2;
    for( i = 0; i < 255; i += 2 )
    {
        image2 = image - Scalar::all(i);
        putText(image2, "3D Drawer forever!", org, FONT_HERSHEY_COMPLEX, 3,
                Scalar(i, i, 255), 5, lineType);

        imshow(wndname, image2);
        if(waitKey(DELAY) >= 0)
            return 0;
    }
    waitKey();
    destroyAllWindows();*/

	//TMP!!!!!!!!
	/*size = 2;
	x_points[0] = -2;
	y_points[0] = 12;
	z_points[0] = -2;
	x_points[1] = -1;
	y_points[1] = 10;
	z_points[1] = 3;*/

	//TMP!!!!!!!!

	//Get GLUT ready
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1280,720);
	glutCreateWindow("3Δ Drawer");

	//thread f1(imageProssesing);
	thread f2(launchGLUT);
	//f1.join();
	f2.join();

	return 1;
}


void changeWindSize(int w, int h)
{
	// предотвращение деления на ноль
	if (h == 0)
	{
		h = 1;
	}
	double ratio =  w * 1.0 / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45.0, ratio, 0.1, 100);
	glMatrixMode(GL_MODELVIEW);
}

void drawSphere(double x_pos, double y_pos, double z_pos, double rad, double qual, double r_col, double g_col, double b_col)
{
	double R_col = r_col / 255;
	double G_col = g_col / 255;
	double B_col = b_col / 255;
	glColor3f(R_col, G_col, B_col);
	glPushMatrix();
	glTranslatef(x_pos, y_pos, z_pos);
 	glutSolidSphere(rad, qual, qual);
	glPopMatrix();
}

void drawLine(double x1_pos, double y1_pos, double z1_pos, double x2_pos, double y2_pos, double z2_pos, double r_col, double g_col, double b_col)
{
	double R_col = r_col / 255;
	double G_col = g_col / 255;
	double B_col = b_col / 255;
	glColor3f(R_col, G_col, B_col);
	glBegin(GL_LINES);
	glVertex3f(x1_pos, y1_pos, z1_pos);
	glVertex3f(x2_pos, y2_pos, z2_pos);
	glEnd();
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

double calIndent (double arr[], int arr_size)
{
	if (arr_size == 0)
	{
		return 0;
	}
	double count = 0;
	for (int i = 0; i < arr_size; i++)
	{
		count += arr[i];
	}
	return count / arr_size;
}

void renderScene()
{
	angle += angle_speed;
	lx = sin(angle + deltaAngle);
	lz = -cos(angle + deltaAngle);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//gluLookAt(lx * 15 + calIndent(x_points, size), ly * 15 + calIndent(z_points, size), lz * 15 + calIndent(y_points, size), calIndent(x_points, size), calIndent(z_points, size),  calIndent(y_points, size), 0, 1, 0);
	gluLookAt(lx * 15, ly * 15, lz * 15, 0, 0, 0, 0, 1, 0);

	drawLine(0, 0, 0, line_x_length, 0, 0, 255, 0, 0);
	for (int i = 0; i <= line_x_length; i++)
	{
		drawSphere(i, 0, 0, 0.05, 10, 255, 0, 0);
	}
	drawLine(0, 0, 0, -line_x_length, 0, 0, 50, 0, 0);
	for (int i = 0; i <= line_x_length; i++)
	{
		drawSphere(-i, 0, 0, 0.05, 10, 50, 0, 0);
	}
	//-----------------
	drawLine(0, 0, 0, 0, line_y_length, 0, 0, 255, 0);
	for (int i = 0; i <= line_y_length; i++)
	{
		drawSphere(0, i, 0, 0.05, 10, 0, 255, 0);
	}
	drawLine(0, 0, 0, 0, -line_y_length, 0, 0, 50, 0);
	for (int i = 0; i <= line_y_length; i++)
	{
		drawSphere(0, -i, 0, 0.05, 10, 0, 50, 0);
	}
	//-----------------

	drawLine(0, 0, 0, 0, 0, line_z_length, 0, 0, 255);
	for (int i = 0; i <= line_z_length; i++)
	{
		drawSphere(0, 0, i, 0.05, 10, 0, 0, 255);
	}
	drawLine(0, 0, 0, 0, 0, -line_z_length, 0, 0, 50);
	for (int i = 0; i <= line_z_length; i++)
	{
		drawSphere(0, 0, -i, 0.05, 10, 0, 0, 50);
	}

	if (state)
	{
		drawSphere(0, 0, 0, 0.1, 10, 0, 255, 255);
	}

	//MAYBE MAKE BOURDERS
	/*drawLine(0, 0, MAX_Y / 2 - calIndent(y_points, size), 0, 0, MIN_Y - calIndent(y_points, size), 0, 0, 100);
	for (int i = MIN_Y; i <= MAX_Y / 2; i++)
	{
		drawSphere(0, 0, i - calIndent(y_points, size), 0.05, 10, 0, 0, 100);
	}
	drawLine(0, 0, MAX_Y / 2 - calIndent(y_points, size), 0, 0, MAX_Y - calIndent(y_points, size), 0, 0, 255);
	for (int i = 0; i <= MAX_Y / 2; i++)
	{
		drawSphere(0, 0, MAX_Y / 2 + i - calIndent(y_points, size), 0.05, 10, 0, 0, 255);
	}*/
	double drawSize = 1;
	/*for (int i = 0; i < size; i++)
	{
		if (center_object)
		{
			drawSphere(x_points[i] * drawSize - calIndent(x_points, size),   z_points[i] * drawSize - calIndent(z_points, size),   y_points[i] * drawSize - calIndent(y_points, size), 0.03, 10, 255, 255, 255);
		} else
		{
			drawSphere(x_points[i] * drawSize,   z_points[i] * drawSize,   y_points[i] * drawSize, 0.03, 10, 255, 255, 255);
		}

	}*/
	for (int i = 0; i < x_points.size(); i++)
		{
            for (int j = 0; j < x_points[i].size(); j++)
            {
				drawSphere(x_points[i][j] * drawSize,   z_points[i][j] * drawSize,   y_points[i][j] * drawSize, 0.03, 5, 255, 255, 255);
            }
		}
	if (draw_lines)
	{
		/*for (int i = 1; i < size; i++)
		{
			if (center_object)
			{
				drawLine(x_points[i] * drawSize - calIndent(x_points, size),   z_points[i] * drawSize - calIndent(z_points, size),   y_points[i] * drawSize - calIndent(y_points, size), x_points[i - 1] * drawSize - calIndent(x_points, size),   z_points[i - 1] * drawSize - calIndent(z_points, size),   y_points[i - 1] * drawSize - calIndent(y_points, size), 0, 255, 0);
			} else
			{
				drawLine(x_points[i] * drawSize,   z_points[i] * drawSize,   y_points[i] * drawSize, x_points[i - 1] * drawSize,   z_points[i - 1] * drawSize,   y_points[i - 1] * drawSize, 0, 255, 0);
			}
		}*/
		for (int i = 0; i < x_points.size(); i++)
		{
            for (int j = 1; j < x_points[i].size(); j++)
            {
				drawLine(x_points[i][j] * drawSize,   z_points[i][j] * drawSize,   y_points[i][j] * drawSize, x_points[i][j - 1] * drawSize,   z_points[i][j - 1] * drawSize,   y_points[i][j - 1] * drawSize, 0, 255, 0);
            }
		}
	}

	glutSwapBuffers();
}

void pressKey(unsigned char key, int unkown_param_1, int unkown_param_2)
{
    if (key == 27)
    {
		exit(0);
	} else if (key == 'd' || key == 'D')
	{
		angle_speed = -0.005;
	} else if (key == 'a' || key == 'A')
	{
		angle_speed = 0.005;
	} else if (key == ' ')
	{
		x_points.clear();
		y_points.clear();
		z_points.clear();
        state = 0;
	} else if (key == 'l' || key == 'L')
	{
		draw_lines = !draw_lines;
	} else if (key == 'p')
	{
		for (int i = 0; i < 10; i++)
		{
			detect_time[i] = 1;
		}
	} else
	{
		angle_speed = 0;
	}
}

void mouseMove(int mouse_x_now, int mouse_y_now)
{
	if (xOrigin >= 0)
	{
		deltaAngle = (mouse_x_now - xOrigin) * 0.001f;

		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	}
	if (yOrigin >= 0)
	{
		deltaAngle2 = (mouse_y_now - yOrigin) * 0.001f;

		if (angle2 + deltaAngle2 >= M_PI / 2 - 0.1)
		{
			deltaAngle2 = M_PI / 2 - angle2 - 0.1;
		}
		if (angle2 + deltaAngle2 <= 0.1 - M_PI / 2)
		{
			deltaAngle2 = -M_PI / 2 - angle2 + 0.1;
		}

		ly = sin(angle2 + deltaAngle2);
	}
}

void mouseButton(int button, int state, int mouse_x_now, int mouse_y_now)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_UP)
		{
			angle += deltaAngle;
			deltaAngle = 0;
			angle2 += deltaAngle2;
			deltaAngle2 = 0;
			xOrigin = -1;
		} else
		{
			xOrigin = mouse_x_now;
			yOrigin = mouse_y_now;
		}
	}
}

void launchGLUT()
{

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeWindSize);
	glutIdleFunc(renderScene);

	lx = sin(angle);
	lz = -cos(angle);
	ly = sin(angle2);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(pressKey);

	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();
}

Mat colorFilter(const Mat& src)
{
    assert(src.type() == CV_8UC3);

    Mat filtered;
    inRange(src, Scalar(h1, s1, v1), Scalar(h2, s2, v2), filtered);

    return filtered;
}

int cam1_finish = 0;
int cam2_finish = 0;

/*void get_camera1_img()
{
	cout << "A";
	cout.flush();
	if (camera_capture1.read(frame) == false)
	{
		exit(EXIT_FAILURE);
	}
	cam1_finish = 1;
	return;
}
void get_camera2_img()
{
	cout << "B";
	cout.flush();
	if (camera_capture2.read(frame2) == false)
	{
		exit(EXIT_FAILURE);
	}
	cam2_finish = 1;
	return;
}*/

void imageProssesing()
{
    CascadeClassifier cascade;
    String cascade_name = "HAAR/output/cascade.xml";
    if( !cascade.load(cascade_name) )
    {
        cout << "Error loading face cascade from: " << cascade_name;
        exit(EXIT_FAILURE);
    };
    if (camera_capture1.open(2) == false)
    {
        exit(EXIT_FAILURE);
    }
    if (camera_capture2.open(4) == false)
    {
        exit(EXIT_FAILURE);
    }

    camera_capture1.set(CAP_PROP_FRAME_WIDTH,  1280);
    camera_capture1.set(CAP_PROP_FRAME_HEIGHT, 720);
    camera_capture1.set(CAP_PROP_FPS,          60);

    camera_capture2.set(CAP_PROP_FRAME_WIDTH,  1280);
    camera_capture2.set(CAP_PROP_FRAME_HEIGHT, 720);
    camera_capture2.set(CAP_PROP_FPS,          60);
    //thread thr1(get_camera1_img);
	//thread thr2(get_camera2_img);
	waitKey(100);
    while (true)
    {
        //printFPS();
        opencv_window_key = waitKey(1);
        //=============================================================================
        //      CAMERA 1

        /*thr1.join();
		thr2.join();
		while (!cam1_finish && !cam2_finish)
		{
			//waiting
		}
		cout << "OK";
		cout.flush();

		cam1_finish = 0;
		cam2_finish = 0; */



        if (camera_capture1.read(frame) == false)
        {
            exit(EXIT_FAILURE);
        }
        if (camera_capture2.read(frame2) == false)
        {
            exit(EXIT_FAILURE);
        }

        Mat frame_gray;
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
        vector<Rect> faces;
        cascade.detectMultiScale(frame_gray, faces);
        for (auto i = 0; i < faces.size(); i++)
        {
            Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
            ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);
        }
        if (faces.size() > 0)
        {
            detect_time[it] = 1;
        } else
        {
            detect_time[it] = 0;
        }
        it++;
        it %= 10;
        if (accumulate(detect_time.begin(), detect_time.end(), 0ll) > 8)
        {
            state = !state;
            if (state)
            {
				x_points.push_back(vector <double> ());
				y_points.push_back(vector <double> ());
				z_points.push_back(vector <double> ());
            }
            fill(detect_time.begin(), detect_time.end(), 0);
        }
        //cout << faces.size() << " " << accumulate(detect_time.begin(), detect_time.end(), 0ll) << " " << state << "\n";
        if (show_camera_windows)
        {
			imshow("camera_capture1 - detection", frame);
        }

        Mat source = frame.clone();
        //colour filter
        cvtColor(frame, frame, COLOR_BGR2HSV);
        frame = colorFilter(frame);

        //detecting contours

        vector<vector<Point> > contours;
        Mat contourOutput = frame.clone();
        findContours( contourOutput, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
        Mat contourframe(frame.size(), CV_8UC3, Scalar(0,0,0));
        Scalar colors[3];
        colors[2] = Scalar(255, 0, 0);
        colors[1] = Scalar(0, 255, 0);
        colors[0] = Scalar(0, 0, 255);

        for (vector<vector<Point> >::iterator it = contours.begin(); it != contours.end(); )
        {
            if (it->size() < 50)
            {
                it = contours.erase(it);
            } else
            {
                ++it;
            }
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
        if (show_camera_windows)
        {
			imshow("Camera1", source);
        }

        if (opencv_window_key == 'q')
            h1++;
        else if (opencv_window_key == 'a')
            h1--;
        else if (opencv_window_key == 'w')
            s1++;
        else if (opencv_window_key == 's')
            s1--;
        else if (opencv_window_key == 'e')
            v1++;
        else if (opencv_window_key == 'd')
            v1--;
        else if (opencv_window_key == 'r')
            h2++;
        else if (opencv_window_key == 'f')
            h2--;
        else if (opencv_window_key == 't')
            s2++;
        else if (opencv_window_key == 'g')
            s2--;
        else if (opencv_window_key == 'y')
            v2++;
        else if (opencv_window_key == 'h')
            v2--;

        //cout << h1 << " " << s1 << " " << v1 << " " << h2 << " " << s2 << " " << v2 << " --- ";


        //=============================================================================
        //      CAMERA 2

        Mat source2 = frame2.clone();

        //colour filter
        cvtColor(frame2, frame2, COLOR_BGR2HSV);
        frame2 = colorFilter(frame2);

        //detecting contours

        vector<vector<Point> > contours2;
        Mat contourOutput2 = frame2.clone();
        findContours( contourOutput2, contours2, CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
        Mat contourframe2(frame2.size(), CV_8UC3, Scalar(0,0,0));
        Scalar colors2[3];
        colors2[2] = Scalar(255, 0, 0);
        colors2[1] = Scalar(0, 255, 0);
        colors2[0] = Scalar(0, 0, 255);

        for (vector<vector<Point> >::iterator it = contours2.begin(); it != contours2.end(); )
        {
            if (it->size() < 50)
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
        if (show_camera_windows)
        {
			imshow("Camera2", source2);
        }


        //=============================================================================
        //      IMAGE PROSESSING

        //cout << point1.x << endl << point1.y << endl << point2.x << endl << point2.y << "\n\n\n";

        firstcamx=(point2.x - 640) / 40;
        secondcamx=(point1.x - 640) / 40;
        firstcamz=(360 - point1.y) / 40;
        double kinda23 = 30.8724;
        if (firstcamx != 0)
        {
            a = kinda23 / (firstcamx);
        }
        if (secondcamx != 0)
        {
            b = kinda23 / (secondcamx);
        }
        if (firstcamz != 0)
        {
            c = kinda23 / (firstcamz);
        }

        double x_fill, y_fill, z_fill;

        if (firstcamx != 0 && secondcamx != 0 && firstcamz != 0)
        {
            x_fill = (a + b) / (a - b);
            y_fill = (2 * a * b) / (a - b);
            z_fill = y_fill / c;
        }
        if (firstcamz == 0 && firstcamx != 0 && secondcamx != 0)
        {
            x_fill = (a + b) / (a - b);
            y_fill = (2 * a * b) / (a - b);
            z_fill = 0;
        }
        if (firstcamx == 0 && firstcamz != 0)
        {
            x_fill = 1;
            y_fill = 2 * b;
            z_fill = y_fill / c;
        }
        if (secondcamx == 0 && firstcamz != 0)
        {
            x_fill = -1;
            y_fill = -2 * a;
            z_fill = y_fill / c;
        }
        if (firstcamx == 0 && firstcamz == 0)
        {
            x_fill = 1;
            y_fill = 2 * b;
            z_fill = 0;
        }
        if (secondcamx == 0 && firstcamz == 0)
        {
            x_fill = 1;
            y_fill = -2 * a;
            z_fill = 0;
        }

        if (y_fill <= 20 && x_fill <= 20 && z_fill <= 20 && x_fill >= -20 && y_fill >= -20 && contours.size() != 0 && contours2.size() != 0 && y_fill >= 0 && abs(point1.y - point2.y) < 100 && state == 1)
        {
			int line_it = x_points.size() - 1;
			x_points[line_it].push_back(x_fill);
			y_points[line_it].push_back(y_fill);
			z_points[line_it].push_back(z_fill);
        }


        //printFPS();
        //cout << x_fill << " " << y_fill << " " << z_fill << " --- " << state << "\n";
    }
}
