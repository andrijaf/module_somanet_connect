#pragma once

// The positions of first instances of each of the available service probes
#define MOTCTL_INSTANCE_BASE 10
#define ECATCNF_INSTANCE_BASE 40
#define CUSTOM_INSTANCE_BASE 42

// Total number of probes per service
#define MOTCTRL_PROBE_NUMBER 6
#define ECATCNF_PROBE_NUMBER 2
#define CUSTOM_PROBE_NUMBER 2

typedef enum { MOTCTRL, ECATCNF, CUSTOM } service_type;
typedef enum { MOTCTRL_TARGET_POSITION, MOTCTRL_ACTUAL_POSITION,
               MOTCTRL_TARGET_TORQUE, MOTCTRL_ACTUAL_TORQUE,
               MOTCTRL_TARGET_VELOCITY, MOTCTRL_ACTUAL_VELOCITY
} motctrl_probe_type;

typedef enum { ECATCNF_FIRST, ECATCNF_SECOND } ecatcnf_probe_type;
