  -h [ --help ]                   help message

  -i [ --include-dirs ] arg (=.)  Including to search directories, can be multiple. Default is current folder
  
  -e [ --exclude-dirs ] arg       Excluded to search directories, can be multiple
  
  -d [ --depth ] arg (=0)         Scan depth, 1 - all directories, 0 - current folder only. Default value is 0
  
  -m [ --min-size ] arg (=1)      Minimum size of the processed file in bytes. Default value is 1
  
  -f [ --file-masks ] arg         Masks for the files participating in comparison
  
  -b [ --block-size ] arg (=256)  The size in bytes of the block to read the files with. Default value is 256
  
  -a [ --algorithm ] arg (=CRC32) Hashing algorithm to hash file blocks. Default value is CRC32

