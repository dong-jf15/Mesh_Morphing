#ifndef _MESH_MORPHING
#define _MESH_MORPHING
#include <opencv2\opencv.hpp>
#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

static void onMousesrc(int event, int x, int y, int flags, void* param);
static void onMousedst(int event, int x, int y, int flags, void* param);

class Mesh_Morphing
{
public:
	struct Triangle
	{
		Point2f Point1;
		Point2f Point2;
		Point2f Point3;
	};
	Mat img_src;
	Mat img_dst;
	Mat img_src_clone;
	Mat img_dst_clone;
	vector<Point2f> feature_src;
	vector<Point2f> feature_dst;
	vector<Triangle> triangle_src;
	vector<Triangle> triangle_dst;

	Mesh_Morphing(Mat img1,Mat img2);
	void get_feature();
	void get_feature(string,string);
	bool Judge_In_Image(Mat image,Point2f Point1);
	void Change_to_Triangle();
	float min(float,float,float);
	float max(float,float,float);
	uchar insert_value(Mat,Point2f);
	bool IN_TRIANGLE(vector<Point2f> &, Point2f &);
	void Affine(vector<Triangle>,vector<Triangle>,Mat,Mat &);
	Mat Mesh_Morphing_Procedure(float);
};
	
#endif