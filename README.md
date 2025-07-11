# Simulation_Performance
Quantify computation requirements (time, memory) for simulation

The original task: https://github.com/Shoram444/SN_simulation_setup_codes/issues/1

**At this moment (July 2025) there are available results for such processes:**

* Se-82 0νββ (foil_bulk)
* Se-82 2νββ (foil_bulk)
* Tl-208 (foil_bulk SOURCE BACKGROUND)
* Bi-214 (foil_bulk SOURCE BACKGROUND)
  
**As soon as possible to this study will be included some other processes**

# Files Navigation

In each **folder with name of the isotope (Se82, Tl208m Bi214 ...)** there are **7** folders with names written in format **<NUMBER>_eve** (e.g. 10pown_eve [n=1...4], n_10pown_eve [n=2,3] ...). 

**Each of such folders contains:**

- files
  
  Setup_0nu_Se82.conf ---> simulation configuration setup file
  
  Simu_0nu_Se82.conf  ---> simulation configuration file
  
  manager.sh          ---> **mian script** which provides operating with sending jobs to the cluster (via **send_0nu.sh**) and proceeding it (**analyze_memory.C** & **analyze_runtime.C**)
  
  send_0nu.sh         ---> SLURM sending script (loads Falaise, creates simulation & reconstucts it, proceeds time)
  
- folder with the name **jobs_done** 
  which **contains the most important information** about the process for the corresponding number of events. In each folder **jobs_done** is located the set of files with the following names      and meanings:

   **(STANDART)**
  
  memory_raw.txt          ---> file with the list of values of used memory for each job with corresponding ID (**seff id | grep "Memory Utilized"**) proceeded by **manages.sh** & **send_0nu.sh**

  runtime_raw.txt         ---> file with the list of values of runtime for each job with corresponding ID proceeded by **manages.sh** & **send_0nu.sh**  

  analyze_memory.C        ---> analyzes memory from memory_raw.txt (gives 3 output files **memory_fit_results.txt**, **memory_hist.root**, **memory_hist.png**)  

  analyze_runtime.C       ---> analyzes memory from runtime_raw.txt (gives 3 output files **runtime_fit_results.txt**, **runtime_hist.root**, **runtime_hist.png**)  

  memory_fit_results.txt  ---> result of processing analyze_memory.C - gives the number of events, the mean value and the std. deviation  

  runtime_fit_results.txt ---> result of processing analyze_runtime.C - gives the number of events, the mean value and the std. deviation  

  runtime_hist.root       ---> result of processing analyze_runtime.C - the ROOT histogram (viewed only via TBrowser)  

  runtime_hist.png        ---> the viewable picture of the histogram  

  memory_hist.root        ---> result of processing analyze_memory.C - the ROOT histogram (viewed only via TBrowser)  

  memory_hist.png         ---> the viewable picture of the histogram  

    **(OPTIONAL)**
  
  collect_mem.sh         --->  produces runtime_raw.txt manually extracting ram for each job in the arbitary selected range of IDs for wich the ..._raw.txt file wasn't created

  collect_runtime.sh     ---> produces memory_raw.txt manually extracting runtime for each job in the arbitary selected range of IDs for wich the ..._raw.txt file wasn't created

**!** These files are **not included** in every **jobs_done** folder because they **are used only in emergency cases** when the jobs were not proceeded properly and **runtime_raw.txt or memory_raw.txt weren't produced automatically by manager.sh** and should be added manually. 

















