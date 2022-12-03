
main:
	# Check for the presence of the libhidapi library
	LIBRARY_AVAIL = $$(ldconfig -p | grep libhidapi | wc -l
	ifeq ($(LIBRARY_AVAIL), $(filter $(LIBRARY_AVAIL), 2 4))
		# Found it; go ahead and compile
		gcc -L/usr/lib main.c -lhidapi-libusb -lhidapi-hidraw -o msi-ml
	else
		echo "hidapi library not found! You should go and install it."
		echo "APT   : \'sudo apt install libhidapi-hidraaw0'
		echo "Pacman: \'sudo pacman -S hidapi'
	

