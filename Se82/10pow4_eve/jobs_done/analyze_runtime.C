void analyze_runtime() {
    int n = 10000;  

    TCanvas* canvas_runtime = new TCanvas("canvas_runtime", "Runtime Analysis", 800, 600);
    FILE* f = fopen("runtime_raw.txt", "r");
    if (!f) {
        printf("Cannot open runtime_raw.txt\n");
        return;
    }

    std::vector<double> values;
    char line[256];
    fgets(line, sizeof(line), f); // ?????????? ?????????
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

    auto h = new TH1D("h_runtime", "Runtime distribution;Time [s];Entries", 30, min - 1, max + 1);
    for (auto v : values)
        h->Fill(v);

    h->Fit("gaus", "0Q"); 
    h->Draw();
    canvas_runtime->SaveAs("runtime_distribution.png");

    FILE* out = fopen("runtime_fit_results.txt", "w");
    fprintf(out, "%d\n%.3f\n%.3f\n", n, h->GetMean(), h->GetStdDev());
    fclose(out);
}

