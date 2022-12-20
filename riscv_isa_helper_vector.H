typedef union {
    uint8_t b[16];
    uint16_t h[8];
    uint32_t w[4];
    void set8( uint8_t i, uint32_t v ) { this->b[i] = v; }
    void set16( uint8_t i, uint32_t v ) { this->h[i] = v; }
    void set32( uint8_t i, uint32_t v ) { this->w[i] = v; }
    uint32_t get8( uint8_t i ) { return this->b[i]; }
    uint32_t get16( uint8_t i ) { return this->h[i]; }
    uint32_t get32( uint8_t i ) { return this->w[i]; }
} v128;