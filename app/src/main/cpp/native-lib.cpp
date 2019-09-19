#include <jni.h>
#include <string>
#include <android/log.h>
#include <android/bitmap.h>
#include <opencv2/opencv.hpp>
#include "cvcanny.h"

#define LOG_TAG "System.out"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

//将Mat转换为bitmap
jobject mat2bitmap(JNIEnv *env, cv::Mat &src, bool needPremultiplyAlpha, jobject bitmap_config) {
    jclass java_bitmap_class = (jclass) env->FindClass("android/graphics/Bitmap");
    jmethodID mid = env->GetStaticMethodID(java_bitmap_class, "createBitmap",
                                           "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
    jobject bitmap = env->CallStaticObjectMethod(java_bitmap_class,
                                                 mid, src.size().width, src.size().height,
                                                 bitmap_config);
    AndroidBitmapInfo info;
    void *pixels = 0;

    try {
        CV_Assert(AndroidBitmap_getInfo(env, bitmap, &info) >= 0);
        CV_Assert(src.type() == CV_8UC1 || src.type() == CV_8UC3 || src.type() == CV_8UC4);
        CV_Assert(AndroidBitmap_lockPixels(env, bitmap, &pixels) >= 0);
        CV_Assert(pixels);

        if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
            cv::Mat tmp(info.height, info.width, CV_8UC4, pixels);
            if (src.type() == CV_8UC1) {
                cvtColor(src, tmp, cv::COLOR_GRAY2RGBA);
            } else if (src.type() == CV_8UC3) {
                cvtColor(src, tmp, cv::COLOR_RGB2BGRA);
            } else if (src.type() == CV_8UC4) {
                if (needPremultiplyAlpha) {
                    cvtColor(src, tmp, cv::COLOR_RGBA2mRGBA);
                } else {
                    src.copyTo(tmp);
                }
            }
        } else {
            // info.format == ANDROID_BITMAP_FORMAT_RGB_565
            cv::Mat tmp(info.height, info.width, CV_8UC2, pixels);
            if (src.type() == CV_8UC1) {
                cvtColor(src, tmp, cv::COLOR_GRAY2BGR565);
            } else if (src.type() == CV_8UC3) {
                cvtColor(src, tmp, cv::COLOR_RGB2BGR565);
            } else if (src.type() == CV_8UC4) {
                cvtColor(src, tmp, cv::COLOR_RGBA2BGR565);
            }
        }
        AndroidBitmap_unlockPixels(env, bitmap);
        return bitmap;
    } catch (cv::Exception e) {
        AndroidBitmap_unlockPixels(env, bitmap);
        jclass je = env->FindClass("org/opencv/core/CvException");
        if (!je) je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, e.what());
        return bitmap;
    } catch (...) {
        AndroidBitmap_unlockPixels(env, bitmap);
        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, "Unknown exception in JNI code {nMatToBitmap}");
        return bitmap;
    }
}


extern "C"
JNIEXPORT jobject JNICALL
Java_dem_vac_surfaceviewdemo_VaccaeOpenCVJNI_getCameraframebitbmp(JNIEnv *env, jclass type,
                                                                  jobject bmp) {

    AndroidBitmapInfo bitmapInfo;
    void *pixelscolor;
    int ret;

    //获取图像信息，如果返回值小于0就是执行失败
    if ((ret = AndroidBitmap_getInfo(env, bmp, &bitmapInfo)) < 0) {
        LOGI("AndroidBitmap_getInfo failed! error-%d", ret);
        return NULL;
    }

    //判断图像类型是不是RGBA_8888类型
    if (bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGI("BitmapInfoFormat error");
        return NULL;
    }

    //获取图像像素值
    if ((ret = AndroidBitmap_lockPixels(env, bmp, &pixelscolor)) < 0) {
        LOGI("AndroidBitmap_lockPixels() failed ! error=%d", ret);
        return NULL;
    }

    //生成源图像
    cv::Mat src(bitmapInfo.height, bitmapInfo.width, CV_8UC4, pixelscolor);

    //生成Canny图像
//    dealcanny(src);

    //获取原图片的参数
    jclass java_bitmap_class = (jclass) env->FindClass("android/graphics/Bitmap");
    jmethodID mid = env->GetMethodID(java_bitmap_class, "getConfig",
                                     "()Landroid/graphics/Bitmap$Config;");
    jobject bitmap_config = env->CallObjectMethod(bmp, mid);
    //将SRC转换为图片
    jobject _bitmap = mat2bitmap(env, src, false, bitmap_config);

    AndroidBitmap_unlockPixels(env, bmp);


    return _bitmap;
}

