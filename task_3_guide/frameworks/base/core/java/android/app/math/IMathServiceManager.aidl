package android.app.math;
/**
 * System private API for communicating with the Math Service.
 * {@hide}
 */
interface IMathServiceManager {
    int add(int a, int b);
    int sub(int a, int b);
    int multiply(int a, int b);
    void initServiceName();
    String getServiceName();
}