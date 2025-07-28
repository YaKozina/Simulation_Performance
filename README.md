# Simulation_Performance
Quantify computation requirements (time, memory) for simulation

The original task: https://github.com/Shoram444/SN_simulation_setup_codes/issues/1  
My task: https://github.com/YaKozina/study_Falaise/issues/1  

**At this moment (July 2025), results are available for the following processes:**

* Se-82 0νββ (foil_bulk)  
* Se-82 2νββ (foil_bulk)  
* Tl-208 (foil_bulk SOURCE BACKGROUND)  
* Bi-214 (foil_bulk SOURCE BACKGROUND)  

To this study may be included some other processes. The design is not perfect but in future it may be improved to make the software work better and more efficiently.

---

## Files Navigation

The **main** branch contains:  
- `Bi214/`, `Se82/`, `Se82_2nu/`, `Tl208/`
- `scripts/`
- `COMSUMED_RAM_TIME_approximation_coeff.pdf`
- `Memory_Runtime_vs_Events.png`

---

Each **folder named after an isotope (e.g. Se82)** contains:

#### a) Event Subfolders

Each folder named in the format `<NUMBER>_eve` (e.g. `10powN_eve/`, `n_10powN_eve/`) contains:

- Configuration files:
  - `Setup_<process>_<isotope>.conf` – simulation setup file
  - `Simu_<process>_<isotope>.conf` – simulation configuration file

- Scripts:
  - `manager.sh` – **main script** for submitting jobs via `send_0nu.sh` and analyzing RAM usage
  - `send_0nu.sh` – SLURM job submission script (loads Falaise, runs simulation and reconstruction, processes runtime)

- `jobs_done/` folder:
  Contains key output and analysis files for the corresponding number of events:

---

### RAM PROCESSING:

- `analyze_memory.C` — analyzes memory from `memory_raw.txt`, output:
  - `memory_fit_results.txt` — (result of `analyze_memory.C`) gives the number of events, the mean value and the std deviation
  - `memory_hist.root` *(not included in this repository)*
  -  `memory_hist.png` — the picture of the RAM usage distribution 
- `memory_raw.txt` — file with the list of values of used memory for each job with corresponding ID (`seff id | grep "Memory Utilized"`) processed by `manager.sh` & `send_0nu.sh`  

---

### TOTAL RUNTIME PROCESSING:

- `analyze_runtime.C` — analyzes memory from `runtime_raw.txt`, output:
  - `runtime_fit_results.txt` — (result of `analyze_runtime.C`) gives the number of events, the mean value and the std. deviation  
  - `runtime_hist.root` *(not included in the repository)*
  - `runtime_distribution.png` — the picture of the runtime distribution   
- `runtime_raw.txt` — file with the list of values of runtime for each job with corresponding ID proceeded by `manager.sh` & `send_0nu.sh`  

---

### ONLY SIMULATION TIME PROCESSING:

- `analyze_flsimu_runtime.C` — analyzes memory from `FLSIMU_runtime_raw.txt`, output:
  - `flsimu_runtime_fit_results.txt` — (result of `analyze_flsimu_runtime.C`) gives the number of events, the mean value and the std. deviation
  - `flsimu_runtime_distribution.png` — the picture of the flsimulate runtime distribution  
- `FLSIMU_runtime_raw.txt` — file with the list of values of ONLY flsimulate runtime for each job with corresponding ID proceeded by `manager.sh` & `send_0nu.sh`  

---

### ONLY RECONSTRUCTION TIME PROCESSING:

- `analyze_flreco_runtime.C` — analyzes memory from `FLRECO_runtime_raw.txt`, output
  - `flreco_runtime_fit_results.txt` — (result of `analyze_flreco_runtime.C`) gives the number of events, the mean value and the std. deviation 
  - `flreco_runtime_distribution.png`  — the picture of the flreconstruct runtime distribution
- `FLRECO_runtime_raw.txt` — file with the list of values of ONLY flreconstruct runtime for each job with corresponding ID proceeded by `manager.sh` & `send_0nu.sh`  

---

### (OPTIONAL) Emergency Scripts

- `collect_mem.sh` — produces `memory_raw.txt` manually extracting RAM for each job in the arbitrary selected range of IDs for which the `..._raw.txt` file wasn't created  
- `collect_runtime.sh` — produces `runtime_raw.txt` manually extracting runtime for each job in the arbitrary selected range of IDs for which the `..._raw.txt` file wasn't created

**!** These scripts are **not included** in every `jobs_done` folder because they **are used only in emergency cases** when the jobs were not processed properly and `runtime_raw.txt` or `memory_raw.txt` weren’t produced automatically by `manager.sh` and should be added manually.

---

#### b) Processing & Visualization Scripts

##### RAM & Total Runtime vs Events

- `prepare_RAM_TIME_DATA_to_txt.sh` — extracts from each folder `jobs_done` in each folder `<NUMBER>_eve` data from `<ram/time>_fit_results.txt`, produces `data_for_root.txt` (contains number of events, mean value of RAM, std deviation of RAM, mean value of total runtime and std deviation of total runtime)  
- `plot_RAM_TIME_TOTAL.C` — produces `events_vs_memory_runtime_300dpi.png` — graphs RAM(Events) & runtime(Events)

---

##### flsimulate & flreconstruct Time Processing

- `prepare_FLSIMU_TIME_DATA_to_txt.sh` — extracts from each folder `jobs_done` in each folder `<NUMBER>_eve` data from `flsimu_runtime_fit_results.txt`, produces `data_for_root_FLSIMU_TIME.txt` (contains number of events, mean value of flsimulate time and std deviation)  
- `prepare_FLRECO_TIME_DATA_to_txt.sh` — extracts from each folder `jobs_done` in each folder `<NUMBER>_eve` data from `flreco_runtime_fit_results.txt`, produces `data_for_root_FLRECO_TIME.txt` (contains number of events, mean value of flreconstruct time and std deviation)  
- `plot_flreco_flsimu_runtime.C` — produces `FLRECO_FLSIMU_runtime_300dpi.png` — graphs **flsimulate time vs events** & **flreconstruct time vs events**

---

### `scripts/` Folder

Contains all universal scripts which were used to process the data, **the names and description match with those provided above**.

---
### 📊 Supplementary Files (the most informative)

- `COMSUMED_RAM_TIME_approximation_coeff.pdf` — the table of approximation coefficients for RAM vs events & total runtime vs events for all studied processes  
- `Memory_Runtime_vs_Events.png` — the layered graph of RAM vs events & total runtime vs events for all studied processes
