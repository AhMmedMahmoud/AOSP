package com.example.mathclient;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import android.app.math.MathServiceManager;
import android.content.Context;

public class MainActivity extends Activity {

    private static final String TAG = "MathTest";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        TextView tv = new TextView(this);
        tv.setText("Running MathService Test…");
        setContentView(tv);

        MathServiceManager manager =
        (MathServiceManager) getSystemService(Context.MATH_SERVICE);

        int sum = manager.add(5, 3);
        int subtract = manager.sub(5, 3);
        int multiply = manager.multiply(5, 3);
        String serviceName = manager.getServiceName();


        update(tv, 
            "Service Name: " + serviceName + "\n" +
            "Result of Sum = " + sum + "\n" +
            "Result of Subtract = " + subtract + "\n" +
            "Result of Multiply = " + multiply + "\n"
        );
    }

    private void update(TextView tv, String text) {
        runOnUiThread(() -> tv.setText(text));
        Log.i(TAG, text);
    }
}
