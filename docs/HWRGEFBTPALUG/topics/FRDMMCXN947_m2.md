# Hardware Rework Guide for FRDM-MCXN947 and X-FRDM-WIFI-M.2 Adapter

This section is a brief hardware rework guidance of the EdgeFast Bluetooth PAL on the NXP FRDM-MCXN947 board and X-FRDM-WIFI-M.2 or the Murata’s 2LL EAR00500 \(2LL\) M.2 modules solution.

The hardware rework consists of one part:

-   UART interface rework

## Hardware rework 

-   UART interface rework
    -   Remove SJ11 1-2, connect SJ11 2-3
    -   Remove SJ10 1-2, connect J1-3 to J9-26

-   X-FRDM-WIFI-M.2 jumper setting
    -   Connect J8(On X-FRDM-WIFI-M.2) for 1.8V
    -   Connect J24(On X-FRDM-WIFI-M.2) for 3.3V
    -   Connect J19(On X-FRDM-WIFI-M.2) for 1.8V
    -   Connect J25(On X-FRDM-WIFI-M.2) for 3.3V
    -   Connect J15(On X-FRDM-WIFI-M.2) for 1.8V
    -   Connect J16(On X-FRDM-WIFI-M.2) for 3.3V
    -   Connect J17(On X-FRDM-WIFI-M.2) for 1.8V
    -   Connect J18(On X-FRDM-WIFI-M.2) for 3.3V
    ![](../images/FRDM-MCXN947-BOT.png "FRDM-MCXN947")


