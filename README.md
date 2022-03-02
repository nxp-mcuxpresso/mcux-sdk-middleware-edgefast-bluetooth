# NXP Middleware EdgeFast Bluetooth Protocol Abstraction Layer
This repository is for EdgeFast Bluetooth Protocol Abstraction Layer delivery, which is a wrapper layer on top of the bluetooth host stack and based on Zephyr Bluetooth host stack API.
This reposirtory is a part of MCUXpresso SDK overall delivery which is composed of several project deliveries. Please go to the [mcux-sdk](https://github.com/NXPmicro/mcux-sdk/) to get the MCUXpresso overall delivery to be able to build and run edgefast examples that are based on this reposirtory.

## License
This repository is under BSD-3-Clause license, license copy please check [COPYING_BSD-3](COPYING-BSD-3).

## EdgeFast Bluetooth Examples
EdgeFast bluetooth examples are located in examples/${boards}/edgefast_bluetooth_examples. 
Different from SDK 2.11.0 package release, only the following examples are provided and macro CONFIG_BT_SMP and CONFIG_BT_SETTINGS is disabled by default.
- wireless_uart
- central_ht
- peripheral_ht
- central_pxm
- peripheral_pxr
- central_hpc
- peripheral_hps
- central_ipsp
- peripheral_ipsp

## Supported NXP Boards
EdgeFast bluetooth examples are supported in the following NXP boards now:
- EVK-MIMXRT1060
- MIMXRT1060-EVKB
- MIMXRT1170-EVK
- EVK-MIMXRT595
- EVK-MIMXRT685

## Contribution
Contributions are not accepted now, and will be opened in the future. 