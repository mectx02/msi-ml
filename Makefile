# A makefile script to build and install the MSI-ML utility.

main:
	# Check for the presence of the libhidapi library
	LIBRARY_AVAIL = $$(ldconfig -p | grep libhidapi | wc -l
	ifeq ($(LIBRARY_AVAIL), $(filter $(LIBRARY_AVAIL), 2 4))
		# Found it; go ahead and compile
		gcc -L/usr/lib main.c -lhidapi-libusb -lhidapi-hidraw -o msi-ml
	else
		# Remind the user that they need to install the HIDAPI library
		echo "hidapi library not found! You should go and install it."
		echo "APT   : \'sudo apt install libhidapi-hidraaw0'"
		echo "Pacman: \'sudo pacman -S hidapi'"
		echo "Build from source: \'https://github.com/libusb/hidapi'"

install:
	# Installs the final executable to the bin directory.
	cp ./msi-ml /usr/bin/.
	# Makes udev shut up and take it so the binary is executable 
	# without elevated permissions.
	cp 10-msi.rules /etc/udev/rules.d/.
