
# Merge Binary Tool
A simple tool to merge multiple binary files at specified offsets.

## on windows system
Simply run the merge tool from Command Prompt:
- merge_bin.exe output_file.bin input1.bin 0xOFFSET1 input2.bin 0xOFFSET2 ...


## on linux / mac
1. Grant execute permissions to the binary
- chmod +x merge_bin


2. Run the merge tool:
- ./merge_bin output_file.bin input1.bin 0xOFFSET1 input2.bin 0xOFFSET2 ...

## build from source
gcc merge_bin.c -o merge_bin

