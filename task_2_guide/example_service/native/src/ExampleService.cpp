#include "ExampleService.hpp"
#include <utils/Log.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "example_service"

namespace example
{
   namespace service
   {
      ExampleService::ExampleService() {}
      ExampleService::~ExampleService() {}

      ::std::shared_ptr<ExampleService> ExampleService::S_INSTANCE = NULL;

      ::std::shared_ptr<ExampleService> ExampleService::getInstance()
      {
         if (S_INSTANCE == NULL)
         {
            S_INSTANCE = ndk::SharedRefBase::make<ExampleService>();
         }
         return S_INSTANCE;
      }

      ::ndk::ScopedAStatus ExampleService::getValues(esapinamespace::ExampleType *_aidl_return)
      {
         esapinamespace::ExampleType extype;

         std::vector<int32_t> cert(10);
         for (unsigned int i = 0; i < cert.size(); i++)
         {
            cert[i] = i;
         }

         extype.cert = cert;
         extype.retval = (int64_t)2;

         *_aidl_return = extype;
         return ndk::ScopedAStatus::ok();
      }
   }
}
