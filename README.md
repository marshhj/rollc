# rollc  
Rollup .h .c .cpp files to one file. Depend [cosmopolitan](https://justine.lol/cosmopolitan/download.html). Can run cross platform.  
Like [nuklear](https://github.com/vurtun/nuklear) build script. But that use python.

# build  
Just use [cosmopolitan](https://justine.lol/cosmopolitan/download.html) method to build ```main.c```.  

## Windows
Here provide a ```build.bat``` for windows to build.You can edit the build file and change the gcc path or other.  
On windows you would download the [cross9.zip](https://justine.lol/linux-compiler-on-windows/cross9.zip) (x86_64-linux-gnu static binaries for windows (355mb)) to build it.

## Linux
Just build with gcc.

# use
If you don't want to build yourself. Just run the ```build/rollc.com``` file.  

```
rollc 
  -r join to roll files.
  -o output file.
  -c config file, default rollc.txt
  -h header files, copy first.
```
