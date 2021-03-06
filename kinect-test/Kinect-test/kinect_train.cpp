/*
Kinect_Train.cpp: 定义控制台应用程序的入口点。
手势识别项目 手势指令模板录入工程项目
这个主函数有以下功能：
1 传感器初始化（包括sensor，souse等）。详细见Initialization()
2 打开文件流（待检查is_open）
3 读取kinect彩色帧数据。ColorSign(Mat color)
4 读取kinect人体骨骼数据，并标记节点及骨骼。DrawBodypoints，需要坐标转换
5 显示标记出人体骨骼节点的彩色图像imshow
6 保存并计算前后两帧之间读取的人体骨骼数据的相对变化Cal_Frame
7 根据Cal_Frame所得到的结果，对视频流进行分帧，提取出可能为指令的部分，并保存到文件中
文件路径为doc = "F:\\ZB\\Desktop\\毕设\\Kinect\\Practical\\Data\\data.txt"
提取的部分保存在vector myJointData_ALL中，在输出至文件后便清空，以便存储下一个手势指令
8 关闭文件流fout
9 释放为保存手势指令而new出的内存空间。ReleasePoint
10 释放传感器（包括sensor，sourse等）。详细见Release()
*/

#include <fstream>
#include <cstring>
#include <vector>
#include "head.h"
#include "data.h"
#define _CRT_SECURE_NO_WARNINGS
using namespace std;
using namespace cv;


int main()
{
	string doc = "F:\\ZB\\Desktop\\毕设\\Kinect\\Practical\\Data\\Data.txt";//模板数据的保存路径
	ofstream fout;
	Initialization();
	myJointData_ALL.clear();
	Mat color(colorHeight, colorWidth, CV_8UC4);//彩色图像
	Mat body(colorHeight, colorWidth, CV_8UC4);//人体图像
	myJointFramenum = 0;//用于计算保存的帧数
	bool frame_start = FALSE;//整个程序开始
	bool gesture_start = FALSE;//录入状态
	bool CalFrameflag = FALSE;//帧间计算判定结果,大于阈值时返回TRUE，小于阈值时返回FALSE
	bool GestureFinish = FALSE;//是否提取出一个完整的指令
	fout.open(doc);
	while (1)
	{
		if (frame_start) CopyJoint(myJointArr, myJointArrcopy);//保存上一帧的数据(本帧，上帧)
		ColorSign(color);//显示彩色图像
		body = color.clone();
		DrawBodypoints(body, myMapper);//画人体骨骼
		CalFrameflag = Cal_Frame(myJointArrcopy, myJointArr);//帧间相对变化的计算

															 //开始分帧
		if (myJointFramenum >= OrderLength)//每个手势不能超过200帧
		{
			cout << "ERROR!" << endl;
			break;
		}
		else
		{
			GestureFinish = GetmyGesture_Train(gesture_start, CalFrameflag, myJointArr, myJointData_ALL);
			if (GestureFinish)
			{
				WriteData(myJointData_ALL, fout);
				GestureFinish = FALSE;
				myJointData_ALL.clear();
				fout << "Get Gesture:" << myJointFramenum << std::endl << std::endl;//输出读取的帧数
				myJointFramenum = 0;
			}
		}
		resize(body, body, Size(800, 500));
		imshow("Body", body);//检测并显示人体骨骼图
		if (waitKey(30) == VK_ESCAPE)
			break;
		frame_start = TRUE;//尚需要确认是否是一帧间隔
		GetGesture = FALSE;
	}
	myJointData_ALL.clear();
	ReleasePoint(p, myJointFramenum);
	Release();
	fout.close();
	system("pause");
	return 0;
}


