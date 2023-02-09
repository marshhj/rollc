@echo off 

SET ECHO_START=+------------------ build start ------------------+
SET ECHO_END=+------------------ build end --------------------+

echo %ECHO_START%

SET MAIN=main.c
SET OUT=build\rollc.com
SET DBG=build\rollc.com.dbg

SET GCC=d:\dev-lib\cross9\bin\x86_64-pc-linux-gnu-gcc.exe
SET OBJCOPY=d:\dev-lib\cross9\bin\x86_64-pc-linux-gnu-objcopy.exe
SET LIB=3rd\cosmopolitan\



if exist %DBG% (
  rm %DBG%
)
if exist %OUT% (
  rm %OUT%
)

%GCC% -DNOINC -g -Os -static -nostdlib -nostdinc -fno-pie -no-pie -mno-red-zone -fno-omit-frame-pointer -pg -mnop-mcount -mno-tls-direct-seg-refs -gdwarf-4 -o %DBG% %MAIN% -fuse-ld=bfd -Wl,-T,%LIB%ape.lds -Wl,--gc-sections -include %LIB%cosmopolitan.h %LIB%crt.o %LIB%ape-no-modify-self.o %LIB%cosmopolitan.a

if exist %DBG% (
  %OBJCOPY% -S -O binary %DBG% %OUT%
) else (
  echo build fail!
)

if exist %OUT% ( 
  echo build ok: %OUT%
  echo %ECHO_END%
  if "%1" == "r" (
    echo +--------------- run %OUT% :
    echo=
    %OUT%
  )
) else (
  echo %ECHO_END%
)