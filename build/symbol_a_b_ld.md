| files gather |
| :----        |
| a.o          |
| b.o          |

| File  | Value              | Size       | Type  | Bind   | Vis     | Ndx   | Name          |
| :---- | :----              | :----      | :---- | :----  | :----   | :---- | :----         |
| a.o   | 0x0000000000000000 | 0x0000002c | FUNC  | GLOBAL | DEFAULT | 1     | main(.symtab) |


| File  | Value              | Size       | Type   | Bind   | Vis     | Ndx   | Name            |
| :---- | :----              | :----      | :----  | :----  | :----   | :---- | :----           |
| a.o   | 0x0000000000000000 | 0x00000000 | NOTYPE | GLOBAL | DEFAULT | UND   | shared(.symtab) |
| a.o   | 0x0000000000000000 | 0x00000000 | NOTYPE | GLOBAL | DEFAULT | UND   | swap(.symtab)   |


| Value              | Size       | Type   | Bind   | Vis     | Ndx   | Name            |
| :----              | :----      | :----  | :----  | :----   | :---- | :----           |
| 0x0000000000000000 | 0x00000000 | NOTYPE | GLOBAL | DEFAULT | UND   | swap(.symtab)   |
| 0x0000000000000000 | 0x00000000 | NOTYPE | GLOBAL | DEFAULT | UND   | shared(.symtab) |
| 0x0000000000000000 | 0x0000002c | FUNC   | GLOBAL | DEFAULT | 1     | main(.symtab)   |


| File  | Value              | Size       | Type   | Bind   | Vis     | Ndx   | Name            |
| :---- | :----              | :----      | :----  | :----  | :----   | :---- | :----           |
| b.o   | 0x0000000000000000 | 0x00000004 | OBJECT | GLOBAL | DEFAULT | 2     | shared(.symtab) |
| b.o   | 0x0000000000000000 | 0x0000004b | FUNC   | GLOBAL | DEFAULT | 1     | swap(.symtab)   |


| Value              | Size       | Type   | Bind   | Vis     | Ndx   | Name            |
| :----              | :----      | :----  | :----  | :----   | :---- | :----           |
| 0x0000000000000000 | 0x0000004b | FUNC   | GLOBAL | DEFAULT | 1     | swap(.symtab)   |
| 0x0000000000000000 | 0x00000004 | OBJECT | GLOBAL | DEFAULT | 2     | shared(.symtab) |
| 0x0000000000000000 | 0x0000002c | FUNC   | GLOBAL | DEFAULT | 1     | main(.symtab)   |


| Section | Address            | Size  |
| :----   | :----              | :---- |
| .text   | 0x0000000000010000 | 0x77  |
| .data   | 0x0000000008000000 | 0x4   |
| .bss    | 0x0000000008000004 | 0x0   |


| File  | Value              | Size       | Type   | Bind   | Vis     | Ndx   | Name            |
| :---- | :----              | :----      | :----  | :----  | :----   | :---- | :----           |
| ab    | 0x000000000010,02c | 0x0000004b | FUNC   | GLOBAL | DEFAULT | 1     | swap(.symtab)   |
| ab    | 0x00000008,000,000 | 0x00000004 | OBJECT | GLOBAL | DEFAULT | 3     | shared(.symtab) |
| ab    | 0x000000000010,000 | 0x0000002c | FUNC   | GLOBAL | DEFAULT | 1     | main(.symtab)   |


