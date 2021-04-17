| a.o                             |
| :----                           |
| This relocation result is empty |

| b.o                            |
| :----                          |
| swap (addr 0x0000000000000000) |
| (0x0000000000000020)->         |

| Relocation Name | Relocation Value                                    |
| :----           | :----                                               |
| swap            | 10020:	e8 07 00 00 00       	callq  1002c <swap>    |
| swap            | 10025:	b8 00 00 00 00       	mov    $0x0,%eax       |

| Load files |
| :----      |
| a.o        |
| b.o        |


| Section | Address            | Size  |
| :----   | :----              | :---- |
| .text   | 0x0000000000010000 | 0x77  |
| .data   | 0x0000000008000000 | 0x4   |
| .bss    | 0x0000000008000004 | 0x0   |

