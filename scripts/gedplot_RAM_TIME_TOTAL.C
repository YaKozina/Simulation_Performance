void plot_RAM_TIME_TOTAL() {
    std::ifstream in("data_for_root.txt");
    if (!in.is_open()) {
        std::cerr << "Cannot open file data_for_root.txt\n";
        return;
    }

    std::vector<double> events, mem_mean, mem_err, run_mean, run_err;
    std::string line;

    while (std::getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        double ev, mm, me, rm, re;
        if (!(iss >> ev >> mm >> me >> rm >> re)) continue;

        events.push_back(ev);
        mem_mean.push_back(mm);
        mem_err.push_back(me);
        run_mean.push_back(rm);
        run_err.push_back(re);
    }

    int n = events.size();
    if (n == 0) {
        std::cerr << "No data to plot\n";
        return;
    }

    TGraphErrors* g_mem = new TGraphErrors(n);
    TGraphErrors* g_run = new TGraphErrors(n);

    for (int i = 0; i < n; ++i) {
        g_mem->SetPoint(i, events[i], mem_mean[i]);
        g_mem->SetPointError(i, 0, mem_err[i]);

        g_run->SetPoint(i, events[i], run_mean[i]);
        g_run->SetPointError(i, 0, run_err[i]);
    }

    TCanvas* c = new TCanvas("c", "Memory and Runtime vs Events", 1600, 800);
    c->Divide(2, 1);

    // ---------- Memory Plot ----------
    c->cd(1);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);

    g_mem->SetTitle("Memory vs Events;Events per Job;RAM [MB]");
    g_mem->SetMarkerStyle(21);
    g_mem->SetMarkerSize(1.2);
    g_mem->SetMarkerColor(kBlack);
    g_mem->Draw("AP");

    double ymax_mem = g_mem->GetHistogram()->GetMaximum() * 1.1;
    g_mem->GetYaxis()->SetRangeUser(0, ymax_mem);

    TF1* fit_mem = new TF1("fit_mem", "pol1");
    g_mem->Fit(fit_mem, "Q");
    fit_mem->SetLineColor(kRed);
    fit_mem->SetLineWidth(2);
    fit_mem->Draw("SAME");

    double intercept_mem = fit_mem->GetParameter(0);
    double slope_mem = fit_mem->GetParameter(1);
    double e_intercept_mem = fit_mem->GetParError(0);
    double e_slope_mem = fit_mem->GetParError(1);
    double chi2_mem = fit_mem->GetChisquare();
    int ndf_mem = fit_mem->GetNDF();
    double chi2_ndf_mem = chi2_mem / ndf_mem;

    TLatex* latex_mem = new TLatex();
    latex_mem->SetNDC();
    latex_mem->SetTextSize(0.025);
    latex_mem->DrawLatex(0.60, 0.30, "y = ax + b");
    latex_mem->DrawLatex(0.60, 0.27, Form("a = %.5f #pm %.5f [MB/event]", slope_mem, e_slope_mem));
    latex_mem->DrawLatex(0.60, 0.24, Form("b = %.2f #pm %.2f [MB]", intercept_mem, e_intercept_mem));
    latex_mem->DrawLatex(0.60, 0.21, Form("#chi^{2} = %.2f, ndf = %d", chi2_mem, ndf_mem));
    latex_mem->DrawLatex(0.60, 0.18, Form("#chi^{2}/ndf = %.2f", chi2_ndf_mem));

    
    // ---------- Runtime Plot ----------
    c->cd(2);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);

    g_run->SetTitle("Runtime vs Events;Events per Job;Runtime [s]");
    g_run->SetMarkerStyle(20);
    g_run->SetMarkerSize(1.2);
    g_run->SetMarkerColor(kBlue + 1);
    g_run->Draw("AP");

    TF1* fit_run = new TF1("fit_run", "pol1");
    g_run->Fit(fit_run, "Q");
    fit_run->SetLineColor(kRed + 1);
    fit_run->SetLineWidth(2);
    fit_run->Draw("SAME");

    double intercept_run = fit_run->GetParameter(0);
    double slope_run = fit_run->GetParameter(1);
    double e_intercept_run = fit_run->GetParError(0);
    double e_slope_run = fit_run->GetParError(1);
    double chi2_run = fit_run->GetChisquare();
    int ndf_run = fit_run->GetNDF();
    double chi2_ndf_run = chi2_run / ndf_run;

    TLatex* latex_run = new TLatex();
    latex_run->SetNDC();
    latex_run->SetTextSize(0.025);
    latex_run->DrawLatex(0.60, 0.30, "y = ax + b");
    latex_run->DrawLatex(0.60, 0.27, Form("a = %.5f #pm %.5f [s/event]", slope_run, e_slope_run));
    latex_run->DrawLatex(0.60, 0.24, Form("b = %.2f #pm %.2f [s]", intercept_run, e_intercept_run));
    latex_run->DrawLatex(0.60, 0.21, Form("#chi^{2} = %.2f,  ndf = %d", chi2_run, ndf_run));
    latex_run->DrawLatex(0.60, 0.18, Form("#chi^{2}/ndf = %.2f", chi2_ndf_run));

    c->Print("events_vs_memory_runtime_300dpi.png", "png 300");
    c->SaveAs("events_vs_memory_runtime.root");
}

