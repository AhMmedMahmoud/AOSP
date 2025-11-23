package com.example.mathclient;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;
import android.content.Context;
import android.app.math.MathServiceManager;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        TextView tv = new TextView(this);
        setContentView(tv);

        MathServiceManager math =
            (MathServiceManager) getSystemService(Context.MATH_SERVICE);

        if (math != null) {
            tv.setText(
                "Service Name: " + math.getServiceName() + "\n\n" +
                "5 + 3 = " + math.add(5, 3) + "\n" +
                "10 - 4 = " + math.sub(10, 4) + "\n" +
                "3 × 7 = " + math.multiply(3, 7)
            );
        } else {
            tv.setText("MathServiceManager = null (not running as priv-app?)");
        }
    }
}
