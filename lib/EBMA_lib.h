#ifndef __EBMA_LIB_H__
#define __EBMA_LIB_H__

#include <iostream>
#include <math.h>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <time.h>
#include <chrono>
#include <vector>
void EMBA(cv::Mat &current_frame,cv::Mat &ref_frame,cv::Mat &predict,short search_size,short block_size);
int Check_boundry(int now_pos,int search_size,int boundry);
int Check_index_boundry(int index,int upper_boundry,int low_boundry);
void Set_MV_color(cv::Mat &MV,cv::Mat &colorSpace,cv::Point &mv,cv::Point &set_pos,short  block_size);




#endif
