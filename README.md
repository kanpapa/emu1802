# emu1802
The computer with only COSMAC CDP1802 and PIC18F47Q43/PIC18F27Q43  
developed by Kazuhiro Ouchi

The EMU1802 is replacement the CPU of the EMUZ80 with a COSMAC.  
The EMU1802-mini is replacement the PIC of the EMU1802 with a PIC18F27Q43.  

## Repository Contents
- emu1802.X - PIC18F47Q43 ROM RAM and UART emulation firmware for EMU1802
- emu1802mini.X - PIC18F27Q43 ROM RAM and UART emulation firmware for EMU1802-mini
- idiotmon - IDIOT/4 monitor and sample programs
- kicad/emu1802_sch.pdf - schematic of EMU1802
- kicad/emu1802_mini_sch.pdf - schematic of EMU1802-mini
- kicad/emu1802_mini_bom.pdf - BOM of EMU1802-mini
- test_programs - Test programs for EMU1802 hardware
- tools/bin2hexsrc.py - A tool to convert binaries to HEX

## Development tools
- [Microchip MPLAB IDE v6.00](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide)
- [SB-Assemblers](https://www.sbprojects.net/sbasm/)

## Blog
You can also refer to my blog. (in Japanese)  
https://kanpapa.com/cosmac/blog/emu1802/

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
