
#ifndef EXAMPLE_SERVICE_H
#define EXAMPLE_SERVICE_H

#include <aidl/example/service/api/BnExampleService.h>

namespace esapinamespace = ::aidl::example::service::api;

namespace example
{
   namespace service
   {
      class ExampleService : public esapinamespace::BnExampleService
      {
      public:
         ExampleService();
         ~ExampleService();

         static ::std::shared_ptr<ExampleService> getInstance();

         // IExampleService AIDL interface callbacks
         ::ndk::ScopedAStatus getValues(esapinamespace::ExampleType *_aidl_return) final;

         const std::string getServiceName(void)
         {
            return std::string() + descriptor + "/default";
         }

      private:
         static ::std::shared_ptr<ExampleService> S_INSTANCE;
      };
   }
}

#endif // EXAMPLE_SERVICE_H
