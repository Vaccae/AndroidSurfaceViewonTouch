package dem.vac.surfaceviewdemo;

import android.graphics.Bitmap;
import android.graphics.Point;

import java.util.List;

/**
 * 作者：Vaccae
 * 邮箱：3657447@qq.com
 * 创建时间：2019-06-19 11:29
 * 功能模块说明：
 */
    public class VaccaeOpenCVJNI {

    static {
        System.loadLibrary("native-lib");
    }

    public native static Bitmap getCameraframebitbmp(Bitmap bmp);

    public native static List<Bitmap> Cameraframetouchgetbitbmps(Bitmap bmp, List<Point> points, boolean isovertouch);
}
