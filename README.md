# ABACUS

ABACUS stands for Automated Behavioral Approximate CircUit Synthesis. This tool enables the designer to
explore the approximated design space of arbitrary high-level Verilog input. ABACUS, in an automated
fashion, generates optimal approximate variants of an accurate hardware, using a recursive stochastic
evolutionary algorithm. For more details please visit our publications:

[1] K. Nepal, S. Hashemi, H. Tann, R. I. Bahar and S. Reda, "Automated High-Level Generation of
Low-Power Approximate Computing Circuits", in IEEE Transactions on Emerging Topics in Computing, 2016.

[2] K. Nepal, Y. Li, R. I. Bahar and S. Reda, " ABACUS: A Technique for Automated Behavioral Synthesis
of Approximate Computing Circuits" in Design, Automation and Test in Europe, 2014.


# Prerequisites

- Python Version 3.7 or newer

- Verilator for simulation given benchmarks. Other simulators can be also used, but simulation script needs to be modified. 

- Yosys for synthesis. Other synthesis tools can be also used, but synthesis script need to be modified, together with the synthesis parsing function synthesize_sesign in abacus.py

- Liberty technology library:


# Known issues
ABACUS uses ODIN II [3] tool to parse the input verilog codes to an abstract syntax tree (AST).
ODIN II does not support several Verilog syntax, which are added in ABACUS. Some syntax are 
still not supported. The list includes:

* none synthesizable Verilog code (display, monitor, etc)
* task, function syntax
* generate syntax
* signed output register syntax

[3] P. Jamieson, K. B. Kent, F. Gharibian and L. Shannon, "Odin II - An Open-Source Verilog HDL
Synthesis Tool for CAD Research," Field-Programmable Custom Computing Machines (FCCM), 2010 18th
IEEE Annual International Symposium on, Charlotte, NC, 2010, pp. 149-156.

# Compilation
- First compile libvpr_6 by going to trunk/libvpr_6 and type "make".
- Then go to trunk/ABACUS and type "make clean && make".

# Files in The Repository

Directories:

  - blockmatching, fft, fir, and perceptron: Contain the files used for their respective testbench
    as reported in publications [1] and [2].

  - trunk: Contains source codes, header files, binaries, and scripts used by ABACUS. 
    - ABACUS/                  : source codes, and all of the supporting files related to source codes.
    - libvpr_6/                : libvpr_6 is used by ABACUS
    - abacus.py                : main script to run ABACUS flow

Regular Files:

  - LICENSE: Contains the license information.
  - README.md: The README file. This file.

# Configuration Files

When creating a new benchmark, you will need to create a new synthesis (synthesis.script) and simulation script (sim.script) in the folder for the benchmark.

# Benchmarks

* Block Matching: Block matching is a technique commonly used in motion estimation and video compression
applications. Block matching partitions a given frame into non-overlapping rectangular blocks and tries to
find the block from the reference frame in a given search range that best matches the current block. The
measure of similarity between the blocks is computed by the sum of the absolute difference. For our design,
we perform full search block matching over a search window in a reference frame to determine the best match
for a block in a current frame. Our particular test case works on 16x16 block sizes from a 352x288 frame
sequence. The quality of a design is assessed using the Peak Signal-to-Noise Ratio (PSNR).

* FFT Implementation: An FFT circuit takes in an input signal and outputs the amplitude and phase spectra
of the signal’s frequency components. For our benchmark, we have implemented an 8-point radix-2 decimation
in time FFT with 16-bit sign-extended integer. We measure accuracy by computing the MSE of the approximate
circuit’s output amplitude spectrum.

* FIR Filter: A 25-tap FIR filter takes in an audio signal and convolves it with a low-pass filter coefficient
array, effectively creating a 1-D filtered output. The quality of an approximate version of this design is
assessed using Mean-Squared Error (MSE) on the amplitude of the audio signals generated. The MSE for the
original FIR filter circuit was computed with a 16-bit sign-extended fixed point coefficient and image input
compared against a floating point implementation done in software.

* Perceptron Classifier: The perceptron classifier is a commonly used application in machine learning. A 
perceptron takes an input data, denoted by vector x, and predicts the class (e.g., -1 or +1) of x by computing
sign(wx), where w is the weight vector and sign(.) is a function that outputs 1 if its argument is positive
and -1 otherwise. The perceptron essentially defines a hyperplane to separate the training data into two classes.
Perceptrons are also capable of classifying non-linearly separable points by mapping the input points to another
space where they are linearly  separable. Our perceptron test case uses a quadratic function to map the input
space. The input data set consisted of 1,000 randomly generated two-dimensional points from two classes.
Classification results were compared against the ground-truth and hence the total percentage change in
classification outputs was considered as the accuracy metric.

# How to Run

In general, to run ABACUS using a single process you need to change directory to trunk and run the
main script:

    cd ABACUS_ROOT/trunk
    python abacus.py [testbench_name] [TESTBENCH_FOLDER] min_accuracy_threshold(0-100)

Note: there may be error if other shell is used.


To run with arbitrary input verilog files (MODIFY THIS):

Create a folder in ABACUS root with the testbench name:

    cd ABACUS_ROOT
    mkdir random_verilog
    
