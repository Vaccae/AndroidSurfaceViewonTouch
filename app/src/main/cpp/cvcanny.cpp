//
// Created by 36574 on 2019-06-20.
//

#include "cvcanny.h"

extern "C"
void dealcanny(cv::Mat &input){
    //将图像转为灰度图
    cv::cvtColor(input, input, cv::COLOR_RGBA2GRAY);
    //高斯模糊
    cv::GaussianBlur(input, input, cv::Size(3, 3), 0.5, 0.5);
    //边缘检测
    cv::Canny(input, input, 40, 120);
}