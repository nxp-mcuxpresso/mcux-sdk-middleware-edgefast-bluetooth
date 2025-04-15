# Hardware rework

1.  HCI UART rework
    -   Solder R93 and R96
2.  PCM interface rework
    -   Solder R70 and R79; remove R76 and R86; Connect J80.
3.  Wake pin rework
    -   When using 2LL M.2 module, remove R456 and R457 to avoid the module has an impact on boot configuration.

**Note:** Make sure to disconnect J80 when debugging. Otherwise, the debugger downloading fails.

**Parent topic:**[Hardware Rework Guide for MIMXRT1040-EVK and Murata M.2 Module](../topics/hardware_rework_guide_for_mimxrt1040-evkb_and_mura.md)

