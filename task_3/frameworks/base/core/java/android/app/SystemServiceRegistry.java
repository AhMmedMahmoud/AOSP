import android.app.sensortask.SensorTaskServiceManager;
import android.app.sensortask.ISensorTaskService;

static {
    ....
    ....
    ....
    registerService(Context.SENSOR_TASK_SERVICE, SensorTaskServiceManager.class,
                new CachedServiceFetcher<SensorTaskServiceManager>() {
                        @Override
                        public SensorTaskServiceManager createService(ContextImpl ctx) {
                            IBinder b = ServiceManager.getService(
                                        Context.SENSOR_TASK_SERVICE);
                            ISensorTaskService service = 
                                        ISensorTaskService.Stub.asInterface(b);
                            return new SensorTaskServiceManager(service);
                        }
                    });
    ....
    ....
    ....
}