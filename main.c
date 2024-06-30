#include <stdio.h> 		// for printf
#include <stdlib.h>		// standard c libraries
#include <stdint.h>		// integer comparison
#include <string.h>		// to make sure that characters work
#include <wchar.h>		// hidapi uses some wide characters
#include <getopt.h>		// for the user input switches


#include <hidapi/hidapi.h> 	// The actually important library



/**
 * Parses an input string to identify all the various colors and places them
 * into a color struct. To correctly use this function, the color must be 
 * address-referenced.
 * 
 * Inputs: 	in (char *), an input string
 * 			c (color *), a color struct to parse the input to
 * 
 * Outputs: void
 */
void color_splitter(char * in, unsigned char * c) {

	sscanf(in, "%2hhx%2hhx%2hhx", &c[0], &c[1], &c[2]);

}


/**
 * Creates an array of color structs based on the input
 * given to the program by the user when run.
 * 
 * Inputs: in (char *), an input string
 * 
 * Outputs: (color *), an array of color values
 */
int parse_color_list(char * in, unsigned char * c) {

	// First, determine the length of color array (which is done by just taking 
	// the length of the input string, adding 1, and dividing by 7)
	int colors = ((int)(strlen(in)) + 1) / 7;

	// TODO: we should do a quick error check on the size for memory reasons


	// Next thing to do is to parse the string
	int i = 0;
	int j = 0;
	int index = 0;
	char str[7];
	for (; i < strlen(in); i+=7) {

		strncpy(str, &in[i], 6);
		str[6] = '\0';

		char temp[3];
		memset(temp, 0, 3);	
		color_splitter(str, temp);

		// Math is hard, but it makes the code slightly more compact
		do {

			c[j] = temp[j % 3];
			j++;

		} while (j % 3 != 0);

	}

	return colors;

}


/**
 * Shows a help list so that the user knows what to do when they're lost
 */
void show_help() {

	printf("Usage: msi-ml [no-args] [settings] [color-mode {\"hex_color_string\"}] \n");
	printf("\n");
	printf("NO-ARGS OPTIONS:\n");
	printf("	--help      (-h): shows this help message\n");
	printf("	--off       (-o): turns off the keyboard backlight. No other args are necessary.\n");
	printf("\n");
	printf("SETTINGS OPTIONS:\n");
	printf("	--backlight (-B): adjusts the backlight brightness (value 0 - 10)\n");
	printf("	--speed     (-S): adjusts the speed between color shifts (value 0 - 2)\n");
	printf("\n");
	printf("COLOR-MODE OPTIONS:\n");
	printf("	--static    (-c): sets the keyboard backlight to a static color\n");
	printf("	--switching (-s): cycles through the given color list\n");
	printf("	--breathing (-b): like switching, but with a break between colors\n");
	printf("\n");
	printf("Colors must be prefaced with quotes (such as 'color_1 color_2') if multiple colors\n");
	printf("are provided, otherwise the program gets confused.\n");

}


