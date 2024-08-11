# A makefile script to build and install the MSI-ML utility.
LIBRARY_AVAIL = $(shell ldconfig -p | grep libhidapi | wc -l)

main:
# Check for the presence of the libhidapi library
ifeq ($(LIBRARY_AVAIL), $(filter $(LIBRARY_AVAIL), 2 4))
	gcc -L/usr/lib main.c -lhidapi-libusb -lhidapi-hidraw -o msi-ml

else
	# Remind the user that they need to install the HIDAPI library
	@echo ""
	@echo "'
	@echo "hidapi library not found! You should go and install it."
	@echo "'
	@echo "APT   : sudo apt install libhidapi-dev"
	@echo "Pacman: sudo pacman -S hidapi'"
	@echo ""
	@echo "Build from source: https://github.com/libusb/hidapi"
endif

install: msi-ml 10-msi.rules init_scripts/keyboard-backlight-disable.sh
# Installs the final executable to the bin directory.
	cp ./msi-ml /usr/bin/.
# Makes udev shut up and take it so the binary is executable 
# without elevated permissions.
	cp 10-msi.rules /etc/udev/rules.d/.
# Installs a daemon to turn off the keyboard backlight on boot
	cp ./init_scripts/keyboard-backlight-disable.sh /etc/init.d/keyboard-backlight-disable
	chmod +x /etc/init.d/keyboard-backlight-disable
	rc-update add keyboard-backlight-disable default 

