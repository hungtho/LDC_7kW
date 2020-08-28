copy "2_Out\Tricore_Gnuc\Edison_Tc.hex" "4_HexFiles\Appl.hex"
rem copy "2_Out\Tricore_Gnuc\QI_Controller.hex" "4_HexFiles\Appl.hex"

cd "3_Tools\HexView"

set PRJ_NAME=Edison
set APPL_FILE="%~dp04_HexFiles\Appl.hex"
set FBL_FILE=%~dp04_HexFiles\AURIX_FBL_Tc.hex
set MERGE_FILE="%~dp04_HexFiles\%PRJ_NAME%_Merged.s19"

Hexview /s %APPL_FILE% /CR:0x80000000-0x800FFFFF /MT:"%FBL_FILE%;0x00:0x80000000-0x80007FFF" /XS:32:2 /AD:0x20 /AL /Af00 -o %MERGE_FILE%

del %APPL_FILE%

rem pause