import com.android.server.math.MathService;

private void startOtherServices(@NonNull TimingsTraceAndSlog t) {
    ....
    t.traceBegin("MathService");
    try {
        MathService math = new MathService(context);
        ServiceManager.addService(context.MATH_SERVICE, math);
    } catch (Throwable e) {
        reportWtf("starting MathService", e);
    }
    t.traceEnd();
    ....
}