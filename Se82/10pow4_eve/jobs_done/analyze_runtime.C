void analyze_runtime() {
    FILE* f = fopen("runtime_raw.txt", "r");
    if (!f) {
        printf("Cannot open runtime_raw.txt\n");
        return;
    }

    int n_events = 10000;
    std::vector<double> values;
    char line[256];

    fgets(line, sizeof(line), f); // skip header
    while (fgets(line, sizeof(line), f)) {
        double val;
        if (sscanf(strrchr(line, '\t'), "\t%lf", &val) == 1)
            values.push_back(val);
    }
    fclose(f);

    if (values.empty()) {
        printf("No data found in runtime_raw.txt\n");
        return;
    }

    double sum = 0.0, sum_sq = 0.0;
    for (double v : values) {
        sum += v;
        sum_sq += v * v;
    }
    double mean = sum / values.size();
    double variance = (sum_sq / values.size()) - (mean * mean);
    double sigma_manual = std::sqrt(variance);

    double min = *std::min_element(values.begin(), values.end());
    double max = *std::max_element(values.begin(), values.end());

    auto h = new TH1D("h_runtime", "Runtime;Time [s];Entries", 200, min - 1, max + 1);
    for (double v : values) h->Fill(v);

    FILE* out = fopen("runtime_fit_results.txt", "w");
    fprintf(out, "%d\n%f\n%f\n", n_events, mean, sigma_manual);
    fclose(out);

    auto c = new TCanvas("c_runtime", "Runtime", 800, 600);
    h->SetLineColor(kRed + 1);
    h->Draw("HIST");
    c->SaveAs("runtime_hist.png");

    TFile* fout = new TFile("runtime_hist.root", "RECREATE");
    h->Write();
    fout->Close();
}

