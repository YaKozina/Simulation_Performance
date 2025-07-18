#!/bin/bash

OUT="data_for_root_FLRECO_TIME.txt"
> "$OUT"  

for dir in *_eve; do
    jobdir="${dir}/jobs_done"
    run_file="${jobdir}/flreco_runtime_fit_results.txt"

    if [[ ! -f "$run_file" ]]; then
        echo "Skipping $dir: missing result file $run_file"
        continue
    fi

    n_events=$(sed -n '1p' "$run_file")
    run_mean=$(sed -n '2p' "$run_file")
    run_err=$(sed -n '3p' "$run_file")

    if [[ -z "$n_events" || -z "$run_mean" || -z "$run_err" ]]; then
        echo "Skipping $dir: incomplete data in $run_file"
        continue
    fi

printf "%-10d %-12.3f %-12.3f\n" "$n_events" "$run_mean" "$run_err" >> "$OUT"

done

