....
@JavaDerive(toString=true)
@Backing(type="int")
enum TestVendorProperty {
    .....
    /**
     * Static read-only property for testing.
     * Returns a constant integer value.
     *
     * VehiclePropertyGroup.VENDOR | VehicleArea.GLOBAL | VehiclePropertyType.INT32
     */
    STATIC_READ_ONLY_PROPERTY = 0x2B00 + 0x20000000 + 0x01000000 + 0x00400000,

    /**
     * Dynamic read/write property for testing.
     * Continuously changes and can be written to.
     *
     * VehiclePropertyGroup.VENDOR | VehicleArea.GLOBAL | VehiclePropertyType.INT32
     */
    DYNAMIC_READ_WRITE_PROPERTY = 0x2B01 + 0x20000000 + 0x01000000 + 0x00400000,
}