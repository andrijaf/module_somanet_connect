
Programming Guide
=================

Sending data via xSCOPE to the web application (motctrl, ecatcnf)
-----------------------------------------------------------------

Step 1: Include the required modules and headers in the Makefile
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Make sure that the Makefile contains the following module:

::

    USED_MODULES = module_somanet_connect

Make sure that the following file is included inside the file where you use the SOMANETconnect functions to send data:

::

    #include <somanet_connect_xscope.h>
    
The default xSCOPE probes are defined inside the "config.xscope" file inside this module.


Step 2: Sending data
^^^^^^^^^^^^^^^^^^^^
To send data to a web application use the somanet_connect_xscope_xxx function. The first parameter is the service type, the second is the probe number, the third is the service instance and the last is the data that is to be sent.

::

    int main(void)
    {
        ...
		somanet_connect_xscope_int(MOTCTRL, MOTCTRL_TARGET_VELOCITY, instance, target_velocity);
        ...
    }


Using the SOMANETconnect server, plugins and services
-----------------------------------------------------

 The SOMANETconnect server is used to receive data from the SOMANETconnect application and to convey them to proper plugins. Plugins are used to control services (whether they are default or custom ones). One plugin can control any number of services of the same type. The SOMANETconnect server and its plugins are connected using an array of interfaces. A certain type of a plugin is connected to its service instances through another array of interfaces.
 
TODO: Describe using default plugins and services
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Creating custom plugins and services
------------------------------------

Step 1: Include the required modules, headers and files
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Make sure that the Makefile contains the following module:

::

    USED_MODULES = module_somanet_connect

Make sure that the following file is included inside the file where you use the SOMANETconnect functions to send data:

::

    #include <somanet_connect_xscope.h>
    
To use custom xSCOPE probes, copy the "config.xscope" file from the module_somanet_connect root directory to the application's root directory and replace all of the default probe definitions ONLY with the definitions of custom probes:

::

	<xSCOPEconfig ioMode="timed" initTracing="true" enabled="true">
	  <!-- Custom probes -->
	  <Probe name="CUSTOM_0_first" type="CONTINUOUS" datatype="INT" units="Value" enabled="true"/>
	  <Probe name="CUSTOM_0_second" type="CONTINUOUS" datatype="INT" units="Value" enabled="true"/>
	</xSCOPEconfig>

Step 2: Create plugin and service headers
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Create the header file for the custom service (inside the application's src directory) which contains the prototype function of that service and define the interface that contains the accessable functions of that service:

::

    #pragma once

	interface custom_service_interface {
	    void start();
	    void stop();
	};
	
	void custom_service(unsigned int instance_number, unsigned int frequency,
			server interface custom_service_interface csi);

Create the header file for the custom plugin inside the application's src directory:

::

    #pragma once

	#include <somanet_connect_plugin_interface.h>
	#include "custom_service.h"
	
	#define CUSTOM_PLUGIN_TYPE 'c'
	#define CUSTOM_PLUGIN_START 's'
	#define CUSTOM_PLUGIN_STOP 'p'
	
	[[combinable]]
	void custom_plugin(server interface somanet_connect_plugin_interface scpi,
			client interface custom_service_interface csi[n], unsigned n);

Step 3: Create plugin and service source files
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
The SOMANETconnect server receives the commands and data from the SOMANETconnect application as an array of bytes in which the first byte determines the type of the service used and sends the rest of the command to the right plugin. The plugin determines the instance number of the service by the first byte of the remaining byte array and the command that is to be issued to the service by the second byte of the remaining byte array. The rest of the command can contain any other instructions or parameters.

Create the source file for the custom service:

::

	#include "custom_service.h"
	#include <print.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <xs1.h>
	
	#ifdef SOMANET_CONNECT
	#include <somanet_connect_xscope.h>
	#endif
	
	void custom_service(unsigned int instance_number, unsigned int frequency,
			server interface custom_service_interface csi) {
	    timer t;
	    uint64_t time;
	    const uint64_t period = (1000 * 250000)/frequency; // 250000 timer ticks = 1ms (ReferenceFrequency="250MHz")
	
	    int run = 0;
	
	    int value = 0;
	    int max_value = 1000;
	
	    int probe_0_int_value = 0;
	    int probe_1_int_value = 0;
	
	    t :> time;
	    srand(time);
	    while(1) {
	        select {
	            case csi.start(): {
	                run = 1;
	                printf("Custom service started successfully\n");
	                break;
	            }
	
	            case csi.stop(): {
	                run = 0;
	                printf("Custom service stopped successfully\n");
	                break;
	            }
	
	            case t when timerafter(time) :> void: {
	                if (run) {
	                    probe_0_int_value = value;
	                    probe_1_int_value = 1000 - value;
	
	#ifdef SOMANET_CONNECT
	                    somanet_connect_xscope_int(CUSTOM, 0, instance_number, probe_0_int_value);
	                    somanet_connect_xscope_int(CUSTOM, 1, instance_number, probe_1_int_value);
	#endif
	
	                    value++;
	
	                    if (value == max_value) {
	                        value = rand() % 300;
	                        max_value = rand() % 300 + 500;
	                    }
	                }
	                time += period;
	
	                break;
	            }
	        }
	    }
	}
	
Create the source file for the custom plugin:

::

	#include "custom_plugin.h"
	#include <print.h>
	#include <stdint.h>
	#include <xs1.h>
	
	[[combinable]]
	void custom_plugin(server interface somanet_connect_plugin_interface scpi,
			client interface custom_service_interface csi[n], unsigned n) {
	    unsigned char type = CUSTOM_PLUGIN_TYPE;
	
	    while(1) {
	        select {
	            case scpi.get_type() -> unsigned char type_value: {
	                type_value = type;
	                break;
	            }
	
	            case scpi.get_command(unsigned char command[n], unsigned n): {
	                unsigned int service_instance_number = command[0];
	                switch (command[1]) {
	                    case CUSTOM_PLUGIN_START: {
	                        csi[service_instance_number].start();
	                        break;
	                    }
	                    case CUSTOM_PLUGIN_STOP: {
	                        csi[service_instance_number].stop();
	                        break;
	                    }
	                    default: {
	                        printstrln("Unknown command!");
	                        break;
	                    }
	                }
	                break;
	            }
	        }
	    }
	}

Step 4: Setup the main.xc file
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Make sure that the necessary files are included:

::

	#include <xscope.h>
	#include <somanet_connect_server.h>
	#include "custom_service.h"
	#include "custom_plugin.h"

Make sure that the following are defined:

::

	#define NO_OF_PLUGINS 1
	#define NO_OF_CUSTOM_SERVICES 2
	
Make sure that the necessary xSCOPE channel is defined and initialized and that the interface arrays are defined. The SOMANETconnect server function and the plugin function should be called on the same tile as parallel combinable functions. Make sure to run the wanted service functions also.

::

	int main(void) {
	    chan c_host_data;
	    interface somanet_connect_plugin_interface scpi[NO_OF_PLUGINS];
	    interface custom_service_interface csi[NO_OF_CUSTOM_SERVICES];
	
	    par
	    {
	        xscope_host_data(c_host_data);
	
	        on tile[0]:
	        {
	            [[combine]]
	            par
	            {
	                somanet_connect_server(c_host_data, scpi, NO_OF_PLUGINS);
	                custom_plugin(scpi[0], csi, NO_OF_CUSTOM_SERVICES);
	            }
	        }
	
	        on tile[3]:
	        {
	            par {
	                custom_service(0, 1000, csi[0]);
	                custom_service(1, 1000, csi[1]);
	            }
	        }
	    }
	
	    return 0;
	}