...
class FakeVehicleHardware : public IVehicleHardware {
  private:
        mutable std::mutex mVendorPropsLock; 
        mutable std::unordered_map<int32_t, int32_t> mVendorPropsValues;
  ...
}      
