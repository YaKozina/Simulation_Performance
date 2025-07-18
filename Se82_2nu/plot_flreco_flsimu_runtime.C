void plot_flreco_flsimu_runtime() {
    const char* files[2] = {"data_for_root_FLRECO_TIME.txt", "data_for_root_FLSIMU_TIME.txt"};
    const char* titles[2] = {
        "FLRECO Runtime vs Events;Events per Job;Time [s]",
        "FLSIMU Runtime vs Events;Events per Job;Time [s]"
    };
    const char* names[2] = {"FLRECO", "FLSIMU"};
    Color_t colors[2] = {kBlack, kBlue + 1};

    TCanvas* c = new TCanvas("c", "FLRECO and FLSIMU Runtime", 1600, 800);
    c->Divide(2, 1);

    for (int panel = 0; panel < 2; ++panel) {
        std::ifstream in(files[panel]);
        if (!in.is_open()) {
            std::cerr << "Cannot open file: " << files[panel] << "\n";
            continue;
        }

        std::vector<double> events, means, errors;
        std::string line;
        while (std::getline(in, line)) {
            if (line.empty() || line[0] == '#') continue;
            std::istringstream iss(line);
            double ev, mean, err;
            if (!(iss >> ev >> mean >> err)) {
                std::cerr << "Skipping malformed line: " << line << std::endl;
                continue;
            }
            events.push_back(ev);
            means.push_back(mean);
            errors.push_back(err);
        }

        int n = events.size();
        if (n == 0) {
            std::cerr << "No valid data in file: " << files[panel] << "\n";
            continue;
        }

        TGraphErrors* graph = new TGraphErrors(n);
        for (int i = 0; i < n; ++i) {
            graph->SetPoint(i, events[i], means[i]);
            graph->SetPointError(i, 0, errors[i]);
        }

        c->cd(panel + 1);
        gPad->SetLeftMargin(0.15);
        gPad->SetBottomMargin(0.15);

        graph->SetTitle(titles[panel]);
        graph->SetMarkerStyle(20 + panel);
        graph->SetMarkerSize(1.2);
        graph->SetMarkerColor(colors[panel]);
        graph->Draw("AP");

        TF1* fit = new TF1(Form("fit_%d", panel), "pol1");
        graph->Fit(fit, "Q");
        fit->SetLineColor(kRed);
        fit->SetLineWidth(2);
        fit->Draw("SAME");

        double A = fit->GetParameter(1);     // slope
        double B = fit->GetParameter(0);     // intercept
        double eA = fit->GetParError(1);
        double eB = fit->GetParError(0);

        double chi2 = fit->GetChisquare();
        int ndf = fit->GetNDF();
        double chi2_ndf = (ndf > 0) ? chi2 / ndf : 0;

        TLatex* latex = new TLatex();
        latex->SetNDC();
        latex->SetTextSize(0.025);
        latex->DrawLatex(0.60, 0.30, "y = ax + b");
        latex->DrawLatex(0.60, 0.27, Form("a = %.5f #pm %.5f [s/event]", A, eA));
        latex->DrawLatex(0.60, 0.24, Form("b = %.2f #pm %.2f [s]", B, eB));
        latex->DrawLatex(0.60, 0.21, Form("#chi^{2} = %.2f,  ndf = %d", chi2, ndf));
        latex->DrawLatex(0.60, 0.18, Form("#chi^{2}/ndf = %.2f", chi2_ndf));

        }


    c->Print("FLRECO_FLSIMU_runtime_300dpi.png", "png 300");
    c->SaveAs("FLRECO_FLSIMU_runtime.root");
}

