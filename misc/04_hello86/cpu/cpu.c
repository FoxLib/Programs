


// Следующий байт CS:IP
int fetch() {

    int b = memory[ segment[ CS ]*16 + ip ];
    ip = (ip + 1) & 0xffff;

    return b;
}

/* Решить ModRM */
void resolve_modrm() {

    int modrm = fetch();
    int defseg;
    int addr;
    int bm;
    int loc_bx = reg[ BX ];
    int loc_bp = reg[ BP ];
    int loc_si = reg[ SI ];
    int loc_di = reg[ DI ];
    int loc_ds = segment[ DS ];
    int loc_ss = segment[ SS ];

    switch (modrm & 0x07) {

        case 0: defseg = loc_ds; addr = loc_bx + loc_si; break;
        case 1: defseg = loc_ds; addr = loc_bx + loc_di; break;
        case 2: defseg = loc_ss; addr = loc_bp + loc_si; break;
        case 3: defseg = loc_ss; addr = loc_bp + loc_di; break;
        case 4: defseg = loc_ds; addr = loc_si; break;
        case 5: defseg = loc_ds; addr = loc_di; break;
        case 6: defseg = loc_ss; addr = loc_bp; break;
        case 7: defseg = loc_ds; addr = loc_bx; break;
    }

    switch (modrm & 0xc0) {

        case 0x00: // none or disp16

            if ((modrm & 0x07) == 0x06) {

                defseg = loc_ds;
                addr  = fetch();
                addr += 256*fetch();
            }

            break;

        case 0x40: // disp8

            bm = fetch();
            addr += ((bm < 128) ? bm : bm - 256);
            break;


        case 0x80: // disp16

            bm  = fetch();
            bm += fetch() * 256;

            addr += bm;
            break;
    }

    addr &= 0xffff;

    // segment override
    
    effective_addr = addr;
    effective_segment = defseg;
}

// ---------------------------------------------------------------------

void step() {
    
    int opcode;
    

    opcode = fetch();

    
    
    resolve_modrm();
    
}
