/*
 * Author: Jessica Gomez <jessica.gomez.hernandez@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "mraa.hpp"

#include <iostream>
#include <unistd.h>
extern "C" {
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
}

#include "Intel_Edison_BT_SPP.hpp"
using namespace std;
/*
 * On board LED blink C++ example
 *
 * Demonstrate how to blink the on board LED, writing a digital value to an
 * output pin using the MRAA library.
 * No external hardware is needed.
 *
 * - digital out: on board LED
 *
 * Additional linker flags: none
 */
Intel_Edison_BT_SPP spp = Intel_Edison_BT_SPP();

int main()
{
	// select onboard LED pin based on the platform type
	// create a GPIO object from MRAA using it
	mraa::Gpio* d_pin = new mraa::Gpio(13, true, false);


	// set the pin as outputroot
	d_pin->dir(mraa::DIR_OUT);

	// loop forever toggling the on board LED every second
	//cout << "1" << endl;
	spp.open();		// Open BT SPP

	for (;;) {
		//cout << "2" << endl;
		//sleep(1);
		ssize_t size = spp.read();
		cout << size << endl;
		if (size > 0 && size < 32)
		{
			cout << "2" << endl;
			char * buf = spp.getBuf();
			cout << buf[0] << ", " << buf[1] << ", "  << buf[2] << endl;
			if (buf[0] == 'o' && buf[1] == 'n')
			{
				d_pin->write(1);
			}
			if (buf[0] == 'o' && buf[1] == 'f' && buf[2] == 'f')
				d_pin->write(0);
		}
	}

	return MRAA_SUCCESS;
}
