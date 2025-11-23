package android.app.math;

import android.os.RemoteException;
import android.annotation.SystemService;
import android.content.Context;
import android.util.Log;
import android.annotation.Nullable;

/** @hide */
@SystemService(Context.MATH_SERVICE)
public final class MathServiceManager {

    private static final String TAG = "MathServiceManager";
    private final IMathServiceManager mService;
    private Context mContext;

    /** @hide */
    public MathServiceManager(Context context, IMathServiceManager service) {
        mContext = context;
        mService = service;
    }

    /** @hide */
    public void initServiceName() {
        try {
            mService.initServiceName();
        } catch (RemoteException ex) {
            Log.e(TAG, "Unable to get math service");
        }
    }

    /** @hide */
    public int add(int a, int b) {
        try {
            return mService.add(a, b);
        } catch (RemoteException ex) {
            Log.e(TAG, "Unable to get add math service");
            return 0;

        }
    }

    /** @hide */
    public int sub(int a, int b) {
        try {
            return mService.sub(a, b);
        } catch (RemoteException ex) {
            Log.e(TAG, "Unable to get sub math service");
            return 0;

        }
    }

    /** @hide */
    public int multiply(int a, int b) {
        try {
            return mService.multiply(a, b);
        } catch (RemoteException ex) {
            Log.e(TAG, "Unable to get multiply math service");
            return 0;

        }
    }

    /** @hide */
    @Nullable
    public String getServiceName() {
        try {
            return mService.getServiceName();
        } catch (RemoteException ex) {
            Log.e(TAG, "Unable to get getServiceName");
            return "ERROR";
        }
    }
}