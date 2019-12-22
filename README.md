# ezip
compression for text files

Description:
-c - Allows for compression
-d - Allows for decompression

for compression run program as is: 
	./ezip -c file1 file2

file1 - original file to be compressed
file2 - name of the compressed file (file cannot already exist)


for decompression run program as is:
	./ezip -d file1 file2

file1 - compressed file
file2 - name of the decompressed file (file cannot already exist)

Building: 
To build ezip, simply run make

NOTE: THIS PROGRAM CAN ONLY COMPRESS FILES THAT ARE SMALLER THAN 4GB, AN UPDATED VERSION IS EXPECTED IN THE FUTURE TO HANDLE LARGER FILES


