# Search Log Files

### Author: cobb208, Cory C.

The program is created to search "log" files for keywords.

Use the make file to generate the executable:

`%: make main && ./main [put your keywords here]`

___

Flags:
- -o Will search for "or"

When the program is ran it will search for all terms given, if no flag o flag is given, it will return any files that contain all keywords.

The directory is not dynamic it is static and set to ./log/\[files\]

To test the program inside the log file you will find files of different types, they were created to ensure the program only filtered out *".log"* files. 

___

### Credit for dbg.h is: Zed A. Shaw from his book *Learn C the Hard Way*