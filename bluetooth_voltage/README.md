Combination of battery_voltage_and_timer and bluetooth_communicator.

One main difference: 100 samples of the battery voltage and battery percent each are taken and averaged, and that is sent via bluetooth to the bluetooth serial monitor. Done to eliminate inaccuracy since output of the information is on-demand and not continuous.

Keeping track of 1000 samples, could be used for battery life prediction.
