# emu1802
The computer with only COSMAC CDP1802 and PIC18F47Q43/PIC18F27Q43  
developed by Kazuhiro Ouchi

The EMU1802 is replacement the CPU of the EMUZ80 with a COSMAC.  
The EMU1802-mini is replacement the PIC of the EMU1802 with a PIC18F27Q43.  

## Repository Contents

- emu1802.X - PIC18F47Q43 ROM RAM and UART emulation firmware for EMU1802  
- emu1802mini.X - PIC18F27Q43 ROM RAM and UART emulation firmware for EMU1802-mini  
- kicad/emu1802_sch.pdf - schematic of EMU1802   
- kicad/emu1802_mini_sch.pdf - schematic of EMU1802-mini   
- kicad/emu1802_mini_bom.pdf - BOM of EMU1802-mini   
- tools/bin2hexsrc.py - A tool to convert binaries to HEX

## Development tools

- [Microchip MPLAB IDE v6.00](https://www.microchip.com/en-us/tools-resources/develop/mplab-x-ide)
- [SB-Assemblers](https://www.sbprojects.net/sbasm/)

## Blog
You can also refer to my blog. (in Japanese)  
https://kanpapa.com/cosmac/blog/emu1802/

## License Information

This project is based on the EMUZ80 hardware and software developed by Tetsuya Suzuki.

EMUZ80 - The computer with only Z80 and PIC18F47Q43  
  Written by Tetsuya Suzuki  
  https://github.com/vintagechips/emuz80  
