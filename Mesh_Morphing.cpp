#include "Mesh_Morphing.h"

Mesh_Morphing::Mesh_Morphing(Mat img1,Mat img2)
{
	img_src = img1.clone();
	img_dst = img2.clone();
	img_src_clone=img_src.clone();
	img_dst_clone=img_dst.clone();
}
void Mesh_Morphing::get_feature()
{
	feature_src.push_back(Point2f(0,0));
	feature_src.push_back(Point2f(0,img_src.cols-1));
	feature_src.push_back(Point2f(img_src.rows-1,0));
	feature_src.push_back(Point2f(img_src.rows-1,img_src.cols-1));
	feature_dst.push_back(Point2f(0,0));
	feature_dst.push_back(Point2f(0,img_src.cols-1));
	feature_dst.push_back(Point2f(img_src.rows-1,0));
	feature_dst.push_back(Point2f(img_src.rows-1,img_src.cols-1));

	
	img_src_clone=img_src.clone();
	img_dst_clone=img_dst.clone();
	Mat temp_src = img_src.clone();
	Mat temp_dst = img_dst.clone();
	cvNamedWindow("image_src");
	cvNamedWindow("image_dst");
	imshow("image_src",temp_src);		
	imshow("image_dst",temp_dst);
	cv::setMouseCallback("image_src",onMousesrc,reinterpret_cast<void*>(this));
	cv::setMouseCallback("image_dst",onMousedst,reinterpret_cast<void*>(this));
	cvWaitKey(0);
	cvDestroyWindow("image_src");
	cvDestroyWindow("image_dst");
}
void Mesh_Morphing::get_feature(string str1,string str2)
{
	ifstream f1,f2;
	f1.open("feature_src.txt");
	f2.open("feature_dst.txt");
	int x,y;
	string line;	
    while (getline(f1, line)) {
        x = stoi(line, nullptr);
        getline(f1, line);
        y = stoi(line, nullptr);
        Point2f fp(x, y);
		this->feature_src.push_back(fp);
    }
	while (getline(f2, line)) {
        x = stoi(line, nullptr);
        getline(f2, line);
        y = stoi(line, nullptr);
        Point2f fp(x, y);
		this->feature_dst.push_back(fp);
    }
	f1.close();
	f2.close();
}
bool Mesh_Morphing::Judge_In_Image(Mat image,Point2f Point1)
{
	if(Point1.x<image.rows&&Point1.y<image.cols&&Point1.x>=0&&Point1.y>=0)
		return true;
	else
		return false;
}
void Mesh_Morphing::Change_to_Triangle()
	{
		Rect rect(0,0,600,600);
		Subdiv2D subdiv(rect);
		int flag;
		for(int i=0;i<feature_src.size();i++)
		{
			subdiv.insert(feature_src[i]);
		}

		vector<Vec6f> temp;
		subdiv.getTriangleList(temp);

		for(int i=0;i<temp.size();i++)
		{
			Point2f Point1(temp[i][0],temp[i][1]);
			Point2f Point2(temp[i][2],temp[i][3]);
			Point2f Point3(temp[i][4],temp[i][5]);

			if(Judge_In_Image(img_src,Point1)&&Judge_In_Image(img_src,Point2)&&Judge_In_Image(img_src,Point3))
			{
				flag=0;
				Triangle temp_src,temp_dst;
				temp_src.Point1=Point1;
				temp_src.Point2=Point2;
				temp_src.Point3=Point3;
				for(int i=0;i<feature_src.size();i++)
				{
					if(feature_src[i]==Point1)
					{
						temp_dst.Point1=feature_dst[i];
						flag++;
					}
					if(feature_src[i]==Point2)
					{
						temp_dst.Point2=feature_dst[i];
						flag++;
					}
					if(feature_src[i]==Point3)
					{
						temp_dst.Point3=feature_dst[i];
						flag++;
					}
					if(flag==3)
					{
						triangle_src.push_back(temp_src);
						triangle_dst.push_back(temp_dst);
						break;
					}
				}
				
			}
		}
		for (int i = 0; i < triangle_src.size(); i++)
			{
				line(img_src_clone, cvPoint(triangle_src[i].Point1.y, triangle_src[i].Point1.x), 
					cvPoint(triangle_src[i].Point2.y, triangle_src[i].Point2.x), CV_RGB(255, 255, 255), 2);
				line(img_src_clone, cvPoint(triangle_src[i].Point1.y, triangle_src[i].Point1.x), 
					cvPoint(triangle_src[i].Point3.y, triangle_src[i].Point3.x), CV_RGB(255, 255, 255), 2);
				line(img_src_clone, cvPoint(triangle_src[i].Point3.y, triangle_src[i].Point3.x), 
					cvPoint(triangle_src[i].Point2.y, triangle_src[i].Point2.x), CV_RGB(255, 255, 255), 2);
			}
		imshow("剖分后源三角形",img_src_clone);
		cout<<"现在展示的是三角剖分后的源图像，按任意键进行下一步选择"<<endl;
		waitKey(0);
		for (int i = 0; i < triangle_dst.size(); i++)
			{
				line(img_dst_clone, cvPoint(triangle_dst[i].Point1.y, triangle_dst[i].Point1.x), 
					cvPoint(triangle_dst[i].Point2.y, triangle_dst[i].Point2.x), CV_RGB(255, 255, 255), 2);
				line(img_dst_clone, cvPoint(triangle_dst[i].Point1.y, triangle_dst[i].Point1.x), 
					cvPoint(triangle_dst[i].Point3.y, triangle_dst[i].Point3.x), CV_RGB(255, 255, 255), 2);
				line(img_dst_clone, cvPoint(triangle_dst[i].Point3.y, triangle_dst[i].Point3.x), 
					cvPoint(triangle_dst[i].Point2.y, triangle_dst[i].Point2.x), CV_RGB(255, 255, 255), 2);
			}
		imshow("剖分后目标三角形",img_dst_clone);		
		cout<<"现在展示的是三角剖分后的目标图像，按任意键进行下一步选择"<<endl;
		waitKey(0);
		destroyAllWindows();
		
	}
