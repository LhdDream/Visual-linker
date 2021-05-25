加载的文件

| Load files                               | Size     | Section Size              |
| :----                                    | :----    | :----                     |
| a.o                                      | 44       | (text: 44 data: 0 bss: 0) |
| b.o                                      | 79       | (text: 75 data: 4 bss: 0) |

段起始地址

| Section | Address            | Size  |
| :----   | :----              | :---- |
| .text   | 0x0000000000010000 | 0x77  |
| .data   | 0x0000000008000000 | 0x4   |
| .bss    | 0x0000000008000004 | 0x0   |

重定位表

| b.o                            |
| :----                          |
| swap (addr 0x0000000000000000) |
| (0x0000000000000020)->         |

重定位表的汇编结果

| Relocation Name | Relocation Value                                    |
| :----           | :----                                               |
| swap            | 10020:	e8 07 00 00 00       	callq  1002c <swap>    |
| swap            | 10025:	b8 00 00 00 00       	mov    $0x0,%eax       |

重定位的表项结果

| File  | Value              | Size       | Type  | Bind   | Vis     | Ndx   | Name          |
| :---- | :----              | :----      | :---- | :----  | :----   | :---- | :----         |
| ab    | 0x000000000001002c | 0x0000004b | FUNC  | GLOBAL | DEFAULT | 1     | swap(.symtab) |

