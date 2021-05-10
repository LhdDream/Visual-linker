段表信息

| File  | Name       | Type         | Address            | Size  |
| :---- | :----      | :----        | :----              | :---- |
| a.o   | .text      | SHT_PROGBITS | 0x0000000000000000 | 44    |
| a.o   | .rela.text | SHT_RELA     | 0x0000000000000000 | 48    |
| a.o   | .data      | SHT_PROGBITS | 0x0000000000000000 | 0     |
| a.o   | .bss       | SHT_NOBITS   | 0x0000000000000000 | 0     |

段表信息

| File  | Name  | Type         | Address            | Size  |
| :---- | :---- | :----        | :----              | :---- |
| b.o   | .text | SHT_PROGBITS | 0x0000000000000000 | 75    |
| b.o   | .data | SHT_PROGBITS | 0x0000000000000000 | 4     |
| b.o   | .bss  | SHT_NOBITS   | 0x0000000000000000 | 0     |

段表起始地址

| Section | Address            |
| :----   | :----              |
| .text   | 0x0000000000010000 |
| .data   | 0x0000000008000000 |
| .bss    | 0x0000000008000004 |

段表合并顺序

| File  | Section | Address            | Size  |
| :---- | :----   | :----              | :---- |
| a.o   | .text   | 0x0000000000010000 | 44    |
| b.o   | .text   | 0x000000000001002c | 75    |
| a.o   | .data   | 0x0000000008000000 | 0     |
| b.o   | .data   | 0x0000000008000000 | 4     |
| a.o   | .bss    | 0x0000000008000004 | 0     |
| b.o   | .bss    | 0x0000000008000004 | 0     |

段表信息

| File  | Name  | Type         | Address            | Size  |
| :---- | :---- | :----        | :----              | :---- |
| ab    | .text | SHT_PROGBITS | 0x0000000000010000 | 119   |
| ab    | .data | SHT_PROGBITS | 0x0000000008000000 | 4     |