int main(int argc, char ** argv) {

	// Set some defining variables first (or, at the very least, reserve some space)
	unsigned char kb_c[40];
	memset(kb_c, 0, 40);
	int brightness = 10;
	int speed = 0;
	int colors = 0;
	unsigned char mode;

	// First, before we do anything, we need to make sure that we can handle any options
	// that the user might want to be able to handle.
	int opts = 1;

	// Don't worry - we'll have to break out of this loop eventually to get to the rest 
	// of the program
	while (opts != -1) {

		// Establish an options list
		static struct option long_options[] = 
		{
			{"help", 				no_argument, 		0,	'h'},
			{"off", 				no_argument, 		0,	'o'},
			{"brightness", 			required_argument, 	0, 	'B'},
			{"speed",	 			required_argument,	0, 	'S'},
			{"static",		 		required_argument, 	0,	'c'},
			{"breathing",			required_argument, 	0, 	'b'},
			{"switching",			required_argument,	0,	's'}
		};

		// Apparently, getopt_long stores its option index here
		int opt_index = 0;
		opts = getopt_long(argc, argv, "hoB:S:c:b:s:", long_options, &opt_index);

		if (argc == 1) show_help();

		// Now we need to parse through every available option in the list
		if (opts == -1) break;

		switch (opts) {

			case 'h':
				show_help();
				abort();

			case 'o':
				kb_c[0] = 0x00;
				kb_c[1] = 0x00;
				kb_c[2] = 0x00;
				brightness = 0;
				speed = 0;
				mode = 0x01;
				colors = 0;
				opts = -1;
				break;

			case 'B':
				sscanf(optarg, "%d", &brightness);
				break;

			case 'S':
				sscanf(optarg, "%d", &speed);
				break;

			case 'c':
				printf("Color: %s\n", optarg);
				mode = 0x01;
				colors = 1;
				color_splitter(optarg, kb_c);
				printf("%2hhx; %2hhx; %2hhx\n", kb_c[0], kb_c[1], kb_c[2]);
				break;

			case 'b':
				printf("Color string: %s\n", optarg);
				mode = 0x02;
				// TODO: fix this error with kb_c since the struct got removed
				// This is probably the dumbest hack because of the return type,
				// but I think it'll work.
				colors = parse_color_list(optarg, kb_c);
				break;


			case 's':
				printf("Color string: %s\n", optarg);
				mode = 0x05;
				colors = parse_color_list(optarg, kb_c);
				break;


			default:
				show_help();
				abort();

		}

	}



	// Initialize the hid api library
	hid_init();

	// First, we need to find where the MSI keyboard is located, since different
	// models can apparently have the keyboard located at a different VID:PID combo
	// We do this by essentially iterating over every USB device present in the system
	// (Thankfully, there usually aren't that many to go through, so it should go
	// fairly quickly)

	struct hid_device_info * devices, * cur_device;
	devices = hid_enumerate(0x0, 0x0);
	cur_device = devices;

	while (cur_device) {

		if (wcscmp(L"MSI", cur_device->manufacturer_string) == 0) break;

		cur_device = cur_device->next;

	}


	// If we can't find what we are looking for, then just exit with an error message
	if (wcscmp(L"MSI", cur_device->manufacturer_string) != 0) {
		printf("MSI keyboard with MysticLight capabilities not found\n");
		return 1;
	}


	//
	// The actually fun part - getting all the data to the keyboard
	//


	// First, establish a 64-byte array
	unsigned char data[64];
	memset(data, 0x00, sizeof(data));
	data[0] = 0x02;
	data[1] = 0x00;

	// Create a handle for our newly found devices; after this step, we can free our
	// linear search list variable
	hid_device * handle = hid_open(cur_device->vendor_id, 
								   cur_device->product_id, 
								   cur_device->serial_number);
	hid_free_enumeration(devices);


	/**
	 * * Technical information * (for all the nerds)
	 * 
	 * Byte 0-1: USB HID report number
	 * Byte 2: mode (steady, breathing, switching)
	 * Byte 3: speed (value from 0 - 2)
	 * Byte 4: brightness (value from 1 - 10)
	 * Byte 5: number of color combinations 
	 * 		(color combos are presented as RR  GG  BB, 
	 * 		 in tuplets of 3 bytes.)
	 */

	data[0] = 0x02;
	data[2] = mode;
	data[3] = speed;
	data[4] = brightness;
	data[5] = colors;

	// At this point, we now need to iterate over all the colors and add them to the data array
	int i = 6;
	for (; i < 6 + (colors * 3); i++)  {
		data[i] = kb_c[i - 6];
	}


	// Now that the data structure is set up, we can send all of the data to the keyboard!
	// MAKE PRETTY LIGHTS I SAY! MAKE THEM! I COMMAND YOU!
	hid_send_feature_report(handle, data, sizeof(data));


	// Do the proper thing and make sure that nothing is taking up
	// precious space in memory
	hid_close(handle);
	hid_exit();

	// If we get here, then the program ended successfully! Congrats - you should have a 
	// colorful & controllable keyboard now! (Unless you have it set to black like your soul)
	return 0;

}
