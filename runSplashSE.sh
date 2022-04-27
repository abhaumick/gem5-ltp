#!/bin/bash

gem5_nCores="16"

gem5_result_prefix="project/results"
gem5_test_case="${gem5_nCores}-se-nohotfix-conf4"

# gem5 Settings
gem5_dir="${PWD}"
gem5_opt="build/ECE666/gem5.opt"
gem5_config="configs/example/se.py"

gem5_args="${gem5_dir}/${gem5_config} -n ${gem5_nCores} --ruby"
gem5_exec="build/ECE666/gem5.opt ${gem5_args}"

# TODO add args for ruby system and

# SPLASH Settings
bm_dataset="simdev"


# bm_type+=( ["key2"]=val2 ["key3"]=val3 )

bm_names=(
    # "barnes"
    "fmm"
    "ocean-contig"
    "radiosity"
    "raytrace"
    "volrend"
    # "water-nsquared"
    # "water-spatial"
    "cholesky"
    "fft"
    "lu-contig"
    "radix"
)

bm_dir="${gem5_dir}/benchmarks/Splash-3/codes"

bm_NTHREADS=$((${gem5_nCores}))
NTHREADS="${bm_NTHREADS}"

# Find .runconf
source  "${bm_dir}/runconf"

gem5_result_dir="${gem5_dir}/${gem5_result_prefix}/m5out-splash-${gem5_test_case}"
mkdir ${gem5_result_dir}

# bm_name="blackscholes"

for bm_name in "${bm_names[@]}"; do

    echo ${bm_name}
    echo ${bm_type[${bm_name}]}

    bm_type=${splash_type[${bm_name}]}
    bm_subdir="${bm_type[${bm_name}]}/${bm_name}"
    bm_build=""
    run_exec=${splash_exec[${bm_name}]}
    echo ${run_exec}
    # bm_build="inst/amd64-linux.gcc.pre" #/bin/${bm_name}"
    # bm_NTHREADS="4"

    # source ${bm_runconf}

    run_exec=${splash_exec[${bm_name}]}
    bm_run_exec="${bm_dir}/${bm_subdir}/${bm_build}/${run_exec}"
    run_path="${bm_dir}/${bm_subdir}/${bm_build}"

    gem5_out_dir="${gem5_result_dir}/${bm_name}"
    rm -rf ${gem5_out_dir}
    mkdir ${gem5_out_dir} -v

    # Write Config to file
    cfg_file="${bm_name}.gem5conf"
    cfg_file_path="${gem5_out_dir}/${cfg_file}"
    touch ${cfg_file_path}
    printf "Writing RunConfig to ${cfg_file_path} \n"
    printf "gem5 Run Configuration\n" >> ${cfg_file_path}
    printf "gem5_dir=\"${gem5_dir}\"\n" >> ${cfg_file_path}
    printf "gem5_opt=\"${gem5_opt}\"\n" >> ${cfg_file_path}
    printf "gem5_config=\"${gem5_config}\"\n" >> ${cfg_file_path}
    printf "gem5_nCores=\"${gem5_nCores}\"\n" >> ${cfg_file_path}
    printf "gem5_args=\"${gem5_args}\"\n" >> ${cfg_file_path}

    printf "bm_name=\"${bm_name}\"\n" >> ${cfg_file_path}
    printf "bm_dataset=\"${bm_dataset}\"\n" >> ${cfg_file_path}
    printf "" >> ${cfg_file_path}

    # EXEC_CMD="${gem5_exec} -n 4 -c  ${bm_run_exec} -o \"${bm_NTHREADS} ${run_path}/in_4K.txt ${run_path}/prices.txt\""
    # echo ${EXEC_CMD}
    # eval ${EXEC_CMD}

    cd ${run_path}

    bm_args="${splash_args[${bm_name}]}"
    run_args=${bm_args}

    # Run Hardware Benchmark
    bm_hw_cmd="time ${bm_run_exec} ${bm_args}"
    eval ${bm_hw_cmd}

    gem5_cmd="${gem5_dir}/${gem5_opt} -d ${gem5_out_dir} ${gem5_args}"
    exec_cmd="${gem5_cmd} -c ${bm_run_exec} -o \"${run_args}\""
    # exec_cmd="${gem5_cmd} -c ${bm_run_exec} ${run_args}"
    echo ${exec_cmd}
    eval "nohup ${exec_cmd} &"

    debug_cmd="gdb --args ${gem5_dir}/${gem5_opt} -d ${gem5_out_dir} ${gem5_args} -c ${bm_run_exec} -o \"${run_args}\""
    # echo ${debug_cmd}
    # eval ${debug_cmd}

done

# echo "Copying results to m5out"
# rm -rf  ${gem5_dir}/m5out/${bm_name} -v
# mkdir  ${gem5_dir}/m5out -v
# mkdir  ${gem5_dir}/m5out/${bm_name} -v
# mv ${run_path}/m5out ${gem5_dir}/m5out/${bm_name}/ -f -v
# mv ${run_path}/*.log ${gem5_dir}/m5out/${bm_name}/ -f -v
echo " .. Done"
echo ""