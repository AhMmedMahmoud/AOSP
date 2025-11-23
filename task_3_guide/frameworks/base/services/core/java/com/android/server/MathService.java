package com.android.server.math;

import android.app.math.IMathServiceManager;
import android.content.Context;
import android.os.RemoteException;
import android.util.Log;
import android.content.Context;
import com.android.server.SystemService;

public class MathService extends IMathServiceManager.Stub {
 private final static String LOG_TAG = "MathService";
 private final Context mContext;

 public MathService(Context context) {
    mContext = context;
 }

 @Override
 public void initServiceName() throws RemoteException {
    Log.i(LOG_TAG,"init Service Running!");
 }

 @Override
 public int add(int a, int b) throws RemoteException {
    return a + b;
 }

 @Override
 public int sub(int a, int b) throws RemoteException {
    return a - b;
 }

 @Override
 public int multiply(int a, int b) throws RemoteException {
    return a * b;
 }

 @Override
 public String getServiceName() throws RemoteException {
    return "Math Service";
 }
}
