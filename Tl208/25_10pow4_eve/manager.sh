#!/bin/bash

module load root

OUT_BASE="/sps/nemo/scratch/ykozina/Falaise/tutorial/0nu/Tl208/25_10pow4_eve/jobs_done"
RUNTIME_FILE="$OUT_BASE/runtime_raw.txt"
FLSIMU_RUNTIME_FILE="$OUT_BASE/FLSIMU_runtime_raw.txt"
FLRECO_RUNTIME_FILE="$OUT_BASE/FLRECO_runtime_raw.txt"
MEMORY_FILE="$OUT_BASE/memory_raw.txt"

mkdir -p "$OUT_BASE/log"

echo -e "Index\tJobID\tRuntime sec" > "$RUNTIME_FILE"
echo -e "Index\tJobID\tFLSIMU Runtime sec" > "$FLSIMU_RUNTIME_FILE"
echo -e "Index\tJobID\tFLRECO Runtime sec" > "$FLRECO_RUNTIME_FILE"

for i in {0..99}; do
    sbatch send_0nu.sh $i
    sleep 0.2
done

echo "Waiting for jobs to finish..."
while squeue -u "$USER" | grep -q "send_0nu"; do
    sleep 15
done
echo "All jobs completed."

# --------------------------
# RUNTIME
# --------------------------
cat << 'EOF' > "$OUT_BASE/analyze_runtime.C"
void analyze_runtime() {
    int n = 250000;
    TCanvas* canvas_runtime = new TCanvas("canvas_runtime", "Runtime Analysis", 800, 600);
    FILE* f = fopen("runtime_raw.txt", "r");
    if (!f) { printf("Cannot open runtime_raw.txt\n"); return; }

    std::vector<double> values;
    char line[256];
    fgets(line, sizeof(line), f);
    while (fgets(line, sizeof(line), f)) {
        double val;
        if (sscanf(strrchr(line, '\t'), "\t%lf", &val) == 1) values.push_back(val);
    }
    fclose(f);
    if (values.empty()) { printf("No data\n"); return; }

    double min = *std::min_element(values.begin(), values.end());
    double max = *std::max_element(values.begin(), values.end());
    auto h = new TH1D("h_runtime", "Runtime distribution;Time [s];Entries", 30, min - 1, max + 1);
    for (auto v : values) h->Fill(v);

    h->Fit("gaus", "0Q");
    h->Draw();
    canvas_runtime->SaveAs("runtime_distribution.png");

    auto f1 = h->GetFunction("gaus");
    FILE* out = fopen("runtime_fit_results.txt", "w");
    fprintf(out, "%d\n%.3f\n%.3f\n", n, f1->GetParameter(1), f1->GetParameter(2));
    fclose(out);
}
EOF

# --------------------------
# FLSIMU_RUNTIME
# --------------------------
cat << 'EOF' > "$OUT_BASE/analyze_flsimu_runtime.C"
void analyze_flsimu_runtime() {
    int n = 250000;
    TCanvas* canvas_flsimu = new TCanvas("canvas_flsimu", "FLSIMU Runtime Analysis", 800, 600);
    FILE* f = fopen("FLSIMU_runtime_raw.txt", "r");
    if (!f) { printf("Cannot open FLSIMU_runtime_raw.txt\n"); return; }

    std::vector<double> values;
    char line[256];
    fgets(line, sizeof(line), f);
    while (fgets(line, sizeof(line), f)) {
        double val;
        if (sscanf(strrchr(line, '\t'), "\t%lf", &val) == 1) values.push_back(val);
    }
    fclose(f);
    if (values.empty()) { printf("No data\n"); return; }

    double min = *std::min_element(values.begin(), values.end());
    double max = *std::max_element(values.begin(), values.end());
    auto h = new TH1D("h_flsimu", "FLSIMU Runtime distribution;Time [s];Entries", 30, min - 1, max + 1);
    for (auto v : values) h->Fill(v);

    h->Fit("gaus", "0Q");
    h->Draw();
    canvas_flsimu->SaveAs("flsimu_runtime_distribution.png");

    auto f1 = h->GetFunction("gaus");
    FILE* out = fopen("flsimu_runtime_fit_results.txt", "w");
    fprintf(out, "%d\n%.3f\n%.3f\n", n, f1->GetParameter(1), f1->GetParameter(2));
    fclose(out);
}
EOF