float Mesh_Morphing::min(float x,float y,float z)
	{
		float min;
		min = x<y? x:y;
		min = min<z? min:z;
		return min;
	}
float Mesh_Morphing::max(float x,float y,float z)
	{
		float max;
		max = x>y? x:y;
		max = max>z? max:z;
		return max;
	}
uchar Mesh_Morphing::insert_value(Mat img,Point2f point)
	{
		Point2f Point1(floor(point.x),floor(point.y));
		Point2f Point2(floor(point.x),ceil(point.y));
		Point2f Point3(ceil(point.x),floor(point.y));
		Point2f Point4(ceil(point.x),ceil(point.y));

		float value1=img.ptr<uchar>((int)Point1.x)[(int)Point1.y];
		float value2=img.ptr<uchar>((int)Point2.x)[(int)Point2.y];
		float value3=img.ptr<uchar>((int)Point3.x)[(int)Point3.y];
		float value4=img.ptr<uchar>((int)Point4.x)[(int)Point4.y];	

		float value;
		float x= point.x-floor(point.x);
		float y= point.y-floor(point.y);

		value = (value1*(1-x)+value3*x)*(1-y)+(value2*(1-x)+value4*x)*y;
		return (uchar)value;
	}
bool Mesh_Morphing::IN_TRIANGLE(vector<Point2f> &trg, Point2f &p) {
    Point2f v0 = trg[2] - trg[0];
    Point2f v1 = trg[1] - trg[0];
    Point2f v2 = p - trg[0];

    float dot00 = v0.dot(v0);
    float dot01 = v0.dot(v1);
    float dot02 = v0.dot(v2);
    float dot11 = v1.dot(v1);
    float dot12 = v1.dot(v2);

    float inverDeno = 1 / (dot00 * dot11 - dot01 * dot01);

    float u = (dot11 * dot02 - dot01 * dot12) * inverDeno;
    if (u < 0 || u > 1) // if u out of range, return directly
    {
        return false;
    }

    float v = (dot00 * dot12 - dot01 * dot02) * inverDeno;
    if (v < 0 || v > 1) // if v out of range, return directly
    {
        return false;
    }

    return u + v <= 1;
	}