Copy the sample config file from the trunk folder to the new folder and modify it as needed:

    cd random_verilog
    cp ../trunk/sample.config ./
    mv sample.config random.config
    vi random.config

Create the necessary folders for the ABACUS tool and put the required files as follows:

- Original/ -> Here you need: The Verilog source files.

- Population/ -> A placeholder. ABACUS tool will populate this folder with mutant files.

- SRC/ -> A placeholder. ABACUS tool will populate this folder with mutant source codes.

- simulation/ -> hold ground truth output, input files, and simulation files for each testcase.
		 Put each of the testcases in a seprate folder named as testcaseX. The 4 existing
		 testbenches can provide good examples.

- synthesis/ -> Provide the .tcl files required for dc compiler synthesis. Synthesis libraries
	      are also required.

The summary would be:

    mkdir Original
    cp DESIGN_SOURCE_FILES ./Original/
    mkdir Population
    mkdir SRC
    mkdir simulation
    mkdir simulation/testcase1/
    cp SIMULATION_FILES GROUND_TRUTH INPUT_FILES ./simulation/testcase1/
    # Repeat last two steps as needed for multiple testcases.
    mkdir synthesis/
    cp SYNTHESIS_FILES.tcl ./synthesis/
    cp LIBRARY_FILES ./synthesis/

If all is done correctly, now all you need is to run ABACUS.

    cd ABACUS_ROOT/trunk
    sh run_ABACUS_flow ../random_verilog/random.config 0

# Output Format
ABACUS uses genetics algorithm (NSGA II and NSGA II hybrid, see [2] for more detail) to perform mutations
on input designs. For each generation of mutation, the mutants which pass the accuracy, power saving
thresholds are saved in the Population folder.

The names of the mutant designs have the following format: [designName]_G[X]_S[Y]_F[Z].v
- [designName] is specific to each benchmark (i.e. fir_filter)
- G stands for Generation number
- S stands for Selection number (the parent of the mutant)
- F stands for File number (for different mutants)
- X, Y, Z are placeholders for numbers

Mutants with the same S number in the same generation share the same parent design. They will have 
different F numbers. In the first generation, there is only one parent (the original design), so each
mutant has S1. From the second generation onward, the NSGA II hybrid will use 4 parents:
1. Global Best: the fittest design from all generations (given name S1)
2. Local Best: Best from the previous generation (S2). If this is the same design as the global best, the S2 
			   will  be the second best from the previous generation.
3. Local Second Best: (S3) similar to S2 except it is the second best.
4. Local Third Best: (S4) similar to S2 except it is the third best.

Currently, S1 parents are allowed to produce 4 children; S2 has 3 chidren; S3 has 2 children, and S4 has 1
child.

In addition to the design files, each generation also produces a summary file called:
    FilesInfo_G[X].txt
where G[X] is for the generation number. The content of this summary file is in the following format:

    #File_Name  Mean_Accuracy(%)    Min_Accuracy(%) Area_Saving(%)  Power_Saving(%) Net_Slack(ns)   Additional_Savings(%)
    
    fir_filter_G1_S1_F1     98.243      98.1473             8.9540      18.7309     0        0   
    fir_filter_G1_S1_F2     89.4577     87.9527             19.8137     24.0762     0        0   
    fir_filter_G1_S1_F3     76.2976     73.5313             16.8679     21.2852     0        0   
    fir_filter_G1_S1_F4     79.9836     77.7669             11.1038     19.3790     0        0   
    fir_filter_G1_S1_F5     97.5239     97.1871             8.0360      12.4602     0        0   
    fir_filter_G1_S1_F6     85.3439     84.1843             12.6930     20.5033     0        0   
    fir_filter_G1_S1_F7     76.069      72.0779             24.8852     26.6994     0        0   
    fir_filter_G1_S1_F8     91.2023     90.3241             20.3369     22.3393     0        0   
    fir_filter_G1_S1_F9     72.1619     69.246              8.4362      16.7991     0        0   
    fir_filter_G1_S1_F10    90.9573     89.5009             22.1418     26.6342     0        0
    
    # The best fit approximate design of Generation 1 is fir_filter_G1_S1_F1

Here, the accuracy statics (mean, min) across test cases, area, power saving, net slack are reported for each mutant.
The additional_saving(%) report the power saving from resulted from Voltage-Frequency Scaling, which are included
in the total power saving (Power_Saving).


# Troubleshooting and Common Issues

- Make sure dc_shell, and Vsim or Verilator are working properly.

- If verilator (or Vsim) is not system-wide installed, make sure to set VERILATOR_ROOT to proper location.
You must then export VERILATOR_ROOT.

- Make sure you are in the trunk folder when
running the main script.

# License and Citation

ABACUS is released under the BSD 2-Clause license.(Refer to LICENSE file).

Please cite ABACUS in your publications if it helps your research:

  @article{nepal2016abacus,
    Author = {Nepal, Kumud and Hashemi, Soheil and Tann, Hokchhay and Bahar, R. Iris and Reda, Sherief},
    Journal = {IEEE Transactions on Emerging Topics in Computing},
    Title = {Automated High-Level Generation of Low-Power Approximate Computing Circuits},
    Year = {2016}
  }

Contact [hokchhay_tann at brown.edu], [soheil_hashemi at brown.edu], or [sherief_reda at brown.edu] for questions.
