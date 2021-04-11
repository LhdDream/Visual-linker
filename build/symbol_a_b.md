| The rules                                                     |
| :----                                                         |
| Local symbols have been determined during compilation         |
| Only GLOBAL symbols are considered during the linking process |

| files gather |
| :----        |
| a.o          |
| b.o          |

 Define Symbol Table 
| File  | Value              | Size       | Type  | Bind   | Vis     | Ndx   | Name          |
| :---- | :----              | :----      | :---- | :----  | :----   | :---- | :----         |
| a.o   | 0x0000000000000000 | 0x0000002c | FUNC  | GLOBAL | DEFAULT | 1     | main(.symtab) |


 Undefine Symbol Table 
| File  | Value              | Size       | Type   | Bind   | Vis     | Ndx   | Name            |
| :---- | :----              | :----      | :----  | :----  | :----   | :---- | :----           |
| a.o   | 0x0000000000000000 | 0x00000000 | NOTYPE | GLOBAL | DEFAULT | UND   | shared(.symtab) |
| a.o   | 0x0000000000000000 | 0x00000000 | NOTYPE | GLOBAL | DEFAULT | UND   | swap(.symtab)   |


Global Symbol Table
| Value              | Size       | Type   | Bind   | Vis     | Ndx   | Name            |
| :----              | :----      | :----  | :----  | :----   | :---- | :----           |
| 0x0000000000000000 | 0x00000000 | NOTYPE | GLOBAL | DEFAULT | UND   | swap(.symtab)   |
| 0x0000000000000000 | 0x00000000 | NOTYPE | GLOBAL | DEFAULT | UND   | shared(.symtab) |
| 0x0000000000000000 | 0x0000002c | FUNC   | GLOBAL | DEFAULT | 1     | main(.symtab)   |


 Define Symbol Table 
| File  | Value              | Size       | Type   | Bind   | Vis     | Ndx   | Name            |
| :---- | :----              | :----      | :----  | :----  | :----   | :---- | :----           |
| b.o   | 0x0000000000000000 | 0x00000004 | OBJECT | GLOBAL | DEFAULT | 2     | shared(.symtab) |
| b.o   | 0x0000000000000000 | 0x0000004b | FUNC   | GLOBAL | DEFAULT | 1     | swap(.symtab)   |


 Undefine Symbol Table 
| File  | Value | Size  | Type  | Bind  | Vis   | Ndx   | Name  |
| :---- | :---- | :---- | :---- | :---- | :---- | :---- | :---- |


Global Symbol Table
| Value              | Size       | Type   | Bind   | Vis     | Ndx   | Name            |
| :----              | :----      | :----  | :----  | :----   | :---- | :----           |
| 0x0000000000000000 | 0x0000004b | FUNC   | GLOBAL | DEFAULT | 1     | swap(.symtab)   |
| 0x0000000000000000 | 0x00000004 | OBJECT | GLOBAL | DEFAULT | 2     | shared(.symtab) |
| 0x0000000000000000 | 0x0000002c | FUNC   | GLOBAL | DEFAULT | 1     | main(.symtab)   |


