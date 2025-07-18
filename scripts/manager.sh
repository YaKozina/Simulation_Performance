#!/bin/bash

module load root

OUT_BASE="/paths/to/your/folder/10pow2_eve/jobs_done"


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
#     int n = 100;  - number of events in simulation configuration file
cat << 'EOF' > "$OUT_BASE/analyze_runtime.C"
void analyze_runtime() {
    int n = 100; 
    TCanvas* canvas_runtime = new TCanvas("canvas_runtime", "Runtime Analysis", 800, 600);
    FILE* f = fopen("runtime_raw.txt", "r");
    if (!f) {
        printf("Cannot open runtime_raw.txt\n");
        return;
    }

    std::vector<double> values;
    char line[256];
    fgets(line, sizeof(line), f); 
    while (fgets(line, sizeof(line), f)) {
        double val;
        if (sscanf(strrchr(line, '\t'), "\t%lf", &val) == 1)
            values.push_back(val);
    }
    fclose(f);

    if (values.empty()) {
        printf("No data\n");
        return;
    }

    double min = *std::min_element(values.begin(), values.end());
    double max = *std::max_element(values.begin(), values.end());

    auto h = new TH1D("h_runtime", "Runtime distribution;Time [s];Entries", 50, min - 1, max + 1);
    for (auto v : values)
        h->Fill(v);

    h->Fit("gaus", "0Q"); 
    h->Draw();
    canvas_runtime->SaveAs("runtime_distribution.png");

    FILE* out = fopen("runtime_fit_results.txt", "w");
    fprintf(out, "%d\n%.3f\n%.3f\n", n, h->GetMean(), h->GetStdDev());
    fclose(out);
}

EOF

# --------------------------
# FLSIMU_RUNTIME
# --------------------------
cat << 'EOF' > "$OUT_BASE/analyze_flsimu_runtime.C"
void analyze_flsimu_runtime() {
    int n = 100; 

    TCanvas* canvas_flsimu = new TCanvas("canvas_flsimu", "FLSIMU Runtime Analysis", 800, 600);
    FILE* f = fopen("FLSIMU_runtime_raw.txt", "r");
    if (!f) {
        printf("Cannot open FLSIMU_runtime_raw.txt\n");
        return;
    }

    std::vector<double> values;
    char line[256];
    fgets(line, sizeof(line), f); 
    while (fgets(line, sizeof(line), f)) {
        double val;
        if (sscanf(strrchr(line, '\t'), "\t%lf", &val) == 1) values.push_back(val);
    }
    fclose(f);

    if (values.empty()) {
        printf("No data\n");
        return;
    }

    double min = *std::min_element(values.begin(), values.end());
    double max = *std::max_element(values.begin(), values.end());

    auto h = new TH1D("h_flsimu", "FLSIMU Runtime distribution;Time [s];Entries", 50, min - 1, max + 1);
    for (auto v : values) h->Fill(v);

    h->Fit("gaus", "0Q"); // ??? ?????????, ??? ????????? ? ????? ?? ??????????????
    h->Draw();
    canvas_flsimu->SaveAs("flsimu_runtime_distribution.png");

    
    FILE* out = fopen("flsimu_runtime_fit_results.txt", "w");
    fprintf(out, "%d\n%.3f\n%.3f\n", n, h->GetMean(), h->GetStdDev());
    fclose(out);
}

EOF

# --------------------------
# FLRECO_RUNTIME
# --------------------------
cat << 'EOF' > "$OUT_BASE/analyze_flreco_runtime.C"
void analyze_flreco_runtime() {
    int n = 100; 

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

    if (values.empty()) {
        printf("No data\n");
        return;
    }

    double min = *std::min_element(values.begin(), values.end());
    double max = *std::max_element(values.begin(), values.end());

    auto h = new TH1D("h_flreco", "FLRECO Runtime distribution;Time [s];Entries", 50, min - 1, max + 1);
    for (auto v : values) h->Fill(v);

    h->Fit("gaus", "0Q"); 
    h->Draw();
    canvas_flreco->SaveAs("flreco_runtime_distribution.png");
    
    FILE* out = fopen("flreco_runtime_fit_results.txt", "w");
    fprintf(out, "%d\n%.3f\n%.3f\n", n, h->GetMean(), h->GetStdDev());
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
    FILE* f = fopen("memory_raw.txt", "r");
    if (!f) { printf("Cannot open memory_raw.txt\n"); return; }
int n_events=100;
    std::vector<double> values;
    char line[256];
    fgets(line, sizeof(line), f);  // skip header
    while (fgets(line, sizeof(line), f)) {
        double val;
        if (sscanf(strrchr(line, '\t'), "\t%lf", &val) == 1)
            values.push_back(val);
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
    double sigma_manual = std::sqrt(variance);

    double min = *std::min_element(values.begin(), values.end());
    double max = *std::max_element(values.begin(), values.end());

    auto h = new TH1D("h_memory", "Memory Usage;Memory [MB];Entries", 50, min - 1, max + 1);
    for (auto v : values) h->Fill(v);

    int max_bin = h->GetMaximumBin();
    double peak1_pos = h->GetBinCenter(max_bin);

    double min_bin_distance = 3; 
    int peak2_bin = -1;
    double second_max_content = -1;
    int n_bins = h->GetNbinsX();

    for (int b = 1; b <= n_bins; ++b) {
        if (b == max_bin) continue;
        if (std::abs(b - max_bin) < min_bin_distance) continue; 
        double cont = h->GetBinContent(b);
        if (cont > second_max_content) {
            second_max_content = cont;
            peak2_bin = b;
        }
    }

    if (peak2_bin < 0) peak2_bin = max_bin;
    double peak2_pos = h->GetBinCenter(peak2_bin);
    double bin_width = h->GetBinWidth(max_bin);

    double fit1_min = peak1_pos - 5 * bin_width;
    double fit1_max = peak1_pos + 5 * bin_width;
    double fit2_min = peak2_pos - 5 * bin_width;
    double fit2_max = peak2_pos + 5 * bin_width;

    TF1* f1 = new TF1("gaus1", "gaus", fit1_min, fit1_max);
    TF1* f2 = new TF1("gaus2", "gaus", fit2_min, fit2_max);


    h->Fit(f1, "NQ", "", fit1_min, fit1_max);
    h->Fit(f2, "NQ", "", fit2_min, fit2_max);

    FILE* out = fopen("memory_fit_results.txt", "w");
    fprintf(out, "%d\n%f\n%f\n", n_events, mean, sigma_manual);


    fclose(out);

    auto c = new TCanvas("c_memory", "Memory", 800, 600);
    h->SetLineColor(kBlue + 1);
    h->Draw("HIST");  
    c->SaveAs("memory_hist.png");

    TFile* fout = new TFile("memory_hist.root", "RECREATE");
    h->Write();
    fout->Close();
}

EOF

cd "$OUT_BASE"
root -l -b -q analyze_memory.C
echo "Memory info analyzed."

