# KernelDrivers

## About
Drivers development for one of the subjects in Warsaw Univerity of Technology in Faculty of Electronics.

## ADC simulator
ADC simulator. Signal definition is sinus as an array of 1000
samples and max value 4095. ADC according to the sampling_rate gives every sample if 
sampling_rate is 1000, every second sample if 500, every 3rd sample
if sampling_rate if 333 and so on. Data from driver can be fetched 
every two second, if user process try faster, then it will be block and
notfied when sample will be available. Every two seconds to emulate 
that communication with a device is not immediate and device has an
internal queue where samples are stored. 

User can set a sampling_rate 
by communication with driver via opening virtual file in /dev/my_dev0 
and calling dedicated read and write functions or direct communication
through echo command like echo 500 > /dev/my_dev0.

### How to start
Build system in buildroot by calling 
`bash prepare.sh`

Compile driver
`bash build.sh`

Run emulated environment:
`bash start_qemu.sh`

Enter /demo directory in emulated environment and register device:
`ismod my_module.ko`

Run python test:
`python3 test_adc.py`

## Questions or need help?
Don't hesitate to send me a mail on jakub.wieczorek0101@gmail.com.

## Copyright and license
KernelDrivers project is copyright to Jakub Wieczorek under the [MIT License](https://opensource.org/licenses/MIT).