# --------------------------
# FLRECO_RUNTIME
# --------------------------
cat << 'EOF' > "$OUT_BASE/analyze_flreco_runtime.C"
void analyze_flreco_runtime() {
    int n = 250000;
    TCanvas* canvas_flreco = new TCanvas("canvas_flreco", "FLRECO Runtime Analysis", 800, 600);
    FILE* f = fopen("FLRECO_runtime_raw.txt", "r");
    if (!f) { printf("Cannot open FLRECO_runtime_raw.txt\n"); return; }

    std::vector<double> values;
    char line[256];
    fgets(line, sizeof(line), f);
    while (fgets(line, sizeof(line), f)) {
        double val;
        if (sscanf(strrchr(line, '\t'), "\t%lf", &val) == 1) values.push_back(val);
    }
    fclose(f);
    if (values.empty()) { printf("No data\n"); return; }

    double min = *std::min_element(values.begin(), values.end());
    double max = *std::max_element(values.begin(), values.end());
    auto h = new TH1D("h_flreco", "FLRECO Runtime distribution;Time [s];Entries", 30, min - 1, max + 1);
    for (auto v : values) h->Fill(v);

    h->Fit("gaus", "0Q");
    h->Draw();
    canvas_flreco->SaveAs("flreco_runtime_distribution.png");

    auto f1 = h->GetFunction("gaus");
    FILE* out = fopen("flreco_runtime_fit_results.txt", "w");
    fprintf(out, "%d\n%.3f\n%.3f\n", n, f1->GetParameter(1), f1->GetParameter(2));
    fclose(out);
}
EOF

cd "$OUT_BASE"
root -l -b -q analyze_runtime.C
root -l -b -q analyze_flsimu_runtime.C
root -l -b -q analyze_flreco_runtime.C
echo "Runtime info analyzed."

echo "120 seconds to wait for memory info to become available..."
sleep 120

echo -e "Index\tJobID\tMemory MB" > "$MEMORY_FILE"

tail -n +2 "$RUNTIME_FILE" | while IFS=$'\t' read -r idx jobid runtime; do
    mem_line=$(seff "$jobid" | grep "Memory Utilized")
    echo "JobID $jobid -> $mem_line"
    mem_value=$(echo "$mem_line" | awk '{print $3}')
    echo -e "$idx\t$jobid\t$mem_value" >> "$MEMORY_FILE"
done
echo "Memory saved to: $MEMORY_FILE"

# --------------------------
# MEMORY
# --------------------------
cat << 'EOF' > "$OUT_BASE/analyze_memory.C"
void analyze_memory() {
    int n = 250000;
    TCanvas* canvas_memory = new TCanvas("canvas_memory", "Memory Usage Analysis", 800, 600);
    FILE* f = fopen("memory_raw.txt", "r");
    if (!f) { printf("Cannot open memory_raw.txt\n"); return; }

    std::vector<double> values;
    char line[256];
    fgets(line, sizeof(line), f);
    while (fgets(line, sizeof(line), f)) {
        double val;
        if (sscanf(strrchr(line, '\t'), "\t%lf", &val) == 1) values.push_back(val);
    }
    fclose(f);
    if (values.empty()) { printf("No data\n"); return; }

    double sum = 0.0, sum_sq = 0.0;
    for (auto v : values) {
        sum += v;
        sum_sq += v * v;
    }
    double mean = sum / values.size();
    double variance = (sum_sq / values.size()) - (mean * mean);
    double sigma = std::sqrt(variance);

    auto h = new TH1D("h_memory", "Memory Usage distribution;Memory [MB];Entries", 30, mean - 3*sigma, mean + 3*sigma);
    for (auto v : values) h->Fill(v);

    h->Fit("gaus", "0Q");
    h->Draw();
    canvas_memory->SaveAs("memory_distribution.png");

    FILE* out = fopen("memory_fit_results.txt", "w");
    fprintf(out, "%d\n%.3f\n%.3f\n", n, mean, sigma);
    fclose(out);

    printf("Memory Analysis:\n");
    printf("Mean: %.3f MB\n", mean);
    printf("Sigma: %.3f MB\n", sigma);
}
EOF

cd "$OUT_BASE"
root -l -b -q analyze_memory.C
echo "Memory info analyzed."

