#include<opencv2\opencv.hpp>
#include<stdio.h>
#include<math.h>
#include<vector>
#include"Mesh_Morphing.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include<fstream>

using namespace std;
using namespace cv;


int main()
{
	Mat img_src=imread("test2.jpg",0);
	Mat img_dst=imread("test1.jpg",0);
	imshow("src",img_src);
	imshow("dst",img_dst);
	cout<<"现在展示的分别是源图像(src)和目标图像(dst),按任意键进行下一步选择"<<endl;
	waitKey(0);
	destroyAllWindows();
	string str;

	Mesh_Morphing MM(img_src,img_dst);
	int flag1;
	int flag2 =1;
	int flag3 =1;
	string src,dst;
	while(flag2)
	{
	cout<<"是否要从已有文件选择特征点？"<<endl<<"1.是"<<endl<<"2.否"<<endl;
	cin>>flag1;
	switch (flag1)
	{
	case 1:
		src="feature_src";
		dst="feature_dst";
		MM.get_feature(src,dst);
		flag2=0;
		break;
	case 2:
		while(flag3)
		{
		cout<<"请选择特征点"<<endl<<"注意：选择特征点时，顺序和数目一定要对应！"<<endl;
		MM.feature_dst.clear();
		MM.feature_src.clear();
		MM.get_feature();
		if(MM.feature_dst.size()==MM.feature_src.size())
			flag3=0;
		else
		{
			cout<<"特征点数量不等！！！"<<endl;
			flag3=1;
		}
		}
		flag2=0;
		break;
	default:
		flag2=1;
		break;
	}
	}

	ofstream feature_src,feature_dst;

	MM.Change_to_Triangle();
	vector<Mat> mid;
	int number;
	cout<<"需要生成多少张图片？"<<endl;
	cin>>number;
	for(int i=0;i<=number-1;i++)
	{
		mid.push_back(MM.Mesh_Morphing_Procedure((float)i/(number-1)));
		cout<<"正在生成第"<<i<<"张图片"<<endl;
	}
	
		cout<<"即将展示共"<<mid.size()<<"张照片"<<endl;
	for(int i=0;i<mid.size();i++)
	{
		str = "mid"+to_string(i)+".jpg";
		imshow("mid",mid[i]);
		imwrite(str,mid[i]);
		waitKey(1000);
	}
	destroyAllWindows();
	cout<<"图片展示完毕，可在目录下查看保存的图片"<<endl;
	flag2=1;
	if(flag1==2)
	{
	while(flag2)
	{
	cout<<"是否保存刚刚选择的特征点？"<<endl<<"1.是"<<endl<<"2.否"<<endl;
	cin>>flag3;
	switch (flag3)
	{
	case 1:
		feature_src.open("feature_src.txt");
		for(int i=0;i<MM.feature_src.size();i++)
		{
			feature_src<<MM.feature_src[i].x<<endl;
			feature_src<<MM.feature_src[i].y<<endl;
		}
			feature_src.close();	
		feature_dst.open("feature_dst.txt");
		for(int i=0;i<MM.feature_dst.size();i++)
		{
			feature_dst<<MM.feature_dst[i].x<<endl;
			feature_dst<<MM.feature_dst[i].y<<endl;
		}
		feature_dst.close();
		flag2=0;
		break;
	case 2:
		flag2=0;
		break;
	default:
		flag2=1;
		break;
	}
	}
	}
	waitKey(0);
	return 0;
}