void Mesh_Morphing::Affine(vector<Triangle> triangle_src,vector<Triangle> triangle_dst,Mat img_src,Mat &img_mid)
	{
		Mat X,Y,Z;
		Mat Aff;
		float min_x,min_y,max_x,max_y;
		for(int i=0;i<triangle_src.size();i++)
		{
			
			vector<Point2f> trg;
			X = (Mat_<float>(3,3)<<triangle_src[i].Point1.x,triangle_src[i].Point2.x,triangle_src[i].Point3.x,
				triangle_src[i].Point1.y,triangle_src[i].Point2.y,triangle_src[i].Point3.y,1.0,1.0,1.0);
			Y = (Mat_<float>(3,3)<<triangle_dst[i].Point1.x,triangle_dst[i].Point2.x,triangle_dst[i].Point3.x,
				triangle_dst[i].Point1.y,triangle_dst[i].Point2.y,triangle_dst[i].Point3.y,1.0,1.0,1.0);
			Z = Y.inv();
			Aff= X*Z;
			min_x = min(triangle_dst[i].Point1.x,triangle_dst[i].Point2.x,triangle_dst[i].Point3.x);
			max_x = max(triangle_dst[i].Point1.x,triangle_dst[i].Point2.x,triangle_dst[i].Point3.x);
			min_y = min(triangle_dst[i].Point1.y,triangle_dst[i].Point2.y,triangle_dst[i].Point3.y);
			max_y = max(triangle_dst[i].Point1.y,triangle_dst[i].Point2.y,triangle_dst[i].Point3.y);
			trg.push_back(triangle_dst[i].Point1);
			trg.push_back(triangle_dst[i].Point2);
			trg.push_back(triangle_dst[i].Point3);
			for(int j=min_x;j<=max_x;j++)
			{
				for(int k=min_y;k<=max_y;k++)
				{
					if(IN_TRIANGLE(trg,Point2f(j,k)))
					{
						Point2f point_src;
						point_src.x	= Aff.at<float>(0,0)*(float)j+Aff.at<float>(0,1)*(float)k+Aff.at<float>(0,2);
						point_src.y	= Aff.at<float>(1,0)*(float)j+Aff.at<float>(1,1)*(float)k+Aff.at<float>(1,2);
						point_src.x = point_src.x>img_src.rows-1? img_src.rows-1:point_src.x;
						point_src.x = point_src.x<0? 0:point_src.x;
						point_src.y = point_src.y>img_src.cols-1? img_src.cols-1:point_src.y;
						point_src.y = point_src.y<0? 0:point_src.y;
						img_mid.ptr<uchar>(j)[k]=insert_value(img_src,point_src);
					}					
				}
			}
		}
	}
Mat Mesh_Morphing::Mesh_Morphing_Procedure(float rate)
	{
		vector<Triangle> triangle_mid(triangle_src.size());
	
		for(int i=0;i<triangle_src.size();i++)
		{
			triangle_mid[i].Point1.x=triangle_src[i].Point1.x*(1-rate)+triangle_dst[i].Point1.x*rate;
			triangle_mid[i].Point1.y=triangle_src[i].Point1.y*(1-rate)+triangle_dst[i].Point1.y*rate;
			triangle_mid[i].Point2.x=triangle_src[i].Point2.x*(1-rate)+triangle_dst[i].Point2.x*rate;
			triangle_mid[i].Point2.y=triangle_src[i].Point2.y*(1-rate)+triangle_dst[i].Point2.y*rate;
			triangle_mid[i].Point3.x=triangle_src[i].Point3.x*(1-rate)+triangle_dst[i].Point3.x*rate;
			triangle_mid[i].Point3.y=triangle_src[i].Point3.y*(1-rate)+triangle_dst[i].Point3.y*rate;
		}
		Mat temp1=Mat::zeros(img_src.rows,img_src.cols,CV_8U);
		Affine(triangle_src,triangle_mid,img_src,temp1);
		Mat temp2=Mat::zeros(img_src.rows,img_src.cols,CV_8U);
		Affine(triangle_dst,triangle_mid,img_dst,temp2);
		Mat temp= temp1*(1-rate)+temp2*rate;
		return temp;
	}
static void onMousesrc(int event, int x, int y, int flags, void* param)  
	{
		Mesh_Morphing *im = reinterpret_cast<Mesh_Morphing*>(param); 
		switch (event)  
		{
			case CV_EVENT_LBUTTONDOWN:     //鼠标左键按下响应：返回坐标
				im->feature_src.push_back(Point2f(y,x));	
				circle(im->img_src_clone,Point2f(x,y),2,Scalar(255,255,255),-1,8,0);
				cout<<"在源图中已经选择了"<<im->feature_src.size()-4<<"个特征点"<<endl;
				imshow("image_src",im->img_src_clone);
				break;
			default:
				break;
		 }	
	}
static void onMousedst(int event, int x, int y, int flags, void* param)  
	{
		Mesh_Morphing *im = reinterpret_cast<Mesh_Morphing*>(param); 
		switch (event)  
		{  
		    case CV_EVENT_LBUTTONDOWN:     //鼠标左键按下响应：返回坐标
				im->feature_dst.push_back(Point2f(y,x));
				circle(im->img_dst_clone,Point2f(x,y),2,Scalar(255,255,255),-1,8,0);
				imshow("image_dst",im->img_dst_clone);
				cout<<"在目标图中已经选择了"<<im->feature_dst.size()-4<<"个特征点"<<endl;
				break;
			default:
				break;
		}
	}