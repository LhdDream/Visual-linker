| Section | Address            | Size  |
| :----   | :----              | :---- |
| .text   | 0x0000000000010000 | 0x77  |
| .data   | 0x0000000008000000 | 0x4   |
| .bss    | 0x0000000008000004 | 0x0   |

| Load files                               | Size     | Section Size              |
| :----                                    | :----    | :----                     |
| a.o                                      | 44       | (text: 44 data: 0 bss: 0) |
| b.o                                      | 79       | (text: 75 data: 4 bss: 0) |

| b.o                            |
| :----                          |
| swap (addr 0x0000000000000000) |
| (0x0000000000000020)->         |

| Relocation Name | Relocation Value                                    |
| :----           | :----                                               |
| swap            | 10020:	e8 07 00 00 00       	callq  1002c <swap>    |
| swap            | 10025:	b8 00 00 00 00       	mov    $0x0,%eax       |