extern "C"
JNIEXPORT jobject JNICALL
Java_dem_vac_surfaceviewdemo_VaccaeOpenCVJNI_Cameraframetouchgetbitbmps(JNIEnv *env, jclass clazz,
                                                                        jobject bmp, jobject points,
                                                                        jboolean isovertouch) {
    AndroidBitmapInfo bitmapInfo;
    void *pixelscolor;
    int ret;

    //获取图像信息，如果返回值小于0就是执行失败
    if ((ret = AndroidBitmap_getInfo(env, bmp, &bitmapInfo)) < 0) {
        LOGI("AndroidBitmap_getInfo failed! error-%d", ret);
        return NULL;
    }

    //判断图像类型是不是RGBA_8888类型
    if (bitmapInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGI("BitmapInfoFormat error");
        return NULL;
    }

    //获取图像像素值
    if ((ret = AndroidBitmap_lockPixels(env, bmp, &pixelscolor)) < 0) {
        LOGI("AndroidBitmap_lockPixels() failed ! error=%d", ret);
        return NULL;
    }

    //生成源图像
    cv::Mat src(bitmapInfo.height, bitmapInfo.width, CV_8UC4, pixelscolor);

    //获取ArrayList类引用
    jclass list_jcls = env->FindClass("java/util/ArrayList");
    if (list_jcls == NULL) {
        LOGI("ArrayList没找到相关类!");
        return bmp;
    }
    //获取ArrayList构造函数id
    jmethodID list_init = env->GetMethodID(list_jcls, "<init>", "()V");
    //创建一个ArrayList对象
    jobject list_obj = env->NewObject(list_jcls, list_init, "");
    //获取ArrayList对象的add()的methodID
    jmethodID list_add = env->GetMethodID(list_jcls, "add", "(Ljava/lang/Object;)Z");

    //获取ArrayList对象的get()的methodID
    jmethodID list_get = env->GetMethodID(list_jcls, "get", "(I)Ljava/lang/Object;");
    //获取ArrayList对象的size()的methodID
    jmethodID list_size = env->GetMethodID(list_jcls, "size", "()I");
    //然后获取我们的Point类的class
    jclass jcls = env->FindClass("android/graphics/Point");
    if (jcls == NULL) {
        return bmp;
    }
    //获取Point的值
    jfieldID pointx = env->GetFieldID(jcls, "x", "I");
    jfieldID pointy = env->GetFieldID(jcls, "y", "I");

    //获取集合的里的个数
    int size= env->CallIntMethod(points,list_size);

    //定义开始和结束的Point
    cv::Point pointstart;
    cv::Point pointend;
    //只取第一个启始点和最后一个结束点
    if (size < 1) {
        return bmp;
    } else {
        //获取启始点的Point
        jobject item = env->CallObjectMethod(points, list_get, 0);
        pointstart = cv::Point(env->GetIntField(item, pointx), env->GetIntField(item, pointy));

        //获取结束点的Point
        item = env->CallObjectMethod(points, list_get, size - 1);
        pointend = cv::Point(env->GetIntField(item, pointx), env->GetIntField(item, pointy));
    }

    //备份源图像
    cv::Mat tmpsrc;
    src.copyTo(tmpsrc);

    //画矩形框
    cv::rectangle(src, pointstart, pointend, cv::Scalar(255, 0, 0),3);

    //获取原图片的参数
    jclass java_bitmap_class = (jclass) env->FindClass("android/graphics/Bitmap");
    jmethodID mid = env->GetMethodID(java_bitmap_class, "getConfig",
                                     "()Landroid/graphics/Bitmap$Config;");
    jobject bitmap_config = env->CallObjectMethod(bmp, mid);
    //将SRC转换为图片
    jobject _bitmap = mat2bitmap(env, src, false, bitmap_config);
    //将源图先插入到返回列表里
    env->CallBooleanMethod(list_obj, list_add, _bitmap);

    //如果画框完成，截取图像
    if (isovertouch) {
        cv::Rect rect(pointstart, pointend);
        cv::Mat dst= tmpsrc(rect);
        jobject dstbmp = mat2bitmap(env, dst, false, bitmap_config);
        env->CallBooleanMethod(list_obj, list_add, dstbmp);
    }

    AndroidBitmap_unlockPixels(env, bmp);


    return list_obj;
}