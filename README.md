# emu1802
The computer with only COSMAC CDP1802 and PIC18F47Q43/PIC18F27Q43  
developed by Kazuhiro Ouchi

The purpose of this experiment is to see if it is possible to make a real COSMAC work with fewer components, at a lower cost, and more compactly.

The EMU1802 is replacement the CPU of the EMUZ80 with a COSMAC.  
The EMU1802-mini is replacement the PIC of the EMU1802 with a PIC18F27Q43.  

## Repository Contents
- [emu1802.X](/emu1802.X/) - PIC18F47Q43 ROM RAM and UART emulation firmware for EMU1802
- [emu1802mini.X](/emu1802mini.X/) - PIC18F27Q43 ROM RAM and UART emulation firmware for EMU1802-mini
- [idiotmon](/idiotmon/) - IDIOT/4 monitor and sample programs
- [kicad/emu1802_sch.pdf](/kicad/emu1802_sch.pdf) - schematic of EMU1802
- [kicad/emu1802_mini_sch.pdf](/kicad/emu1802_mini_sch.pdf) - schematic of EMU1802-mini
- [kicad/emu1802_mini_bom.md](/kicad/emu1802_mini_bom.md) - BOM of EMU1802-mini
- [test_programs](/test_programs/) - Test programs for EMU1802 hardware
- [tools/bin2hexsrc.py](/tools/bin2hexsrc.py) - A tool to convert binaries to HEX
- [EMU1802mini_Setup_ja.md](/EMU1802mini_Setup_ja.md) - Setup manual for EMU1802mini (Japanese)
- [README.md](/README.md) - This manual

## Development tools
- [Microchip MPLAB IDE v6.00](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide)
- [Microchip MPLAB SNAP](https://www.microchip.com/en-us/development-tool/PG164100)
- [SB-Assembler 3](https://www.sbprojects.net/sbasm/)
- Python3

## Blog
You can also refer to my blog. (in Japanese)  
https://kanpapa.com/cosmac/blog/emu1802/

## Pictures

The EMU1802-mini  
![EMU1802-mini](/pictures/emu1802-mini.jpg)

Testing of the EMU1802-mini  
![Testing of EMU1802-mini](/pictures/emu1802-mini-uart-test0703.jpg)

The idiot monitor is working on the EMU1802-mini  
![The idiot monitor is working on emu1802-mini](/pictures/emu1802-mini-idiot-monitor.jpg)

Prototype of The EMU1802  
![Prototype of The EMU1802](/pictures/emu1802-series.jpg)


## License Information
References to any products, programs or services do not imply that they will be available in all countries in which their respective owner operates.  

Company, product, or services names may be trademarks or services marks of others.  

All libraries used in this code are copyright their respective authors.  

This project is based on the EMUZ80 hardware and software developed by Tetsuya Suzuki.

EMUZ80 - The computer with only Z80 and PIC18F47Q43  
  Written by Tetsuya Suzuki  
  https://github.com/vintagechips/emuz80  

The firmware includes the IDIOT monitor.  

IDIOT/4 MONITOR FOR THE 1802  
  WRITTEN: 6/23/82 BY LEE HART  
  COPYRIGHT 1982-85 BY TMSI  
  This program may be used for any noncommercial use free of charge.  
  fixes by Herb Johnson HRJ April 2010, Jan 2014  
  https://www.retrotechnology.com/memship/mship_idiot.html
