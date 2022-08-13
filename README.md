# msi-ml
MSI-ML is a C utility designed to control the RGB backlighting on MSI's MysticLight RGB products.

This utility was created in Linux, by a Linux user, for other Linux users with MSI laptops. While there are some other programs that are able to do keyboard backlight control on MSI laptops (such as MSIKLM), they were designed with the SteelSeries hardware in mind.


## Requirements
While theoretically this would work under Windows, I haven't tried it since this seems to be mostly a Linux issue. Windows already has MSI's Mystic Light software - I would recommend using that first (although it's not great, imo) before attempting to use this.

Secondly, you should have a C compiler (such as GCC) and CMake installed. If you don't, use your distrobution's package manager to install, or build them from source.


## Installation
First, clone the repository. Then, make a build directory. Then, issue `cmake ..`, and then finally, `make`.

So far, I haven't tested doing an installation to anywhere, but if you copy the final result to `/usr/bin` (or some other directory in your PATH), it should work just fine.


## Usage
All of the directions can be found with `msi-ml -h` or `msi-ml --help`.


## Contributions
Many thanks to the creators of the hidapi project - this would have been damn near impossible without their library.

In addition, thanks to Jona Saffer for the creation of his python script, which was the basis for this program.
