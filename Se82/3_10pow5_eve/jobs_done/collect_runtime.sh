#!/bin/bash

START=65360798
END=65361010
OUTPUT="runtime_raw.txt"

echo -e "Index\tJobID\tRuntime sec" > "$OUTPUT"

index=0

for ((jobid=START; jobid<=END; jobid++)); do
    runtime_line=$(seff "$jobid" 2>/dev/null | grep "Job Wall-clock time")

    if [[ -n "$runtime_line" ]]; then
        time_str=$(echo "$runtime_line" | awk -F': ' '{print $2}' | tr -d ' ')
        
        # Парсимо h:m:s або m:s або s
        IFS='-' read -ra date_time <<< "$time_str"
        time_part="${date_time[-1]}"
        IFS=':' read -ra parts <<< "$time_part"

        # Обробка h:m:s, m:s, або s
        case ${#parts[@]} in
            3) hours=${parts[0]}; minutes=${parts[1]}; seconds=${parts[2]} ;;
            2) hours=0; minutes=${parts[0]}; seconds=${parts[1]} ;;
            1) hours=0; minutes=0; seconds=${parts[0]} ;;
            *) hours=0; minutes=0; seconds=0 ;;
        esac

        total_seconds=$((10#$hours * 3600 + 10#$minutes * 60 + 10#$seconds))
        echo -e "$index\t$jobid\t$total_seconds" >> "$OUTPUT"
    else
        echo -e "$index\t$jobid\tNO_DATA" >> "$OUTPUT"
    fi

    ((index++))
done

echo "✅ $OUTPUT"

