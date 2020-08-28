copy "2_Out\Tricore_Gnuc\Edison_Tc.hex" "4_HexFiles\Appl.hex"
rem copy "2_Out\Tricore_Gnuc\QI_Controller.hex" "4_HexFiles\Appl.hex"

cd "3_Tools\HexView"

set PRJ_NAME=Edison
set Hex_FILE="%~dp04_HexFiles\Appl.hex"
set S19_FILE="%~dp04_HexFiles\%PRJ_NAME%_Appl.s19"

Hexview /s %Hex_FILE% /CR:0x80000000-0x800FFFFF /XS:32:2 /AD:0x20 /AL /Af00 -o %S19_FILE%

del %Hex_FILE%

rem pause