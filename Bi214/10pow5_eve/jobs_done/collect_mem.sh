#!/bin/bash

START=66698656
END=66698759
OUTPUT="memory_raw.txt"

echo -e "Index\tJobID\tMemory_MB" > "$OUTPUT"

index=0

for ((jobid=START; jobid<=END; jobid++)); do
    mem_line=$(seff "$jobid" 2>/dev/null | grep "Memory Utilized")

    if [[ -n "$mem_line" ]]; then
        value=$(echo "$mem_line" | awk '{print $3}')
        unit=$(echo "$mem_line" | awk '{print $4}')

        # ??????????? ? ?????????
        case "$unit" in
            KB) value=$(awk "BEGIN {print $value/1024}") ;;
            MB) ;;  # ???????? ?? ?
            GB) value=$(awk "BEGIN {print $value*1024}") ;;
            *) value="UNKNOWN" ;;
        esac

        echo -e "$index\t$jobid\t$value" >> "$OUTPUT"
    else
        echo -e "$index\t$jobid\tNO_DATA" >> "$OUTPUT"
    fi

    ((index++))
done

echo "? $OUTPUT"

