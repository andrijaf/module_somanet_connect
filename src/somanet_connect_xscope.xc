#include "somanet_connect_xscope.h"
#include <print.h>
#include <xscope.h>
#include <stdlib.h>

void somanet_connect_xscope_int(service_type service_v, unsigned int probe, unsigned int service_instance, unsigned int data) {
    int probe_number;
    switch (service_v) {
        case MOTCTRL:
            probe_number = MOTCTL_INSTANCE_BASE + MOTCTRL_PROBE_NUMBER * service_instance + probe;
            break;
        case ECATCNF:
            probe_number = ECATCNF_INSTANCE_BASE + ECATCNF_PROBE_NUMBER * service_instance + probe;
            break;
        case CUSTOM:
            probe_number = CUSTOM_INSTANCE_BASE + CUSTOM_PROBE_NUMBER * service_instance + probe;
            break;
        default:
            printstrln("ERROR: Unknown type");
            exit(1);
            return; // Used just to avoid a warning saying that default is not closed with return or break
    }
    if (probe_number < 0 || probe_number > 255) {
        printstrln("ERROR: Invalid probe number");
        exit(1);
    }
    xscope_int(probe_number, data);
}
