# BSN

**B**ody **S**ensor **N**etwork is a software system prototype of a sensor network for monitoring human body vital signs with self-adaptability capabilities.

### Folder content

./docs - contains the goal models and formulae generated
./experiment - contains the details, data and r algorithms used to derive the graphs in the paper
./lepton - contains the library used for formulae parsing
./libbsn - contains the classes the support the bsn
./odv - contains the modules used in bsn execution and configuration files

### Prerequesites

You must have OpenDaVINCI version v4.16.0 installed to run the prototype. The * framework * can be found in the repository: https://github.com/se-research/OpenDaVINCI, where there are various operating systems. Link to the precompiled package: https://github.com/se-research/OpenDaVINCI/blob/master/docs/installation.pre-compiled.rst

### Compiling

To compile, one must execute the compile.sh file:

```
sudo bash compile.sh
```

The installation shall be automatic.

### Execution

A standard execution can be performed by calling, used for the experiments:

```
sudo bash launch.sh
```

If one wants to execute in a different set, it is straightforward to execute separately the commandlines place in launch.sh. Other instructions about configuring the OpenDaVINCI commandline execution instructions can be found in their GitHub.

### Configurations

* Operational System - Linux Ubuntu 16.04
* Compiler -  gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.5)

## Authors

* **Ricardo D. Caldas** - https://github.com/rdinizcal
* **Gabriel Levi** - https://github.com/gabrielevi10
* **Léo Moraes** - https://github.com/leooleo 
* **Genaína N. Rodrigues** _(adviser)_ - genaina@unb.br
