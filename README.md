# msi-ml
MSI-ML is a C utility designed to control the RGB backlighting on MSI's MysticLight RGB products.

This utility was created in Linux, by a Linux user, for other Linux users with MSI laptops. While there are some other programs that are able to do keyboard backlight control on MSI laptops (such as MSIKLM), they were designed with the SteelSeries hardware in mind.


## Requirements
This is Linux software, developed by a Linux user, for other Linux users with MysticLight-capable keyboards. If you are wanting to use this under Windows, 
sorry; MSI provdes a first-party utility. You should go use that.

In terms of installation, you should have a C compiler (such as GCC) and the Hidapi library installed from your package manager (or built from source - you don't have to install it from your package manager.). 


## Installation
I've recently adapted this to allow for multiple init systems. The code *should* be exacly the same between each branch, with only the init scripts dfiffering between them.
To install for your system (assuming that you're running a systemd init system), just call `make`. Or, if you care to specify, `make systemd`. For OpenRC systems, call `make openrc`. 
Make should take care of the rest for you, though it will ask for your password to install all of the required files.


## Usage
All of the directions can be found with `msi-ml -h` or `msi-ml --help`.


## Contributions
Many thanks to the creators of the hidapi project - this would have been damn near impossible without their library.

In addition, thanks to Jona Saffer for the creation of his python script, which was the basis for this program.
