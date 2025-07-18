void analyze_memory() {
    int n = 200000;
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
