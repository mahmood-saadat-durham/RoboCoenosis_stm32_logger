/^(\.text|\.data|\.rodata)/ {arduino_size += $2 }
/^(\.data|\.bss|\.noinit)/ {arduino_data += $2 }
/^(\.eeprom)/ {arduino_eeprom += $2 }
END { print "\nSketch uses " arduino_size " bytes (" int(arduino_size/1048576*100+0.5)  "%)  of program storage space. Maximum is " 1048576 " bytes.\nGlobal variables use "arduino_data" bytes ("int(arduino_data/98304*100+0.5)"%) of dynamic memory, leaving "98304-arduino_data" bytes for local variables. Maximum is "98304" bytes.\n"}