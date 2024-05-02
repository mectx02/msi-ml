#!/sbin/openrc-run

# An init script to disable the keyboard backlight on MSI Mystic-Light capable 
# laptops. This script is only meant to be used on openrc-style init systems. 
# If you're running systemd, install the .service script instead.

command=/usr/bin/msi-ml
name="MSI MysticLight keyboard backlight daemon"
description="A daemon to turn off the keyboard backlight on supported MSI laptops"

# Because this script only requires that an executable be started, we can ignore
# the dependencies() and stop() commands.

start() {
	# Because we are disabling the backlight, we simply call 'msi-ml -o'. 
	# Change as desired if you want a different keyboard backlight color.
	${command} -o
}
