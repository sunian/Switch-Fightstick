/*
Nintendo Switch Fightstick - Proof-of-Concept

Based on the LUFA library's Low-Level Joystick Demo
	(C) Dean Camera
Based on the HORI's Pokken Tournament Pro Pad design
	(C) HORI

This project implements a modified version of HORI's Pokken Tournament Pro Pad
USB descriptors to allow for the creation of custom controllers for the
Nintendo Switch. This also works to a limited degree on the PS3.

Since System Update v3.0.0, the Nintendo Switch recognizes the Pokken
Tournament Pro Pad as a Pro Controller. Physical design limitations prevent
the Pokken Controller from functioning at the same level as the Pro
Controller. However, by default most of the descriptors are there, with the
exception of Home and Capture. Descriptor modification allows us to unlock
these buttons for our use.
*/

/** \file
 *
 *  Main source file for the posts printer demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "Joystick.h"

// Main entry point.
int main(void) {
	// We'll start by performing hardware and peripheral setup.
	SetupHardware();
	// We'll then enable global interrupts for our use.
	GlobalInterruptEnable();
	// Once that's done, we'll enter an infinite loop.
	for (;;)
	{
		// We need to run our task to process and deliver data for our IN and OUT endpoints.
		HID_Task();
		// We also need to run the main USB management task.
		USB_USBTask();
	}
}

// Configures hardware and peripherals, such as the USB peripherals.
void SetupHardware(void) {
	// We need to disable watchdog if enabled by bootloader/fuses.
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	// We need to disable clock division before initializing the USB hardware.
	clock_prescale_set(clock_div_1);
	// We can then initialize our hardware and peripherals, including the USB stack.


	DDRD = 0x0; // read from pin D
	DDRD |= (1<<6); // output to LED on D6

	DDRC = 0x0; // read from pin C
	DDRF = 0x0; // read from pin F
	DDRB = 0x0; // read from pin B
	DDRE |= (1<<6); // connected to RESET pin on Arduino
	// The USB stack should be initialized last.
	USB_Init();
}

// Fired to indicate that the device is enumerating.
void EVENT_USB_Device_Connect(void) {
	// We can indicate that we're enumerating here (via status LEDs, sound, etc.).
}

// Fired to indicate that the device is no longer connected to a host.
void EVENT_USB_Device_Disconnect(void) {
	// We can indicate that our device is not ready (via status LEDs, sound, etc.).
}

// Fired when the host set the current configuration of the USB device after enumeration.
void EVENT_USB_Device_ConfigurationChanged(void) {
	bool ConfigSuccess = true;

	// We setup the HID report endpoints.
	ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_OUT_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);
	ConfigSuccess &= Endpoint_ConfigureEndpoint(JOYSTICK_IN_EPADDR, EP_TYPE_INTERRUPT, JOYSTICK_EPSIZE, 1);

	// We can read ConfigSuccess to indicate a success or failure at this point.
}

// Process control requests sent to the device from the USB host.
void EVENT_USB_Device_ControlRequest(void) {
	// We can handle two control requests: a GetReport and a SetReport.

	// Not used here, it looks like we don't receive control request from the Switch.
}

// Process and deliver data from IN and OUT endpoints.
void HID_Task(void) {
	// If the device isn't connected and properly configured, we can't do anything here.
	if (USB_DeviceState != DEVICE_STATE_Configured)
		return;

	// We'll start with the OUT endpoint.
	Endpoint_SelectEndpoint(JOYSTICK_OUT_EPADDR);
	// We'll check to see if we received something on the OUT endpoint.
	if (Endpoint_IsOUTReceived())
	{
		// If we did, and the packet has data, we'll react to it.
		if (Endpoint_IsReadWriteAllowed())
		{
			// We'll create a place to store our data received from the host.
			USB_JoystickReport_Output_t JoystickOutputData;
			// We'll then take in that data, setting it up in our storage.
			while(Endpoint_Read_Stream_LE(&JoystickOutputData, sizeof(JoystickOutputData), NULL) != ENDPOINT_RWSTREAM_NoError);
			// At this point, we can react to this data.

			// However, since we're not doing anything with this data, we abandon it.
		}
		// Regardless of whether we reacted to the data, we acknowledge an OUT packet on this endpoint.
		Endpoint_ClearOUT();
	}

	// We'll then move on to the IN endpoint.
	Endpoint_SelectEndpoint(JOYSTICK_IN_EPADDR);
	// We first check to see if the host is ready to accept data.
	if (Endpoint_IsINReady())
	{
		// We'll create an empty report.
		USB_JoystickReport_Input_t JoystickInputData;
		// We'll then populate this report with what we want to send to the host.
		GetNextReport(&JoystickInputData);
		// Once populated, we can output this data to the host. We do this by first writing the data to the control stream.
		while(Endpoint_Write_Stream_LE(&JoystickInputData, sizeof(JoystickInputData), NULL) != ENDPOINT_RWSTREAM_NoError);
		// We then send an IN packet on this endpoint.
		Endpoint_ClearIN();
	}
}

typedef enum {
	SYNC_CONTROLLER,
	PLAY
} State_t;
State_t state = SYNC_CONTROLLER;

int report_count = 0;

// Prepare the next report for the host.
void GetNextReport(USB_JoystickReport_Input_t* const ReportData) {

	// Prepare an empty report
	memset(ReportData, 0, sizeof(USB_JoystickReport_Input_t));
	ReportData->LX = STICK_CENTER;
	ReportData->LY = STICK_CENTER;
	ReportData->RX = STICK_CENTER;
	ReportData->RY = STICK_CENTER;
	ReportData->HAT = HAT_CENTER;

	// States and moves management
	switch (state)
	{
		case SYNC_CONTROLLER:
		    PORTD |= (1<<6); // LED on while syncing
			if (report_count > 100)
			{
				report_count = 0;
				state = PLAY;
			}
			else if (report_count == 25 || report_count == 50)
			{
				ReportData->Button |= SWITCH_L | SWITCH_R;
				PORTE &= ~(1<<6);
			}
			else if (report_count == 75 || report_count == 100)
			{
				ReportData->Button |= SWITCH_A;
				PORTE |= (1<<6);

			}
			report_count++;
			break;
		case PLAY:
		    // flash the LED once every 100 packets
		    if (report_count == 0) {
		        PORTD |= (1<<6); // set PD6
		    } else {
		        PORTD &= ~(1<<6); // clear PD6
		    }
            report_count++;
            report_count = report_count % 100;

            // assign first 8 buttons from pin C
            ReportData->Button = PINC;
            // rest of the buttons from pin B
            if (PINB & (1<<1)) {
                ReportData->Button |= SWITCH_PLUS;
            }
            if (PINB & (1<<2)) {
                ReportData->Button |= SWITCH_MINUS;
            }
            if (PINB & (1<<3)) {
                ReportData->Button |= SWITCH_HOME;
            }

            if (PIND & (1<<3)) {
                ReportData->HAT = HAT_TOP;
            }
            if (PIND & (1<<2)) {
                ReportData->HAT = HAT_BOTTOM;
            }
            if (PIND & (1<<1)) {
                ReportData->HAT = HAT_LEFT;
            }
            if (PIND & (1<<0)) {
                ReportData->HAT = HAT_RIGHT;
            }

            // analog sticks from pin F
            int stickMax = STICK_MAX;
            int stickMin = STICK_MIN;
            if (PINB & (1<<0)) {
                // only for left analog stick
                stickMax = STICK_CENTER + 64;
                stickMin = STICK_CENTER - 64;
            }

            if (PINF & (1<<7)) {
                ReportData->LY = stickMin;
            }
            if (PINF & (1<<6)) {
                ReportData->LY = stickMax;
            }
            if (PINF & (1<<5)) {
                ReportData->LX = stickMin;
            }
            if (PINF & (1<<4)) {
                ReportData->LX = stickMax;
            }
            if (PINF & (1<<3)) {
                ReportData->RY = STICK_MIN;
            }
            if (PINF & (1<<2)) {
                ReportData->RY = STICK_MAX;
            }
            if (PINF & (1<<1)) {
                ReportData->RX = STICK_MAX;
            }
            if (PINF & (1<<0)) {
                ReportData->RX = STICK_MIN;
            }

			return;
	}

}
