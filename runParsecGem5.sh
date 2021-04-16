#!/bin/bash

gem5_nCores="4"

# gem5 Settings
gem5_dir="${PWD}"
gem5_opt="build/ECE666/gem5.opt"
gem5_config="configs/example/se.py"

gem5_exec="build/ECE666/gem5.opt configs/example/se.py"

# TODO add args for ruby system and


bm_name="blackscholes"
bm_dataset="simsmall"

bm_dir="${gem5_dir}/benchmarks/parsec-2.1"
bm_subdir="pkgs/apps/${bm_name}"
bm_build="inst/amd64-linux.gcc.pre" #/bin/${bm_name}"
bm_NTHREADS="1"

# Find .runconf
NTHREADS="${bm_NTHREADS}"
bm_runconf="${bm_dir}/${bm_subdir}/parsec/${bm_dataset}.runconf"

source ${bm_runconf}

bm_run_exec="${bm_dir}/${bm_subdir}/${bm_build}/${run_exec}"
run_path="${bm_dir}/pkgs/apps/${bm_name}/run"


# EXEC_CMD="${gem5_exec} -n 4 -c  ${bm_run_exec} -o \"${bm_NTHREADS} ${run_path}/in_4K.txt ${run_path}/prices.txt\""
# echo ${EXEC_CMD}
# eval ${EXEC_CMD}

cd ${run_path}


bm_args="${run_args}"
gem5_cmd="${gem5_dir}/${gem5_opt} ${gem5_dir}/${gem5_config}"
exec_cmd="${gem5_cmd} -n ${gem5_nCores} -c ${bm_run_exec} -o \"${run_args}\""
eval ${exec_cmd}

echo "Copying results to m5out"
mkdir  ${gem5_dir}/m5out -v
mkdir  ${gem5_dir}/m5out/${bm_name} -v
mv ${run_path}/m5out/* ${gem5_dir}/m5out/${bm_name}/ -f -v
echo " .. Done"
echo ""