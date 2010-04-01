// Ports library demo, this is the receiver, see also the blink_xmit example
// 2009-02-14 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: blink_recv.pde 4727 2009-12-08 21:39:49Z jcw $

#include "Ports.h"
#include "RF12.h"

// the state received from the transmitter
RemoteNode::Data state;

// local ports
Port one (1), two (2), three (3), four (4);

void setup() {
    rf12_initialize('A', RF12_868MHZ);
}

void loop() {
    if (rf12_recvDone() && rf12_crc == 0 && rf12_len == sizeof state) {
        memcpy(&state, (void*) (rf12_buf + 2), sizeof state);
        
        if (state.flags & 0x80) {
            one.mode((state.modes & 0x01) != 0);
            two.mode((state.modes & 0x02) != 0);
            three.mode((state.modes & 0x04) != 0);
            four.mode((state.modes & 0x08) != 0);
        }
        
        if (state.flags & 0x40) {
            one.digiWrite(state.digiIO & 0x01);
            four.digiWrite(state.digiIO & 0x08);
        }

        if (state.flags & 0x10)
            two.anaWrite(state.anaOut[0]);

        if (state.flags & 0x20)
            three.anaWrite(state.anaOut[1]);
        
        // not used right now
        //
        // if (state.flags & 0x01)
        //     state.anaIn[0] = one.anaRead();
        // if (state.flags & 0x02)
        //     state.anaIn[1] = two.anaRead();
        // if (state.flags & 0x04)
        //     state.anaIn[2] = three.anaRead();
        // if (state.flags & 0x08)
        //     state.anaIn[3] = four.anaRead();

        state.flags = 0;
        
        if ((rf12_hdr & ~RF12_HDR_MASK) == RF12_HDR_ACK) {
            uint8_t h = RF12_HDR_CTL | RF12_HDR_DST | rf12_hdr & RF12_HDR_MASK;
            rf12_sendStart(h, &state, sizeof state);
        }
    }
}
