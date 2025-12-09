package com.mozcelikors.eap;

import android.animation.ObjectAnimator;
import android.annotation.SuppressLint;

import android.app.Activity;

import android.content.Context;
import android.content.ContextWrapper;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.res.Configuration;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;

import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.util.Log;

import android.view.Display;
import android.view.Gravity;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class AppListLinearLayout extends LinearLayout {

    private PackageManager mPm;
    private Context mContext;
    private Activity mActivity;
    private int mScreenWidth;
    private int mScreenHeight;
    private List<String> mAppNameList = Arrays.asList(
            "Calendar",
            "Clock",
            "Gallery",
            "Settings",
            "Video",
            "Calculator",
            "Contacts",
            "Files",
            "Power Off",
            "Search",
            "Camera",
            "Explorer",
            "Sound Recorder",
            "Lightning",
            "Music");

    public void initObjects(Context context)
    {
        mPm = context.getPackageManager();
        mContext = context;
        mActivity = getActivity(mContext);

        //Get screen size.
        DisplayMetrics metrics = new DisplayMetrics();
        Display display = mActivity.getWindowManager().getDefaultDisplay();
        display.getMetrics(metrics);
        int screenWidth = 0, screenHeight = 0;
        Method mGetRawH = null, mGetRawW = null;

        try {
            // For JellyBean 4.2 (API 17) and onward
            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.JELLY_BEAN_MR1) {
                display.getRealMetrics(metrics);

                screenWidth = metrics.widthPixels;
                screenHeight = metrics.heightPixels;
            } else {
                mGetRawH = Display.class.getMethod("getRawHeight");
                mGetRawW = Display.class.getMethod("getRawWidth");

                try {
                    screenWidth = (Integer) mGetRawW.invoke(display);
                    screenHeight = (Integer) mGetRawH.invoke(display);
                } catch (IllegalArgumentException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                } catch (IllegalAccessException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                } catch (InvocationTargetException e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                }
            }
        } catch (NoSuchMethodException e3) {
            e3.printStackTrace();
        }
        mScreenWidth = screenWidth;
        mScreenHeight = screenHeight;
    }

    public AppListLinearLayout(Context context) {
        super(context);
        initObjects(context);
    }

    public AppListLinearLayout(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        initObjects(context);
    }

    public AppListLinearLayout(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        initObjects(context);
    }

    public int getScreenWidth()
    {
        return mScreenWidth;
    }

    public int getScreenHeight()
    {
        return mScreenHeight;
    }

    public Activity getActivity(Context context)
    {
        if (context == null)
        {
            return null;
        }
        else if (context instanceof ContextWrapper)
        {
            if (context instanceof Activity)
            {
                return (Activity) context;
            }
            else
            {
                return getActivity(((ContextWrapper) context).getBaseContext());
            }
        }

        return null;
    }

    public List<ApplicationInfo> getInstalledApplicationList() {
        // Get list of apps
        PackageManager pm = mContext.getPackageManager();
        List<ApplicationInfo> apps = pm.getInstalledApplications(0);
        // Limit list to only installed apps
        List<ApplicationInfo> installedApps = new ArrayList<ApplicationInfo>();
        for(ApplicationInfo app : apps) {
            //checks for flags; if flagged, check if updated system app
            /*if((app.flags & ApplicationInfo.FLAG_UPDATED_SYSTEM_APP) != 0) {
                installedApps.add(app);
                //it's a system app, not interested
            } else if ((app.flags & ApplicationInfo.FLAG_SYSTEM) != 0) {
                //Discard this one
                //in this case, it should be a user-installed app
                if ((app.flags & ApplicationInfo.FLAG_TEST_ONLY) != 0)
                {
                    installedApps.add(app);
                }
            } else {
                //installedApps.add(app);
            }*/
            if ((app.flags & ApplicationInfo.FLAG_SUSPENDED) == 0) {
                installedApps.add(app);
            }
        }
        return installedApps;
    }

    private Drawable getAppIconFromIndex (List<ApplicationInfo> installedApps, int i, Configuration config)
    {
        Drawable icon = mPm.getApplicationIcon(installedApps.get(i));
        return icon;
    }

    private CharSequence getAppLabelFromIndex(List<ApplicationInfo> installedApps, int i)
    {
        return mPm.getApplicationLabel(installedApps.get(i));
    }

    private Intent getAppLaunchIntentFromIndex (List<ApplicationInfo> installedApps, int i)
    {
        return mPm.getLaunchIntentForPackage(installedApps.get(i).packageName);
    }

    public void fillLayoutWithAppIcons (Configuration config)
    {
        View mView = mActivity.findViewById(R.id.layout_top);

        int mColumnCount;

        // Declare our view object
        List<ApplicationInfo> installedApps = this.getInstalledApplicationList();

        this.removeAllViews();
        this.setBottom(20);
        this.setGravity(Gravity.CENTER_VERTICAL | Gravity.CENTER_HORIZONTAL);

        int total = installedApps.size();

        for (int i = 0; i < total; i++)
        {
            int margin = 25;
            final LinearLayout linearLayout1 = new LinearLayout(mContext);
            //LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(
            //        LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
            LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(
                    150, 100);
            layoutParams.setMargins (margin, margin, margin, 0);
            linearLayout1.setOrientation(LinearLayout.VERTICAL);

            ImageView oImageView = new ImageView(mContext);
            ImageView oShadowImageView = new ImageView(mContext);

            final Drawable application_icon = getAppIconFromIndex(installedApps, i, config);
            final CharSequence application_label = getAppLabelFromIndex(installedApps, i);
            final Intent application_launchIntent = getAppLaunchIntentFromIndex(installedApps, i);

            Log.d("App name =" , application_label.toString());

            if (application_icon == null)
                return;

            if (mAppNameList.contains(application_label))
            {
                linearLayout1.setId(i);


                oImageView.setImageDrawable(application_icon);
                oImageView.setLayoutParams(new LinearLayout.LayoutParams(215,100));
                oImageView.setClickable(true);
                oImageView.setOnClickListener(new View.OnClickListener() {
                    @Override
                    public void onClick(View v) {
                        mActivity.startActivity(application_launchIntent);
                    }
                });
                linearLayout1.addView(oImageView, layoutParams);

                TextView textView1 = new TextView(mContext);
                textView1.setText(application_label);
                textView1.setTextColor(0xFFFFFFFF);
                textView1.setWidth(oImageView.getWidth());
                textView1.setHeight(20);
                textView1.setGravity(Gravity.CENTER_HORIZONTAL);
                linearLayout1.addView(textView1, layoutParams);

                oShadowImageView.setBackgroundResource(R.drawable.shadow3);
                oShadowImageView.setLayoutParams(new LinearLayout.LayoutParams(50,2));
                linearLayout1.addView(oShadowImageView, layoutParams);

                linearLayout1.setTranslationY(-100f);
                linearLayout1.setAlpha(0f);

                linearLayout1.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        ObjectAnimator animation1 = ObjectAnimator.ofFloat(linearLayout1, "translationY", 0f);
                        animation1.setDuration(1000);
                        ObjectAnimator animation2 = ObjectAnimator.ofFloat(linearLayout1, "alpha", 1f);
                        animation2.setDuration(3000);
                        animation1.start();
                        animation2.start();
                    }
                }, 1000);


                LinearLayout.LayoutParams layoutParamMaster = new LinearLayout.LayoutParams
                        (LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
                this.addView(linearLayout1, layoutParamMaster);
            }

        }
    }
}
