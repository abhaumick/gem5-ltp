# ECE 666 - `gem5` Starter

All students docs present in `lab/` 

## Building gem5

### Build

```bash
scons-3.6 -j 16 build/ECE666/gem5.opt
```

### Clean

```bash
rm -rf build
```


## Task 1

1. Added back main routines to `hw1-mm.cpp` and `hw1-sort.cpp`
1. Built `hw1-mm` and `hw1-sort` binaries
1. Copied binaries into `lab/bin`

### Launching the tasks

```bash
./build/ECE666/gem5.opt configs/example/se.py -n 4 -c ./lab/bin/hw1-mm -o "64 4"
```

Results @ `/lab/results/t1-stats.txt`

<!-- 
<details>
  <summary>Click to expand!</summary>
  
  ```javascript
    function whatIsLove() {
      console.log('Baby Don't hurt me. Don't hurt me');
      return 'No more';
    }
  ```
</details>
-->


## Task 2

find `html` doc @ `/build/ECE666/mem/ruby/protocol/html/index.html`

find source @ `/src/mem/ruby/protocol` &rightarrow; `.slicc` and `.sm` files 


## Task 3 - Running the Ruby random tester

```bash
./build/ECE666/gem5.opt --debug-flags=ProtocolTrace ./configs/example/ruby_random_test.py -n 4 --maxloads 100 2>&1 | tee ./lab/results/t3_trace.log
```

Understanding the protocol trace : http://www.gem5.org/documentation/learning_gem5/part3/MSIdebugging/

## Task 4 - Cache simulation with real applications

```bash
./build/ECE666/gem5.opt configs/example/se.py -n 4 --ruby -c ./lab/bin/hw1-mm -o "64 4"
```


## Task 5

Download the [x86 pre-built binaries](http://www.m5sim.org/dist/current/x86/x86-system.tar.bz2) 

```bash
wget http://www.m5sim.org/dist/current/x86/x86-system.tar.bz2
``` 

Extract using 
```bash
tar -xvf x86-system.tar.bz2 
```

Inject required binary into disk image

- mount `.img` file in a linux system with su priviledges
- create directories and files using `sudo`
- unmount, deploy to gem5

```bash
sudo mount -o loop,offset=32256 ./linux-x86.img ./mount1
sudo mount -o loop,offset=536870912 ./linux-x86.img ./mount2
# Do Stuff Here - use SUDO
sudo umount ./mount
```
Copy to remote system using 
```bash
scp ./lab-linux-x86.img abhaumic@ecegrid.ecn.purdue.edu:/home/min/a/abhaumic/AdvCompArch
```

Execute a full system simulation using

```bash
./build/ECE666/gem5.opt configs/example/fs.py -n 4 --ruby --kernel=./binaries/x86_64-vmlinux-2.6.22.9.smp --disk-image=./disks/lab-linux-x86.img --script=./lab/m5script.sh 
```
Contents of `m5script.sh`

```bash
#!/bin/bash

./home/lab/pthreads-pi-64-4

m5 exit

```
