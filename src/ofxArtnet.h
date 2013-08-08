//
//  ofxArtnet.h
//  artnet
//
//  Created by 宏行 堀 on 12/01/10.
//  Copyright 2012年 rhizomatiks. All rights reserved.
//

#ifndef artnet_ofxArtnet_h
#define artnet_ofxArtnet_h
#include <artnet.h>
#include "ofMain.h"

#define _TIMEOUT 1000


enum status_artnet { NOT_READY, 
                    NODES_FINDING,
                    NODES_FOUND
};


class ofxArtnet : public ofThread
{
public:
    static int nodes_found;
    static vector<string> foundedIps;
    string SHORT_NAME;
    string LONG_NAME;
    static status_artnet status;
    int find_timeout, sd;
    
    artnet_node node;
    
    ofxArtnet()
    {
        nodes_found = 0;
        status = NOT_READY;
        SHORT_NAME = "ArtNet Node";
        LONG_NAME = "libartnet setdmx example";
    }
    
    ~ofxArtnet()
    {
        //stopThread();
    }
    
    void setup(const char* interfaceIP, int port_addr = 0, int verbose = 0);
    void threadedFunction();
    void sendDmx( string targetIp, const unsigned char* data512, int size );
    void sendDmx( string targetIp, int targetSubnet, int targetUniverse, const unsigned char* data512, int size );
    
private:
    int static reply_handler(artnet_node n, void *pp, void *d) {
        artnet_node_list nl = artnet_get_nl(n);
        if (ofxArtnet::nodes_found == artnet_nl_get_length(nl)) {
            // this is not a new node, just a previously discovered one sending
            // another reply
            return 0;
        } else {
            artnet_node_entry ne;
            if ( ofxArtnet::nodes_found == 0 ) ne = artnet_nl_first(nl);
            else artnet_node_entry ne = artnet_nl_next(nl);
            
            char ips[50] = "";
            sprintf(ips,"%d.%d.%d.%d", ne->ip[0], ne->ip[1], ne->ip[2], ne->ip[3]);
            foundedIps.push_back(ips);
            
            ofxArtnet::nodes_found++;
            print_node_config(ne);
        }
        return 0;
    }
    
    void static print_node_config(artnet_node_entry ne) {
        char ips[50] = "";
        sprintf(ips,"%d.%d.%d.%d", ne->ip[0], ne->ip[1], ne->ip[2], ne->ip[3]);
        printf("--------- %d.%d.%d.%d -------------\n", ne->ip[0], ne->ip[1], ne->ip[2], ne->ip[3]);
        printf("Short Name:   %s\n", ne->shortname);
        printf("Long Name:    %s\n", ne->longname);
        printf("Node Report:  %s\n", ne->nodereport);
        
        printf("Subnet:       0x%hhx\n", ne->sub);
        printf("Numb Ports:   %d\n", ne->numbports);
        printf("Input Addrs:  0x%hhx, 0x%hhx, 0x%hhx, 0x%hhx\n", ne->swin[0], ne->swin[1], ne->swin[2], ne->swin[3] );
        printf("Output Addrs: 0x%hhx, 0x%hhx, 0x%hhx, 0x%hhx\n", ne->swout[0], ne->swout[1], ne->swout[2], ne->swout[3] );
        printf("----------------------------------\n");
    }
};

#endif
