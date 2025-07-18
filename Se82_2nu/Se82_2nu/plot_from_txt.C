void plot_from_txt() {
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

    TCanvas* c = new TCanvas("c", "Memory and Runtime vs Events", 1200, 600);
    c->Divide(2, 1);

    // ---------- Memory Plot ----------
    c->cd(1);
    g_mem->SetTitle("Memory vs Events;Events per Job;RAM [MB]");
    g_mem->SetMarkerStyle(21);
    g_mem->SetMarkerSize(1.2);
    g_mem->SetMarkerColor(kBlack);
    g_mem->Draw("AP");

    TF1* fit_mem = new TF1("fit_mem", "pol1");
    g_mem->Fit(fit_mem, "Q");
    fit_mem->SetLineColor(kRed);
    fit_mem->SetLineWidth(2);
    fit_mem->Draw("SAME");

    double a_mem = fit_mem->GetParameter(0);
    double b_mem = fit_mem->GetParameter(1);
    double ea_mem = fit_mem->GetParError(0);
    double eb_mem = fit_mem->GetParError(1);
    double r2_mem = g_mem->GetCorrelationFactor() * g_mem->GetCorrelationFactor();

    TLatex* latex_mem = new TLatex();
    latex_mem->SetNDC();
    latex_mem->SetTextSize(0.045);
    latex_mem->DrawLatex(0.15, 0.85, "Fit: y = a + b#upointx");
    latex_mem->DrawLatex(0.15, 0.80, Form("a = %.2f #pm %.2f", a_mem, ea_mem));
    latex_mem->DrawLatex(0.15, 0.75, Form("b = %.5f #pm %.5f", b_mem, eb_mem));
    latex_mem->DrawLatex(0.15, 0.70, Form("R^{2} = %.3f", r2_mem));

    TLegend* leg_mem = new TLegend(0.60, 0.85, 0.88, 0.95);
    leg_mem->SetTextSize(0.045);
    leg_mem->AddEntry(g_mem, "Memory", "p");
    leg_mem->AddEntry(fit_mem, "Linear Fit", "l");
    leg_mem->Draw();

    // ---------- Runtime Plot ----------
    c->cd(2);
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

    double a_run = fit_run->GetParameter(0);
    double b_run = fit_run->GetParameter(1);
    double ea_run = fit_run->GetParError(0);
    double eb_run = fit_run->GetParError(1);
    double r2_run = g_run->GetCorrelationFactor() * g_run->GetCorrelationFactor();

    TLatex* latex_run = new TLatex();
    latex_run->SetNDC();
    latex_run->SetTextSize(0.045);
    latex_run->DrawLatex(0.15, 0.85, "Fit: y = a + b#upointx");
    latex_run->DrawLatex(0.15, 0.80, Form("a = %.2f #pm %.2f", a_run, ea_run));
    latex_run->DrawLatex(0.15, 0.75, Form("b = %.5f #pm %.5f", b_run, eb_run));
    latex_run->DrawLatex(0.15, 0.70, Form("R^{2} = %.3f", r2_run));

    TLegend* leg_run = new TLegend(0.60, 0.85, 0.88, 0.95);
    leg_run->SetTextSize(0.045);
    leg_run->AddEntry(g_run, "Runtime", "p");
    leg_run->AddEntry(fit_run, "Linear Fit", "l");
    leg_run->Draw();

    c->SaveAs("events_vs_memory_runtime.png");
    c->SaveAs("events_vs_memory_runtime.root");
}

