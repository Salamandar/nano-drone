openocd -f interface/stlink-v2-1.cfg -f target/stm32f0x.cfg -c "telnet_port 4444" -c "gdb_port 3333" -c "init" -c "reset init" >/dev/null &

telnet localhost 4444 >/dev/null &

ddd --debugger arm-none-eabi-gdb --eval-command="target remote localhost:3333" NanoRobot
