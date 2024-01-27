#include "xnet_tiny.h"
static xnet_packet_t tx_packet,rx_packet;

xnet_packet_t* xnet_alloc_for_send(uint16_t data_size){
    tx_packet.data=tx_packet.payload+XNET_CFG_PACKET_MAX_SIZE-data_size;
    tx_packet.size=data_size;
    return &tx_packet;
}

xnet_packet_t* xnet_alloc_for_read(uint16_t data_size){
    rx_packet.data=rx_packet.payload;
    rx_packet.size=data_size;
    return & rx_packet;
}