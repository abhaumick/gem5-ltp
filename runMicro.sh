#!/bin/bash

gem5_nCores="4"

# gem5 Settings
gem5_dir="${PWD}"
gem5_opt="build/ECE666/gem5.opt"
gem5_config="configs/example/se.py"

# gem5_args="--debug-flags=ProtocolTrace ${gem5_dir}/${gem5_config} -n ${gem5_nCores} --ruby"
gem5_args="${gem5_dir}/${gem5_config} -n ${gem5_nCores} --ruby --cpu-type=DerivO3CPU"
run_dir="${gem5_dir}/results/ubench_loadStore"
gem5_exec="build/ECE666/gem5.opt ${gem5_args}"

# TODO add args for application and
toy_exec="ubench_loadStore"
toy_exec_dir="../binaries"
bm_name="toy-${toy_exec}"
toy_args="4 1024 8"


bm_args="${run_args}"

gem5_cmd="${gem5_dir}/${gem5_opt} -d ${run_dir}"
exec_cmd="${gem5_cmd} ${gem5_args} -c ${toy_exec_dir}/${toy_exec} -o \"${toy_args}\""
# eval ${exec_cmd} > protocoltrace.txt

val_exec="valgrind --leak-check=full --track-origins=yes --suppressions=util/valgrind-suppressions"

val_cmd="${val_exec} ${exec_cmd}"

debug_cmd="gdb --args ${gem5_dir}/${gem5_opt} ${gem5_args} -c ${toy_exec_dir}/${toy_exec} -o \"${toy_args}\""
echo ${exec_cmd}
eval ${exec_cmd}

echo "Copying results to m5out"
echo " .. Done"
echo ""