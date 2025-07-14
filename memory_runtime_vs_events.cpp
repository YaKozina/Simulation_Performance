void memory_runtime_vs_events() {
    gStyle->SetOptFit(0);  
    const char* isotopes[4] = {"Se82 bb-2nu", "Se82 bb-0nu", "Tl208 bg", "Bi214 bg"};
    const int n = 7;
    double events[n] = {100, 1000, 2500, 5000, 10000, 200000, 300000};
    double ex[n] = {0, 0, 0, 0, 0, 0, 0};

    Color_t colors[4]  = {kRed, kBlue, kGreen+2, kMagenta};
    Style_t markers[4] = {20, 21, 22, 23};

    double mem_y[4][n] = {
        {298.548866, 335.032300, 355.430100, 410.067800, 455.356200, 554.183800, 643.191600},
        {165.382300, 393.316900, 472.832828, 501.098300, 415.107200, 518.292700, 688.905200},
        {205.090000, 311.654747, 327.050909, 342.677500, 371.428500, 1313.177600, 1748.377600},
        {281.328061, 160.702449, 191.947172, 192.743333, 265.600000, 518.292700, 1214.566400}
    };

    double mem_ey[4][n] = {
        {62.974775, 43.366547, 35.083141, 41.846530, 30.254534, 52.473646, 39.885831},
        {62.056580, 50.552780, 50.310707, 27.679703, 75.608543, 46.517820, 72.908539},
        {65.727570, 52.224801, 51.477400, 60.804514, 61.081427, 64.943239, 75.576750},
        {86.743514, 28.824946, 43.407030, 34.939992, 41.500107, 46.517820, 67.721867}
    };

    double time_y[4][n] = {
        {30.360825, 101.740000, 219.960000, 437.640000, 842.850000, 20614.480000, 32755.800000},
        {16.200000, 129.350000, 469.313131, 761.640000, 1262.900000, 29430.210000, 48587.860000},
        {18.777778, 63.181818, 165.151515, 272.290000, 556.410000, 12133.510000, 17981.740000},
        {19.867347, 58.897959, 145.848485, 257.020833, 457.040404, 10262.370000, 16195.230000}
    };

    double time_ey[4][n] = {
        {10.983761, 12.938021, 33.434090, 61.125039, 134.338034, 2636.056192, 2478.581558},
        {3.906405, 28.670324, 179.441369, 135.746272, 139.145428, 3216.930749, 4752.329582},
        {18.753885, 17.949629, 18.671192, 40.838045, 78.717990, 1715.943026, 3102.710208},
        {9.965899, 20.176006, 49.688192, 56.349353, 83.446338, 1086.835145, 2111.553418}
    };

    
    ofstream mem_out("memory_fits.txt");
    ofstream time_out("runtime_fits.txt");

    TCanvas *c1 = new TCanvas("c1", "Memory and Runtime vs Events", 1800, 600);
    c1->Divide(2,1);

    
    c1->cd(1);
    TMultiGraph *mg_mem = new TMultiGraph();
    TLegend *leg_mem = new TLegend(0.12, 0.6, 0.5, 0.88);
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

        TF1 *fit = new TF1(Form("fit_mem_%d",i), "pol1", 0, 350000);
        fit->SetLineColor(colors[i]);
        fit->SetLineStyle(2);
        fit->SetLineWidth(2);

        gr->Fit(fit, "Q");  
        gr->GetListOfFunctions()->Add(fit);

        leg_mem->AddEntry(gr, isotopes[i], "p");
        leg_mem->AddEntry(fit, Form("%s Fit", isotopes[i]), "l");

        // ????? ? ????
        mem_out << "MEMORY fit for " << isotopes[i] << ":\n";
        mem_out << "  Slope (a) = " << fit->GetParameter(1) << " +/- " << fit->GetParError(1) << "\n";
        mem_out << "  Intercept (b) = " << fit->GetParameter(0) << " +/- " << fit->GetParError(0) << "\n";
          }

    mg_mem->SetTitle("Memory [MiB] vs n of Events/Job;Events per Job;Memory (MiB)");
    mg_mem->Draw("A PMC");
    leg_mem->Draw();

    
    c1->cd(2);
    TMultiGraph *mg_time = new TMultiGraph();
    TLegend *leg_time = new TLegend(0.12, 0.6, 0.5, 0.88);
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

        TF1 *fit = new TF1(Form("fit_time_%d",i), "pol1", 0, 350000);
        fit->SetLineColor(colors[i]);
        fit->SetLineStyle(2);
        fit->SetLineWidth(2);

        gr->Fit(fit, "Q");  
        gr->GetListOfFunctions()->Add(fit);

        leg_time->AddEntry(gr, isotopes[i], "p");
        leg_time->AddEntry(fit, Form("%s Fit", isotopes[i]), "l");
        time_out << "RUNTIME fit for " << isotopes[i] << ":\n";
        time_out << "  Slope (b) = " << fit->GetParameter(1) << " +/- " << fit->GetParError(1) << "\n";
        time_out << "  Intercept (a) = " << fit->GetParameter(0) << " +/- " << fit->GetParError(0) << "\n";
          }

    mg_time->SetTitle("Runtime [s] vs n of Events/Job;Events per Job;Runtime (s)");
    mg_time->Draw("A PMC");
    leg_time->Draw();
    c1->SaveAs("Memory_Runtime_vs_Events.png");
    c1->SaveAs("Memory_Runtime_vs_Events.root");
    mem_out.close();
    time_out.close();
    c1->Draw();
}

