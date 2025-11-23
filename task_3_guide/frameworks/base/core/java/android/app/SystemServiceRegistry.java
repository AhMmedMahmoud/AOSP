import android.app.math.MathServiceManager;
import android.app.math.IMathServiceManager;

static {
    ....
    ....
    ....
    registerService(Context.MATH_SERVICE, MathServiceManager.class,
            new CachedServiceFetcher < MathServiceManager > () {
                @Override
                public MathServiceManager createService(ContextImpl context) 
                        throws ServiceNotFoundException {
                    IBinder b = ServiceManager.getServiceOrThrow(
                            Context.MATH_SERVICE);
                    IMathServiceManager service =
                            IMathServiceManager.Stub.asInterface(b);
                    return new MathServiceManager(context, service);
                }
            });
    ....
    ....
    ....
}