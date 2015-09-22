#pragma once

interface somanet_connect_plugin_interface {
    unsigned char get_type();
    void get_command(unsigned char p[n], unsigned n);
};
