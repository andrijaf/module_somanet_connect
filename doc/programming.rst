
Programming Guide
=================

Sending data via xSCOPE to the web application (motctrl, ecatcnf)
-----------------------------------------------------------------

Step 1: Include the required modules, headers and files
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Make sure you Makefile contains the following module

::

    USED_MODULES = module_somanet_connect

Make sure you include this file in the file where you use the SOMANETconnect functions to send data

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