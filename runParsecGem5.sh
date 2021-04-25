#!/bin/bash
 
gem5_nCores="4"
 
# gem5 Settings
gem5_dir="${PWD}"
gem5_opt="build/ECE666/gem5.opt"
gem5_config="configs/example/se.py"
 
gem5_args="${gem5_dir}/${gem5_config} -n ${gem5_nCores} --ruby"
gem5_exec="build/ECE666/gem5.opt ${gem5_args}"
 
# TODO add args for ruby system and
 
# PARSEC Settings
bm_dataset="simsmall"
 
bm_names=(
    "blackscholes"
    "bodytrack"
    # "canneal" --  kernel
    #"dedup"    --  kernel
    "ferret"
    "fluidanimate"
    "freqmine"
    "raytrace"
    "swaptions"
)
 
# bm_name="blackscholes"
 
for bm_name in "${bm_names[@]}"; do
 
    echo ${bm_name}
 
    bm_dir="${gem5_dir}/benchmarks/parsec-2.1"
    bm_subdir="pkgs/apps/${bm_name}"
    bm_build="inst/amd64-linux.gcc.pre" #/bin/${bm_name}"
    bm_NTHREADS="4"
 
    # Find .runconf
    NTHREADS="${bm_NTHREADS}"
    bm_runconf="${bm_dir}/${bm_subdir}/parsec/${bm_dataset}.runconf"
 
    source ${bm_runconf}
 
    bm_run_exec="${bm_dir}/${bm_subdir}/${bm_build}/${run_exec}"
    run_path="${bm_dir}/pkgs/apps/${bm_name}/run"
 
    gem5_out_dir="${gem5_dir}/m5out/${bm_name}"
    rm -rf ${gem5_out_dir}
    mkdir ${gem5_out_dir} -v
 
    # EXEC_CMD="${gem5_exec} -n 4 -c  ${bm_run_exec} -o \"${bm_NTHREADS} ${run_path}/in_4K.txt ${run_path}/prices.txt\""
    # echo ${EXEC_CMD}
    # eval ${EXEC_CMD}
 
    bm_hw_cmd="${gem5_dir}/benchmarks/parsec-2.1/bin/parsecmgmt -a run -p ${bm_name} -i ${bm_dataset} -x pre"
    eval ${bm_hw_cmd}
 
    cd ${run_path}
 
    bm_args="${run_args}"
    gem5_cmd="${gem5_dir}/${gem5_opt} -d ${gem5_out_dir} ${gem5_args}"
    exec_cmd="${gem5_cmd} -c ${bm_run_exec} -o \"${run_args}\""
    echo ${exec_cmd}
    eval ${exec_cmd} &
 
    debug_cmd="gdb --args ${gem5_dir}/${gem5_opt} -d ${gem5_out_dir} ${gem5_args} -c ${bm_run_exec} -o \"${run_args}\""
    # echo ${debug_cmd}
    # eval ${debug_cmd}
 
done
 
echo "Copying results to m5out"
# rm -rf  ${gem5_dir}/m5out/${bm_name} -v
# mkdir  ${gem5_dir}/m5out -v
# mkdir  ${gem5_dir}/m5out/${bm_name} -v
# mv ${run_path}/m5out ${gem5_dir}/m5out/${bm_name}/ -f -v
# mv ${run_path}/*.log ${gem5_dir}/m5out/${bm_name}/ -f -v
echo " .. Done"
echo ""