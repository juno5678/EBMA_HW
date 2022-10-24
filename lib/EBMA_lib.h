#ifndef __EBMA_LIB_H__
#define __EBMA_LIB_H__

#include <iostream>
#include <math.h>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <time.h>
#include <chrono>
#include <vector>
void EMBA(cv::Mat &current_frame,cv::Mat &ref_frame,cv::Mat &predict,int search_size,int block_size);

#endif
