from smbus import *

bus=SMBus(2)

bus.read_word_data(0x02,0x02)


bus.write_word_data(0x02,0x03,0x04)

