package dem.vac.surfaceviewdemo;

import android.content.Context;
import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.ImageView;



import java.util.List;

/**
 * 作者：Vaccae
 * 邮箱：3657447@qq.com
 * 创建时间：2019-09-11 14:00
 * 功能模块说明：
 */
public class ImageAdapter extends RecyclerView.Adapter<RecyclerView.ViewHolder> {

    public List<CImage> mImages;

    private Context mContext;

    public ImageAdapter(Context context, List<CImage> images) {
        mContext=context;
        mImages=images;
    }

    @NonNull
    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {

        View v=LayoutInflater.from(mContext).inflate(R.layout.rcl_image, parent, false);
        return new imgviewHolder(v);
    }

    @Override
    public void onBindViewHolder(@NonNull RecyclerView.ViewHolder holder, int position) {
        final imgviewHolder imgvholder=(imgviewHolder) holder;

        final CImage item=mImages.get(position);

        imgvholder.imgv.setImageBitmap(item.bmp);
        imgvholder.imgv.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                item.ischeck=!item.ischeck;
                imgvholder.imgcheck.setChecked(item.ischeck);
            }
        });
        imgvholder.imgcheck.setChecked(item.ischeck);
        imgvholder.imgcheck.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                item.ischeck=b;
            }
        });
    }

    @Override
    public int getItemCount() {
        return mImages.size();
    }


    public static class imgviewHolder extends RecyclerView.ViewHolder {

        public ImageView imgv;
        public CheckBox imgcheck;

        public imgviewHolder(@NonNull View itemView) {
            super(itemView);

            imgv=itemView.findViewById(R.id.item_imgsrc);
            imgcheck=itemView.findViewById(R.id.item_check);
        }
    }
}
