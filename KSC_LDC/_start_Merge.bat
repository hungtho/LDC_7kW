del "%~dp0HexFiles\Merged.s19"

cd "%~dp0Tools\HexView"

Hexview /s "%~dp0build\out.hex" /MT:"%~dp0HexFiles\FBL.hex;0x00:0x00FC0000-0x00FC7FFF" /XS:32:1 /AD:0x20 /AL -o "%~dp0HexFiles\Merged.s19"

rem pause