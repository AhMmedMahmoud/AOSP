package com.mozcelikors.eap;

import android.animation.ObjectAnimator;
import android.app.Activity;
import android.app.Application;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.TransitionDrawable;
import android.os.Build;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.view.View;
import android.view.Window;
import android.widget.ImageView;
import android.widget.Toast;
import android.widget.VideoView;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.annotation.RequiresApi;

import com.mozcelikors.eap.R;
import com.mozcelikors.eap.AppListLinearLayout;

import java.io.IOException;

public class MainActivity extends Activity implements Application.ActivityLifecycleCallbacks {

    private SlidingMenu mSlidingMenu;
    private AppListLinearLayout mAppListLinearLayout;
    private ImageView mCevizLogoImageView;
    private CountDownTimer mVideoPauseTimer;

    @RequiresApi(api = Build.VERSION_CODES.Q)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        registerActivityLifecycleCallbacks(this);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);
        mSlidingMenu = (SlidingMenu) findViewById(R.id.slidingmenu);

        // Animations to logo
        mCevizLogoImageView = (ImageView) findViewById(R.id.cevizlogoimageview);
        mCevizLogoImageView.setScaleX(2.0f);
        mCevizLogoImageView.setScaleY(2.0f);
        mCevizLogoImageView.setTranslationY(300f);
        mCevizLogoImageView.setAlpha(0f);
        ObjectAnimator animation1 = ObjectAnimator.ofFloat(mCevizLogoImageView, "scaleX", 1f);
        ObjectAnimator animation2 = ObjectAnimator.ofFloat(mCevizLogoImageView, "scaleY", 1f);
        ObjectAnimator animation3 = ObjectAnimator.ofFloat(mCevizLogoImageView, "translationY", 0f);
        ObjectAnimator animation4 = ObjectAnimator.ofFloat(mCevizLogoImageView, "alpha", 1f);
        animation1.setDuration(2000);
        animation2.setDuration(2000);
        animation3.setDuration(2000);
        animation4.setDuration(1500);
        animation1.start();
        animation2.start();
        animation3.start();
        animation4.start();

        // Timer for video pausing
        mVideoPauseTimer = new CountDownTimer(15000, 5000) {
            public void onTick(long millisUntilFinished) {

            }

            public void onFinish() {
                VideoView videoView = (VideoView) findViewById(R.id.videoView);
                videoView.pause();
            }
        };

        // Run video
        VideoView videoView = (VideoView) findViewById(R.id.videoView);  //casting to VideoView is not Strictly required above API level 26
        videoView.setVideoPath("android.resource://" + getPackageName() + "/" + R.raw.waves); //set the path of the video that we need to use in our VideoView
        videoView.start();  //start() method of the VideoView class will start the video to play
        mVideoPauseTimer.cancel();
        mVideoPauseTimer.start();

        mAppListLinearLayout = (AppListLinearLayout) findViewById(R.id.applist1);
        mAppListLinearLayout.fillLayoutWithAppIcons(this.getResources().getConfiguration());
    }

    @Override
    public void onActivityCreated(@NonNull Activity activity, @Nullable Bundle savedInstanceState) {

    }

    @Override
    public void onActivityStarted(@NonNull Activity activity) {

    }

    @Override
    public void onActivityResumed(Activity activity) {
        VideoView videoView = (VideoView) findViewById(R.id.videoView);
        videoView.start();
        mVideoPauseTimer.cancel();
        mVideoPauseTimer.start();
    }

    @Override
    public void onActivityPaused(@NonNull Activity activity) {
        VideoView videoView = (VideoView) findViewById(R.id.videoView);
        videoView.pause();
        mVideoPauseTimer.cancel();
    }

    @Override
    public void onActivityStopped(@NonNull Activity activity) {
        VideoView videoView = (VideoView) findViewById(R.id.videoView);
        videoView.pause();
        mVideoPauseTimer.cancel();
    }

    @Override
    public void onActivitySaveInstanceState(@NonNull Activity activity, @NonNull Bundle outState) {

    }

    @Override
    public void onActivityDestroyed(@NonNull Activity activity) {
        VideoView videoView = (VideoView) findViewById(R.id.videoView);
        videoView.pause();
        mVideoPauseTimer.cancel();
    }

}
