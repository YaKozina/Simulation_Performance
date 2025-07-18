#!/bin/sh

#SBATCH --job-name=send_0nu
#SBATCH --mem=5G
#SBATCH --licenses=sps
#SBATCH --time=24:00:00
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1

INDEX=$1

source "${THRONG_DIR}/config/supernemo_profile.bash"
snswmgr_load_stack base@2024-09-04
snswmgr_load_setup falaise@5.1.2

OUT_BASE="/sps/nemo/scratch/ykozina/Falaise/tutorial/0nu/Tl208/25_10pow4_eve/jobs_done"
OUT_DIR="$OUT_BASE/job_$INDEX"
RUNTIME_FILE="$OUT_BASE/runtime_raw.txt"
FLSIMU_runtime="$OUT_BASE/FLSIMU_runtime_raw.txt"
FLRECO_runtime="$OUT_BASE/FLRECO_runtime_raw.txt"
mkdir -p "$OUT_DIR"

SIM_FILE="$OUT_DIR/Simu_0nu_Tl208_${INDEX}.brio"
RECO_FILE="$OUT_DIR/Reco_0nu_Tl208_${INDEX}.brio"

start_time=$(date +%s)

/sps/nemo/sw/redhat-9-x86_64/snsw/opt/falaise-5.1.2/bin/flsimulate \
    -c /sps/nemo/scratch/ykozina/Falaise/tutorial/0nu/Tl208/25_10pow4_eve/Simu_0nu_Tl208.conf \
    -o "$SIM_FILE"


mid_time_FLSIMU=$(date +%s)

/sps/nemo/sw/redhat-9-x86_64/snsw/opt/falaise-5.1.2/bin/flreconstruct \
    -i "$SIM_FILE" \
    -p /sps/nemo/sw/Falaise/install_develop/share/Falaise-4.1.0/resources/snemo/demonstrator/reconstruction/official-2.0.0.conf \
    -o "$RECO_FILE"

end_time=$(date +%s)
runtime=$((end_time - start_time))
flsimu_time=$((mid_time_FLSIMU - start_time))
flreco_time=$((end_time - mid_time_FLSIMU))

echo -e "$INDEX\t$SLURM_JOB_ID\t$runtime" >> "$RUNTIME_FILE"
echo -e "$INDEX\t$SLURM_JOB_ID\t$flsimu_time" >> "$FLSIMU_runtime"
echo -e "$INDEX\t$SLURM_JOB_ID\t$flreco_time" >> "$FLRECO_runtime"
