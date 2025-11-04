#include <aidl/example/service/api/IExampleService.h>
#include <android/binder_manager.h>
#include <utils/Log.h>
#include <stdio.h>
#include <stdlib.h>

using namespace ::aidl::example::service::api;;

int main ()
{
	ALOGW("Example Service user app started.\n");
	
	const char* serviceName = "example.service.api.IExampleService/default";

	// Get binder
    auto binder = ndk::SpAIBinder(AServiceManager_getService(serviceName));
    if (binder.get() == nullptr) {
		ALOGE("Failed to get service: %s", serviceName);
		return -1;
	}

	::std::shared_ptr<IExampleService> service = nullptr;
	service = IExampleService::fromBinder(binder);
	if (service != nullptr)
	{
		ALOGW("%s:%d: Connect IExampleService API successful", __FUNCTION__, __LINE__);

		ExampleType* example_type = new ExampleType();

		service->getValues(example_type);
		int64_t retval = example_type->retval;
		std::vector<int32_t> cert = example_type->cert;

		ALOGW ("%s:%d: Got retval: %d", __FUNCTION__, __LINE__,retval);
		for (unsigned int i = 0; i< cert.size(); i++)
		{
			ALOGW ("%s:%d: Got cert[%d]: %d", __FUNCTION__, __LINE__,i,cert[i]);
		}
	}
	else
	{
		ALOGW("%s:%d: Connect IExampleService API failed", __FUNCTION__, __LINE__);
		return -1;
	}

	return 0;
}