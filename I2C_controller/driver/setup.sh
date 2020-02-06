insmod i2c_adapter.ko
modprobe i2c-dev

touch test.txt
echo "hello stm32" > test.txt
