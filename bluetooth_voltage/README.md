Combination of battery_voltage_and_timer and bluetooth_communicator. Stripped out run time, but may be added back in for additional functionality.

One main difference: 500 samples of the battery voltage and percent are taken and averaged, and that is sent via bluetooth to the bluetooth serial monitor. Done to eliminate inaccuracy since output of the information is on-demand and not continuous.
