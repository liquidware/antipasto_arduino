// Ports library definitions
// 2009-02-13 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: Ports.h 4888 2010-03-23 16:28:07Z jcw $

#ifndef Ports_h
#define Ports_h

#include <WProgram.h>
#include <stdint.h>

class Port {
protected:
    uint8_t portNum;

    inline uint8_t digiPin() const
        { return portNum ? portNum + 3 : 18; }
    inline uint8_t digiPin2() const
        { return portNum ? portNum + 13 : 19; }
    inline uint8_t anaPin() const
        { return portNum - 1; }
public:
    inline Port (uint8_t num) : portNum (num) {}

    // DIO pin
    inline void mode(uint8_t value) const
        { pinMode(digiPin(), value); }
    inline uint8_t digiRead() const
        { return digitalRead(digiPin()); }
    inline void digiWrite(uint8_t value) const
        { return digitalWrite(digiPin(), value); }
    inline void anaWrite(uint8_t val) const
        { analogWrite(digiPin(), val); }
    inline uint32_t pulse(uint8_t state, uint32_t timeout =1000000L) const
        { return pulseIn(digiPin(), state, timeout); }
    
    // AIO pin
    inline void mode2(uint8_t value) const
        { pinMode(digiPin2(), value); }
    inline uint16_t anaRead() const
        { return analogRead(anaPin()); }        
    inline uint8_t digiRead2() const
        { return digitalRead(digiPin2()); }
    inline void digiWrite2(uint8_t value) const
        { return digitalWrite(digiPin2(), value); }
    inline uint32_t pulse2(uint8_t state, uint32_t timeout =1000000L) const
        { return pulseIn(digiPin2(), state, timeout); }
        
    // IRQ pin (INT1, shared across all ports)
    static void mode3(uint8_t value)
        { pinMode(3, value); }
    static uint8_t digiRead3()
        { return digitalRead(3); }
    static void digiWrite3(uint8_t value)
        { return digitalWrite(3, value); }
        
    // both pins: data on DIO, clock on AIO
    inline void shift(uint8_t bitOrder, uint8_t value) const
        { shiftOut(digiPin(), digiPin2(), bitOrder, value); }
    uint16_t shiftRead(uint8_t bitOrder, uint8_t count =8) const;
    void shiftWrite(uint8_t bitOrder, uint16_t value, uint8_t count =8) const;
};

class RemoteNode {
public: 
    typedef struct {
        uint8_t flags, modes, digiIO, anaOut[2];
        uint16_t anaIn[4]; // only bits 0..11 used
    } Data;

    RemoteNode (char id, uint8_t band, uint8_t group =0);
    
    void poll(uint16_t msecs);

    friend class RemoteHandler;
    friend class RemotePort;
private:
    uint8_t nid;
    uint32_t lastPoll;
    Data data;
};

class RemoteHandler {
public:
    static void setup(uint8_t id, uint8_t band, uint8_t group =0);
    static uint8_t poll(RemoteNode& node, uint8_t send);
};

class RemotePort : protected Port {
    RemoteNode& node;

    inline uint8_t pinBit() const
        { return portNum - 1; }
    inline uint8_t pinBit2() const
        { return portNum + 3; }
public:
    RemotePort (RemoteNode& remote, uint8_t num) : Port (num), node (remote) {}

    void mode(uint8_t value) const;
    uint8_t digiRead() const;
    void digiWrite(uint8_t value) const;
    void anaWrite(uint8_t val) const;
    
    void mode2(uint8_t value) const;    
    uint16_t anaRead() const;
    uint8_t digiRead2() const;
    void digiWrite2(uint8_t value) const;    
};

class PortI2C : public Port {
    uint8_t uswait;
    
    inline void hold() const
        { delayMicroseconds(uswait); }
    inline void sdaOut(uint8_t value) const
        { mode(!value); digiWrite(value); }
    inline uint8_t sdaIn() const
        { return digiRead(); }
    inline void sclHi() const
        { hold(); digiWrite2(1); }
    inline void sclLo() const
        { hold(); digiWrite2(0); }
public:
    enum { KHZMAX = 1, KHZ400 = 2, KHZ100 = 9 };
    
    PortI2C (uint8_t num, uint8_t rate =KHZMAX);
    
    uint8_t start(uint8_t addr) const;
    void stop() const;
    uint8_t write(uint8_t data) const;
    uint8_t read(uint8_t last) const;
};

class DeviceI2C {
    const PortI2C& port;
    uint8_t addr;
    
public:
    DeviceI2C(const PortI2C& p, uint8_t me) : port (p), addr (me << 1) {}
    
    bool isPresent() const;
    uint8_t send() const
        { return port.start(addr); }
    uint8_t receive() const
        { return port.start(addr | 1); }
    void stop() const
        { port.stop(); }
    uint8_t write(uint8_t data) const
        { return port.write(data); }
    uint8_t read(uint8_t last) const
        { return port.read(last); }
        
    uint8_t setAddress(uint8_t me)
        { addr = me << 1; }
};

