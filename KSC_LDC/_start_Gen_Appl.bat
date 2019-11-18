del "%~dp0HexFiles\Appl.s19"

cd "%~dp0Tools\HexView"

Hexview /s "%~dp0build\out.hex" /XS:32:1 /AD:0x20 /AL -o "%~dp0HexFiles\Appl.s19"

rem pause