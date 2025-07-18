void memory_runtime_vs_events() {
    gStyle->SetOptFit(0);  
    const char* isotopes[4] = {"Se82 bb-2nu", "Se82 bb-0nu", "Tl208 bg", "Bi214 bg"};
    const int n = 8;
    double events[n] = {100, 10000, 50000, 100000, 150000, 200000, 250000, 300000};
    double ex[n] = {0, 0, 0, 0, 0, 0, 0, 0};

    Color_t colors[4]  = {kRed, kBlue, kGreen+2, kMagenta};
    Style_t markers[4] = {20, 21, 22, 23};

    double mem_y[4][n] = {
        {224.994, 433.998, 387.352, 404.586122, 404.505135, 537.592167, 533.494757, 553.427},
        {190.408, 385.262, 431.373, 477.329, 534.278, 582.981, 655.467, 709.037},
        {223.666, 301.104, 465.198, 738.946, 937.235, 1188.966, 1430.733, 1684.275},
        {230.827, 271.074, 362.566, 525.049, 668.642, 818.308, 1006.583, 1121.948}
    };

    double mem_ey[4][n] = {
        {98.925, 52.242, 49.081, 64.515066, 65.615304, 92.788366, 72.569353, 95.595831},
        {61.646, 53.273, 55.427, 59.784, 63.166, 66.396, 56.622, 41.519},
        {99.046, 56.078, 44.770, 67.892, 53.061, 64.648, 59.358, 69.775},
        {70.465, 44.600, 27.000, 40.149, 33.984, 38.460, 77.970, 50.676}
    };

    double time_y[4][n] = {
        {17.696, 846.170, 4258.783, 9889.2, 14914.2, 20244.13, 25769.57, 30857.73},
        {21.100, 1477.72, 7626.59, 15704.2, 23358.74, 32642.2, 41363.89, 51491.65},
        {12.950, 573.25, 2724.89, 5850.88, 8622.65, 11603.42, 15108.41, 17351.47},
        {37.202, 492.64, 2419.57, 5012.1, 7663.18, 9934.85, 12466.21, 14828.87}
    };

    double time_ey[4][n] = {
        {3.501, 156.819, 2172.968, 1282.78, 2282.104, 3179.017, 3745.674, 3913.507},
        {3.708, 204.947, 910.9, 1687.854, 3697.992, 3633.749, 4794.58, 7431.336},
        {2.080, 76.42, 473.752, 942.357, 1256.981, 1715.592, 1965.36, 2326.864},
        {12.972, 64.985, 360.364, 661.177, 949.515, 1279.318, 1457.917, 2113.61}
    };

 TCanvas *c1 = new TCanvas("c1", "Memory and Runtime vs Events", 3000, 1200);
    c1->SetWindowSize(3000 + (3000 - c1->GetWw()), 1200 + (1200 - c1->GetWh()));
    c1->Divide(2, 1);

    // -------- Memory vs Events --------
    c1->cd(1);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);

    TMultiGraph *mg_mem = new TMultiGraph();
    TLegend *leg_mem = new TLegend(0.20, 0.6, 0.5, 0.88);
    leg_mem->SetBorderSize(0);
    leg_mem->SetTextSize(0.03);
    leg_mem->SetHeader("Memory vs Events", "C");

    for(int i=0; i<4; i++) {
        TGraphErrors *gr = new TGraphErrors(n, events, mem_y[i], ex, mem_ey[i]);
        gr->SetMarkerStyle(markers[i]);
        gr->SetMarkerColor(colors[i]);
        gr->SetLineColor(colors[i]);
        gr->SetLineWidth(2);

        mg_mem->Add(gr, "P");

        TF1 *fit = new TF1(Form("fit_mem_%d", i), "pol1", 0, 350000);
        fit->SetLineColor(colors[i]);
        fit->SetLineStyle(1);
        fit->SetLineWidth(2);
        gr->Fit(fit, "Q");
        gr->GetListOfFunctions()->Add(fit);

        leg_mem->AddEntry(gr, isotopes[i], "p");
        leg_mem->AddEntry(fit, Form("%s Fit", isotopes[i]), "l");

        ofstream mem_out(Form("memory_fit_%d.txt", i));
        mem_out << "100\n" << fit->GetParameter(1) << "\n" << fit->GetParError(1) << "\n";
        mem_out.close();
    }

    mg_mem->SetTitle("Memory [MiB] vs n of Events/Job;Events per Job;Memory (MiB)");
    mg_mem->Draw("A PMC");
    mg_mem->GetXaxis()->SetTitleSize(0.045);
    mg_mem->GetYaxis()->SetTitleSize(0.045);
    mg_mem->GetXaxis()->SetLabelSize(0.035);
    mg_mem->GetYaxis()->SetLabelSize(0.035);
    leg_mem->Draw();

    // -------- Runtime vs Events --------
    c1->cd(2);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);

    TMultiGraph *mg_time = new TMultiGraph();
    TLegend *leg_time = new TLegend(0.20, 0.6, 0.5, 0.88);
    leg_time->SetBorderSize(0);
    leg_time->SetTextSize(0.03);
    leg_time->SetHeader("Runtime vs n of Events/Job", "C");

    for(int i=0; i<4; i++) {
        TGraphErrors *gr = new TGraphErrors(n, events, time_y[i], ex, time_ey[i]);
        gr->SetMarkerStyle(markers[i]);
        gr->SetMarkerColor(colors[i]);
        gr->SetLineColor(colors[i]);
        gr->SetLineWidth(2);

        mg_time->Add(gr, "P");

        TF1 *fit = new TF1(Form("fit_time_%d", i), "pol1", 0, 350000);
        fit->SetLineColor(colors[i]);
        fit->SetLineStyle(1);
        fit->SetLineWidth(2);
        gr->Fit(fit, "Q");
        gr->GetListOfFunctions()->Add(fit);

        leg_time->AddEntry(gr, isotopes[i], "p");
        leg_time->AddEntry(fit, Form("%s Fit", isotopes[i]), "l");

        ofstream time_out(Form("runtime_fit_%d.txt", i));
        time_out << "100\n" << fit->GetParameter(1) << "\n" << fit->GetParError(1) << "\n";
        time_out.close();
    }

    mg_time->SetTitle("Runtime [s] vs n of Events/Job;Events per Job;Runtime (s)");
    mg_time->Draw("A PMC");
    mg_time->GetXaxis()->SetTitleSize(0.045);
    mg_time->GetYaxis()->SetTitleSize(0.045);
    mg_time->GetXaxis()->SetLabelSize(0.035);
    mg_time->GetYaxis()->SetLabelSize(0.035);
    leg_time->Draw();

    // Save output
    c1->Print("Memory_Runtime_vs_Events.png", "png 300");
    c1->SaveAs("Memory_Runtime_vs_Events.root");
}

