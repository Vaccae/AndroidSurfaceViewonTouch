package dem.vac.surfaceviewdemo;

import android.Manifest;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity implements BitmapCallBack {

    private VaccaeSurfaceView surfaceView;
    private RelativeLayout surfaceviewlayout;

    private LinearLayoutManager layoutManager;
    private List<CImage> mImages;
    private ImageAdapter imgadpt;
    private RecyclerView rclview;


    private void InitRecyclerView() {
        //初始化Recyclerview数据
        mImages=new ArrayList<>();
        rclview=findViewById(R.id.recyclerview_img);
        imgadpt=new ImageAdapter(this, mImages);
        //设置布局管理器 , 将布局设置成纵向
        layoutManager = new LinearLayoutManager(this, LinearLayout.HORIZONTAL, false);
        layoutManager.setOrientation(LinearLayout.HORIZONTAL);
        rclview.setLayoutManager(layoutManager);
        rclview.setAdapter(imgadpt);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        requestPermission();

        InitRecyclerView();

        surfaceviewlayout=findViewById(R.id.surfaceviewlayout);

        surfaceView=new VaccaeSurfaceView(MainActivity.this);
        surfaceviewlayout.addView(surfaceView);

    }


    void requestPermission(){
        final int REQUEST_CODE = 1;
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{
                            Manifest.permission.CAMERA, Manifest.permission.WRITE_EXTERNAL_STORAGE},
                    REQUEST_CODE);
        }
    }

    @Override
    public void CallBackOver(Bitmap... bmps) {
        Bitmap bmp=bmps[0];
        CImage item=new CImage();
        item.bmp=bmp;
        item.ischeck=true;

        mImages.add(item);
        imgadpt.notifyDataSetChanged();
    }
}
