@ECHO OFF
"C:\Program Files\Atmel\AVR Tools\AvrAssembler2\avrasm2.exe" -S "D:\Projects\XMOD\Modular\labels.tmp" -fI -W+ie -o "D:\Projects\XMOD\Modular\Modular.hex" -d "D:\Projects\XMOD\Modular\Modular.obj" -e "D:\Projects\XMOD\Modular\Modular.eep" -m "D:\Projects\XMOD\Modular\Modular.map" "D:\Projects\XMOD\Modular\Modular.asm"
