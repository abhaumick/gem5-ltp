# PARSEC 2.1

https://parsec.cs.princeton.edu/download.htm

## Pre-Built Binaries

```bash
wget http://parsec.cs.princeton.edu/download/2.1/parsec-2.1-core.tar.gz
wget http://parsec.cs.princeton.edu/download/2.1/binaries/parsec-2.1-amd64-linux.tar.gz
wget http://parsec.cs.princeton.edu/download/2.1/parsec-2.1-sim.tar.gz

tar -zxvf parsec-2.1-core.tar.gz
tar -zxvf parsec-2.1-amd64-linux.tar.gz
tar -zxvf parsec-2.1-sim.tar.gz

rm -rf parsec-2.1-core.tar.gz
rm -rf parsec-2.1-amd64-linux.tar.gz
rm -rf parsec-2.1-sim.tar.gz

./parsec-2.1/bin/setup.amd64-linux 

./parsec-2.1//bin/parsecmgmt -a run -i simsmall -x pre
```

## Alternate source

https://www.cs.utexas.edu/~parsec_m5/

check the provided linux.smp image and parsec2.1 image 
> (might be prebuilt or not) 

## Build

```bash
cd benchmarks
wget http://parsec.cs.princeton.edu/download/2.1/parsec-2.1.tar.gz
wget http://parsec.cs.princeton.edu/download/2.1/binaries/parsec-2.1-amd64-linux.tar.gz
tar -xzvf parsec-2.1.tar.gz
```

- Modify `./parsec-2.1/config/gcc.bldconf`
  - update paths to compiler

```bash
  export CC_HOME="/package/gcc/8.3.0"
  export BINUTIL_HOME="/usr"
```

build using 

```bash
# saguaro
./bin/parsecmgmt -a build -p blackscholes ferret fluidanimate freqmine swaptions vips x264
#ecegrid
```
### Build Issues

```bash
# bodytrack
looser throw specifier for 'virtual threads::WorkerGroup::~WorkerGroup() throw (threads::CondException, threads::MutexException)'
     ~WorkerGroup();
     ^

# facesim
Collisions_And_Interactions/../Rigid_Bodies/../Data_Structures/../Read_Write/FILE_UTILITIES.h:63:40: error: no match for ‘operator!=’ (operand types are ‘std::ofstream’ {aka ‘std::basic_ofstream<char>’} and ‘int’)
 {return std::ofstream(filename.c_str())!=0;}
              ~~~~~~~~~~~~~~~~~~~~~~~~~~^~~

# raytrace
checking for GLUT... configure: error: Package requirements (x11 xmu xi) were not met:

No package 'xmu' found

Consider adjusting the PKG_CONFIG_PATH environment variable if you
installed software in a non-standard prefix.

Alternatively, you may set the environment variables GLUT_CFLAGS
and GLUT_LIBS to avoid the need to call pkg-config.
See the pkg-config man page for more details.

[PARSEC] Error: 'env ./configure --with-driver=xlib --enable-glut --enable-static --disable-shared --prefix=/home/min/a/abhaumic/AdvCompArch/gem5-ltp/benchmarks/parsec-2.1/./pkgs/libs/mesa/inst/amd64-linux.gcc' failed.
```