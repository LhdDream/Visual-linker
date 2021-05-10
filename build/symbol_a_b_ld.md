载入文件列表

| files gather |
| :----        |
| a.o          |
| b.o          |

已定义符号列表

| File  | Value              | Size       | Type  | Bind   | Vis     | Ndx   | Name          |
| :---- | :----              | :----      | :---- | :----  | :----   | :---- | :----         |
| a.o   | 0x0000000000000000 | 0x0000002c | FUNC  | GLOBAL | DEFAULT | 1     | main(.symtab) |


未定义符号列表

| File  | Value              | Size       | Type   | Bind   | Vis     | Ndx   | Name            |
| :---- | :----              | :----      | :----  | :----  | :----   | :---- | :----           |
| a.o   | 0x0000000000000000 | 0x00000000 | NOTYPE | GLOBAL | DEFAULT | UND   | shared(.symtab) |
| a.o   | 0x0000000000000000 | 0x00000000 | NOTYPE | GLOBAL | DEFAULT | UND   | swap(.symtab)   |


全局符号列表

| Value              | Size       | Type   | Bind   | Vis     | Ndx   | Name            |
| :----              | :----      | :----  | :----  | :----   | :---- | :----           |
| 0x0000000000000000 | 0x00000000 | NOTYPE | GLOBAL | DEFAULT | UND   | swap(.symtab)   |
| 0x0000000000000000 | 0x00000000 | NOTYPE | GLOBAL | DEFAULT | UND   | shared(.symtab) |
| 0x0000000000000000 | 0x0000002c | FUNC   | GLOBAL | DEFAULT | 1     | main(.symtab)   |


已定义符号列表

| File  | Value              | Size       | Type   | Bind   | Vis     | Ndx   | Name            |
| :---- | :----              | :----      | :----  | :----  | :----   | :---- | :----           |
| b.o   | 0x0000000000000000 | 0x00000004 | OBJECT | GLOBAL | DEFAULT | 2     | shared(.symtab) |
| b.o   | 0x0000000000000000 | 0x0000004b | FUNC   | GLOBAL | DEFAULT | 1     | swap(.symtab)   |


全局符号列表

| Value              | Size       | Type   | Bind   | Vis     | Ndx   | Name            |
| :----              | :----      | :----  | :----  | :----   | :---- | :----           |
| 0x0000000000000000 | 0x0000004b | FUNC   | GLOBAL | DEFAULT | 1     | swap(.symtab)   |
| 0x0000000000000000 | 0x00000004 | OBJECT | GLOBAL | DEFAULT | 2     | shared(.symtab) |
| 0x0000000000000000 | 0x0000002c | FUNC   | GLOBAL | DEFAULT | 1     | main(.symtab)   |


目标文件

| File  | Value              | Size       | Type   | Bind   | Vis     | Ndx   | Name            |
| :---- | :----              | :----      | :----  | :----  | :----   | :---- | :----           |
| ab    | 0x000000000010,02c | 0x0000004b | FUNC   | GLOBAL | DEFAULT | 1     | swap(.symtab)   |
| ab    | 0x00000008,000,000 | 0x00000004 | OBJECT | GLOBAL | DEFAULT | 3     | shared(.symtab) |
| ab    | 0x000000000010,000 | 0x0000002c | FUNC   | GLOBAL | DEFAULT | 1     | main(.symtab)   |