// The millisecond timer can be used for timeouts up to 60000 milliseconds.
// Setting the timeout to zero disables the timer.
//
// for periodic timeouts, poll the timer object with "if (timer.poll(123)) ..."
// for one-shot timeouts, call "timer.set(123)" and poll as "if (timer.poll())"

class MilliTimer {
    word next;
    byte armed;
public:
    MilliTimer () : armed (0) {}
    
    byte poll(word ms =0);
    word remaining() const;
    byte idle() const { return !armed; }
    void set(word ms);
};

// interface for the Blink Plug - see http://news.jeelabs.org/docs/bp1.html

class BlinkPlug : public Port {
    MilliTimer debounce;
    byte leds, lastState;
public:
    BlinkPlug (byte port)
        : Port (port), leds (0), lastState (0) {}
    
    void ledOn(byte mask);
    void ledOff(byte mask);
    byte state();
    byte pushed();
};

// interface for the Memory Plug - see http://news.jeelabs.org/docs/mp1.html

class MemoryPlug : public DeviceI2C {
public:
    MemoryPlug (PortI2C& port)
        : DeviceI2C (port, 0x50) {}

    void load(word page, void* buf, byte offset =0, int count =256);
    void save(word page, const void* buf, byte offset =0, int count =256);
};

class MemoryStream {
    MemoryPlug& dev;
    word start, curr;
    char step;
    byte buffer[256], pos;
public:
    MemoryStream (MemoryPlug& plug, word page =0, char dir =1)
            : dev (plug), start (page), curr (page), step (dir), pos (0) {}
    
    long position(byte writing =0) const;
    byte get();
    void put(byte data);
    word flush();
    void reset();
};

// interface for the UART Plug - see http://news.jeelabs.org/docs/up1.html

class UartPlug : public Print {
    DeviceI2C dev;
    // avoid per-byte access, fill entire buffer instead to reduce I2C overhead
    byte rxbuf[20], in, out;

    void regSet (byte reg, byte value);
    void regRead (byte reg);
    
public:
    UartPlug (PortI2C& port, byte addr)
        : dev (port, addr), in (0), out (0) {}
        
    void begin(long);
    byte available();
    int read();
    void flush();
    virtual void write(byte);
};

// interface for the Dimmer Plug - see http://news.jeelabs.org/docs/dp1.html

class DimmerPlug : public DeviceI2C {
public:
    enum {
        MODE1, MODE2,
        PWM0, PWM1, PWM2, PWM3, PWM4, PWM5, PWM6, PWM7,
        PWM8, PWM9, PWM10, PWM11, PWM12, PWM13, PWM14, PWM15,
        GRPPWM, GRPFREQ,
        LEDOUT0, LEDOUT1, LEDOUT2, LEDOUT3,
        SUBADR1, SUBADR2, SUBADR3, ALLCALLADR,
    };

    DimmerPlug (PortI2C& port, byte addr)
        : DeviceI2C (port, addr) {}
        
    void setReg(byte reg, byte value) const;
    byte getReg(byte reg) const;
};

// interface for the Lux Plug - see http://news.jeelabs.org/docs/xp1.html

class LuxPlug : public DeviceI2C {
    union { byte b[4]; word w[2]; } data;
public:
    enum {
        CONTROL, TIMING,
        THRESHLOWLOW, THRESHLOWHIGH, THRESHHIGHLOW, THRESHHIGHHIGH, INTERRUPT,
        ID = 0xA,
        DATA0LOW = 0xC, DATA0HIGH, DATA1LOW, DATA1HIGH,
    };

    LuxPlug (PortI2C& port, byte addr) : DeviceI2C (port, addr) {}
    
    void begin() {
        send();
        write(0xC0 | CONTROL);
        write(3); // power up
        stop();
    }
    
    const word* getData();

    word calcLux(byte iGain =0, byte tInt =2) const;
};

// interface for the Gravity Plug - see http://news.jeelabs.org/docs/gp1.html

class GravityPlug : public DeviceI2C {
    union { byte b[6]; int w[3]; } data;
public:
    GravityPlug (PortI2C& port) : DeviceI2C (port, 0x38) {}
    
    void begin() {}
    
    const int* getAxes();
};

// interface for the Heading Plug - see http://news.jeelabs.org/docs/hp1.html
// FIXME - this HDPM01 interface class isn't working properly yet!

class HeadingPlug : public PortI2C {
    DeviceI2C eeprom, adc, compass;
    Port aux;
    // keep following fields in order:
    word C1, C2, C3, C4, C5, C6, C7;
    byte A, B, C, D;

    byte eepromByte(byte reg) const;
    void getConstants();
    word adcValue(byte press) const;
    void setReset(byte reset) const;

public:
    HeadingPlug (int num)
        : PortI2C (num), eeprom (*this, 0x50), adc (*this, 0x77),
          compass (*this, 0x30), aux (5-num) {}
    
    void begin();
    void pressure(int& temp, int& pres) const;
    void heading(int& xaxis, int& yaxis) const;
};

#endif
