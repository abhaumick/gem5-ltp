#!/bin/bash

gem5_nCores="4"

# gem5 Settings
gem5_dir="${PWD}"
gem5_opt="build/ECE666/gem5.opt"
gem5_config="configs/example/se.py"

gem5_args="${gem5_dir}/${gem5_config} -n ${gem5_nCores} --ruby"
gem5_exec="build/ECE666/gem5.opt ${gem5_args}"

# TODO add args for application and
toy_exec="hw1-mm"
toy_exec_dir="project/bin"
bm_name="toy-${toy_exec}"
toy_args="64 4"


bm_args="${run_args}"

gem5_cmd="${gem5_dir}/${gem5_opt}"
exec_cmd="${gem5_cmd} ${gem5_args} -c ${toy_exec_dir}/${toy_exec} -o \"${toy_args}\""
# eval ${exec_cmd}

debug_cmd="gdb --args ${gem5_dir}/${gem5_opt} ${gem5_args} -c ${toy_exec_dir}/${toy_exec} -o \"${toy_args}\""
echo ${debug_cmd}
eval ${debug_cmd}

echo "Copying results to m5out"
rm -rf  ${gem5_dir}/m5out/${bm_name} -v
mkdir  ${gem5_dir}/m5out -v
mkdir  ${gem5_dir}/m5out/${bm_name} -v
mv ${run_path}/m5out ${gem5_dir}/m5out/${bm_name}/ -f -v
mv ${run_path}/*.log ${gem5_dir}/m5out/${bm_name}/ -f -v
echo " .. Done"
echo ""