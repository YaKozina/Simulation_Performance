#!/bin/bash

OUT="data_for_root.txt"
> "$OUT" 

for dir in *_eve; do
    jobdir="${dir}/jobs_done"

    mem_file="${jobdir}/memory_fit_results.txt"
    run_file="${jobdir}/runtime_fit_results.txt"

    [[ -f "$mem_file" && -f "$run_file" ]] || {
        echo "Skipping $dir: missing result file(s)"
        continue
    }

    events=$(sed -n '1p' "$mem_file")
    mem_mean=$(sed -n '2p' "$mem_file")
    mem_err=$(sed -n '3p' "$mem_file")

    run_mean=$(sed -n '2p' "$run_file")
    run_err=$(sed -n '3p' "$run_file")

    if [[ -z "$events" || -z "$mem_mean" || -z "$mem_err" || -z "$run_mean" || -z "$run_err" ]]; then
        echo "Skipping $dir: incomplete data"
        continue
    fi

printf "%-10d %-12.3f %-12.3f %-12.3f %-12.3f\n" "$events" "$mem_mean" "$mem_err" "$run_mean" "$run_err" >> "$OUT"

done

