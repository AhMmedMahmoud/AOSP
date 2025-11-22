import com.android.server.sensortask.SensorTaskService;

private void startOtherServices(@NonNull TimingsTraceAndSlog t) {
    ....
    t.traceBegin("SensorTaskService");
    try {
        mSystemServiceManager.startService(SensorTaskService.class);
    } catch (Throwable e) {
        reportWtf("starting SensorTaskService", e);
    }
    t.traceEnd();
    ....
}