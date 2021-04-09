| files gather |
| :----        |
| a.o          |
| b.o          |
| ab           |

a.o
 Define Symbol Table 
| Value              | Size       | Type    | Bind   | Vis     | Ndx   | Name          |
| :----              | :----      | :----   | :----  | :----   | :---- | :----         |
| 0x0000000000000000 | 0x00000000 | FILE    | LOCAL  | DEFAULT | ABS   | a.c(.symtab)  |
| 0x0000000000000000 | 0x00000000 | SECTION | LOCAL  | DEFAULT | 1     | (.symtab)     |
| 0x0000000000000000 | 0x00000000 | SECTION | LOCAL  | DEFAULT | 3     | (.symtab)     |
| 0x0000000000000000 | 0x00000000 | SECTION | LOCAL  | DEFAULT | 4     | (.symtab)     |
| 0x0000000000000000 | 0x00000000 | SECTION | LOCAL  | DEFAULT | 6     | (.symtab)     |
| 0x0000000000000000 | 0x00000000 | SECTION | LOCAL  | DEFAULT | 7     | (.symtab)     |
| 0x0000000000000000 | 0x00000000 | SECTION | LOCAL  | DEFAULT | 5     | (.symtab)     |
| 0x0000000000000000 | 0x0000002c | FUNC    | GLOBAL | DEFAULT | 1     | main(.symtab) |


a.o
 Undefine Symbol Table 
| Value              | Size       | Type   | Bind   | Vis     | Ndx   | Name            |
| :----              | :----      | :----  | :----  | :----   | :---- | :----           |
| 0x0000000000000000 | 0x00000000 | NOTYPE | GLOBAL | DEFAULT | UND   | shared(.symtab) |
| 0x0000000000000000 | 0x00000000 | NOTYPE | GLOBAL | DEFAULT | UND   | swap(.symtab)   |


b.o
 Define Symbol Table 
| Value              | Size       | Type    | Bind   | Vis     | Ndx   | Name            |
| :----              | :----      | :----   | :----  | :----   | :---- | :----           |
| 0x0000000000000000 | 0x00000000 | FILE    | LOCAL  | DEFAULT | ABS   | b.c(.symtab)    |
| 0x0000000000000000 | 0x00000000 | SECTION | LOCAL  | DEFAULT | 1     | (.symtab)       |
| 0x0000000000000000 | 0x00000000 | SECTION | LOCAL  | DEFAULT | 2     | (.symtab)       |
| 0x0000000000000000 | 0x00000000 | SECTION | LOCAL  | DEFAULT | 3     | (.symtab)       |
| 0x0000000000000000 | 0x00000000 | SECTION | LOCAL  | DEFAULT | 5     | (.symtab)       |
| 0x0000000000000000 | 0x00000000 | SECTION | LOCAL  | DEFAULT | 6     | (.symtab)       |
| 0x0000000000000000 | 0x00000000 | SECTION | LOCAL  | DEFAULT | 4     | (.symtab)       |
| 0x0000000000000000 | 0x00000004 | OBJECT  | GLOBAL | DEFAULT | 2     | shared(.symtab) |
| 0x0000000000000000 | 0x0000004b | FUNC    | GLOBAL | DEFAULT | 1     | swap(.symtab)   |


b.o
 Undefine Symbol Table 
| Value | Size  | Type  | Bind  | Vis   | Ndx   | Name  |
| :---- | :---- | :---- | :---- | :---- | :---- | :---- |


ab
 Define Symbol Table 
| Value              | Size       | Type    | Bind   | Vis     | Ndx   | Name                           |
| :----              | :----      | :----   | :----  | :----   | :---- | :----                          |
| 0x000000000010,000 | 0x00000000 | SECTION | LOCAL  | DEFAULT | 1     | (.symtab)                      |
| 0x000000000010,078 | 0x00000000 | SECTION | LOCAL  | DEFAULT | 2     | (.symtab)                      |
| 0x00000008,000,000 | 0x00000000 | SECTION | LOCAL  | DEFAULT | 3     | (.symtab)                      |
| 0x0000000000000000 | 0x00000000 | SECTION | LOCAL  | DEFAULT | 4     | (.symtab)                      |
| 0x0000000000000000 | 0x00000000 | FILE    | LOCAL  | DEFAULT | ABS   | a.c(.symtab)                   |
| 0x0000000000000000 | 0x00000000 | FILE    | LOCAL  | DEFAULT | ABS   | b.c(.symtab)                   |
| 0x0000000000000000 | 0x00000000 | FILE    | LOCAL  | DEFAULT | ABS   | (.symtab)                      |
| 0x00000008,000,008 | 0x00000000 | OBJECT  | LOCAL  | DEFAULT | 3     | _GLOBAL_OFFSET_TABLE_(.symtab) |
| 0x000000000010,02c | 0x0000004b | FUNC    | GLOBAL | DEFAULT | 1     | swap(.symtab)                  |
| 0x00000008,000,000 | 0x00000004 | OBJECT  | GLOBAL | DEFAULT | 3     | shared(.symtab)                |
| 0x000000000010,000 | 0x0000002c | FUNC    | GLOBAL | DEFAULT | 1     | main(.symtab)                  |


ab
 Undefine Symbol Table 
| Value | Size  | Type  | Bind  | Vis   | Ndx   | Name  |
| :---- | :---- | :---- | :---- | :---- | :---- | :---- |


