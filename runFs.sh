#!/bin/bash


gem5_opt="./build/ECE666/gem5.opt"
gem5_result_prefix="project/results"
gem5_test_case="fs-atfix"
gem5_result_dir="${gem5_result_prefix}/m5out-${gem5_test_case}"
gem5_config="configs/example/fs.py"
gem5_nCores="4"

bm_names=(
    "blackscholes"
    # "ferret" - no libjpeg in fs linux
    "freqmine"
    "swaptions"
    "bodytrack"
    "fluidanimate"
    "raytrace"
    "canneal"
    # "dedup"
)

mkdir ${gem5_result_dir}
mkdir "${gem5_result_dir}/terminals"

for bm_name in "${bm_names[@]}"; do

    bm_result_dir="${gem5_result_dir}/${bm_name}"
    mkdir ${bm_result_dir}
    # gem5_args="--debug-start=206000497500 --debug-flags=ProtocolTrace,RubySlicc,RubyNetwork,RubyGenerated,Exec -d ${gem5_result_dir} ${gem5_config} -n ${gem5_nCores} --ruby"
    gem5_args="-d ${bm_result_dir} ${gem5_config} -n ${gem5_nCores} --ruby"

    gem5_kernel="../gem5-asgn/binaries/x86_64-vmlinux-2.6.22.9.smp"
    # gem5_kernel="./binaries/vmlinux-5.4.49"

    disk1="../gem5-asgn/disks/lab-linux-x86.img"
    # disk1="./disks/newlinux.img"
    disk2="./disks/parsec_disk.img"


    # m5script="./project/m5script.sh"
    m5script="./project/scripts/m5script-${bm_name}.sh"

    fs_cmd="${gem5_opt} ${gem5_args} --kernel=${gem5_kernel} --disk-image=${disk1} --disk-image=${disk2} --script=${m5script}"
    # fs_cmd="${gem5_opt} ${gem5_args} --kernel=${gem5_kernel} --disk-image=${disk1} --disk-image=${disk2}"

    echo ${fs_cmd}
    eval ${fs_cmd} > "${gem5_result_dir}/terminals/fs_${bm_name}.txt" &

done
