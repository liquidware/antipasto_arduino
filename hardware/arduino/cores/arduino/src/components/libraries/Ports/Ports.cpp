// Ports library definitions
// 2009-02-13 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: Ports.cpp 4888 2010-03-23 16:28:07Z jcw $

#include "Ports.h"

// flag bits sent to the receiver
#define MODE_CHANGE 0x80    // a pin mode was changed
#define DIG_CHANGE  0x40    // a digital output was changed
#define PWM_CHANGE  0x30    // an analog (pwm) value was changed on port 2..3
#define ANA_MASK    0x0F    // an analog read was requested on port 1..4

uint16_t Port::shiftRead(uint8_t bitOrder, uint8_t count) const {
    uint16_t value = 0, mask = bit(LSBFIRST ? 0 : count - 1);
    for (uint8_t i = 0; i < count; ++i) {
        digiWrite2(1);
        delayMicroseconds(5);
        if (digiRead())
            value |= mask;
        if (bitOrder == LSBFIRST)
            mask <<= 1;
        else
            mask >>= 1;
        digiWrite2(0);
        delayMicroseconds(5);
    }
    return value;
}

void Port::shiftWrite(uint8_t bitOrder, uint16_t value, uint8_t count) const {
    uint16_t mask = bit(LSBFIRST ? 0 : count - 1);
    for (uint8_t i = 0; i < count; ++i) {
        digiWrite(value & mask);
        if (bitOrder == LSBFIRST)
            mask <<= 1;
        else
            mask >>= 1;
        digiWrite2(1);
        digiWrite2(0);
    }
}

RemoteNode::RemoteNode (char id, uint8_t band, uint8_t group) 
    : nid (id & 0x1F)
{
    memset(&data, 0, sizeof data);
    RemoteHandler::setup(nid, band, group);
}

void RemoteNode::poll(uint16_t msecs) {
    uint8_t pending = millis() >= lastPoll + msecs;
    if (RemoteHandler::poll(*this, pending))
        lastPoll = millis();
}

void RemotePort::mode(uint8_t value) const {
    node.data.flags |= MODE_CHANGE;
    bitWrite(node.data.modes, pinBit(), value);
}

uint8_t RemotePort::digiRead() const {
    return bitRead(node.data.digiIO, pinBit());
}

void RemotePort::digiWrite(uint8_t value) const {
    node.data.flags |= DIG_CHANGE;
    bitWrite(node.data.digiIO, pinBit(), value);
}

void RemotePort::anaWrite(uint8_t val) const {
    if (portNum == 2 || portNum == 3) {
        bitSet(node.data.flags, portNum + 2);
        node.data.anaOut[portNum - 2] = val;
    } else
        digiWrite2(val >= 128);
}
   
void RemotePort::mode2(uint8_t value) const {
    node.data.flags |= MODE_CHANGE;
    bitWrite(node.data.modes, pinBit2(), value);
}
   
uint16_t RemotePort::anaRead() const {
    bitSet(node.data.flags, pinBit());
    return node.data.anaIn[pinBit()];
}
   
uint8_t RemotePort::digiRead2() const {
    return bitRead(node.data.digiIO, pinBit2());
}

void RemotePort::digiWrite2(uint8_t value) const {
    node.data.flags |= DIG_CHANGE;
    bitWrite(node.data.digiIO, pinBit2(), value);
}

PortI2C::PortI2C (uint8_t num, uint8_t rate)
    : Port (num), uswait (rate)
{
    sdaOut(1);
    mode2(OUTPUT);
    sclHi();
}

uint8_t PortI2C::start(uint8_t addr) const {
    sclLo();
    sclHi();
    sdaOut(0);
    return write(addr);
}

void PortI2C::stop() const {
    sdaOut(0);
    sclHi();
    sdaOut(1);
}

uint8_t PortI2C::write(uint8_t data) const {
    sclLo();
    for (uint8_t mask = 0x80; mask != 0; mask >>= 1) {
        sdaOut(data & mask);
        sclHi();
        sclLo();
    }
    sdaOut(1);
    sclHi();
    uint8_t ack = ! sdaIn();
    sclLo();
    return ack;
}

uint8_t PortI2C::read(uint8_t last) const {
    uint8_t data = 0;
    for (uint8_t mask = 0x80; mask != 0; mask >>= 1) {
        sclHi();
        if (sdaIn())
            data |= mask;
        sclLo();
    }
    sdaOut(last);
    sclHi();
    sclLo();
    if (last)
        stop();
    sdaOut(1);
    return data;
}

bool DeviceI2C::isPresent () const {
    byte ok = send();
    stop();
    return ok;
}

byte MilliTimer::poll(word ms) {
    byte ready = 0;
    if (armed) {
        word remain = next - millis();
        // since remain is unsigned, it will overflow to large values when
        // the timeout is reached, so this test works as long as poll() is
        // called no later than 5535 millisecs after the timer has expired
        if (remain <= 60000)
            return 0;
        // return a value between 1 and 255, being msecs+1 past expiration
        // note: the actual return value is only reliable if poll() is
        // called no later than 255 millisecs after the timer has expired
        ready = -remain;
    }
    set(ms);
    return ready;
}

word MilliTimer::remaining() const {
    word remain = armed ? next - millis() : 0;
    return remain <= 60000 ? remain : 0;
}

void MilliTimer::set(word ms) {
    armed = ms != 0;
    if (armed)
        next = millis() + ms - 1;
}

void BlinkPlug::ledOn (byte mask) {
    if (mask & 1) {
        digiWrite(0);
        mode(OUTPUT);
    }
    if (mask & 2) {
        digiWrite2(0);
        mode2(OUTPUT);
    }
    leds |= mask;
}

void BlinkPlug::ledOff (byte mask) {
    if (mask & 1) {
        mode(INPUT);
        digiWrite(1);
    }
    if (mask & 2) {
        mode2(INPUT);
        digiWrite2(1);
    }
    leds &= ~ mask;
}

byte BlinkPlug::state () {
    byte saved = leds;
    ledOff(1+2);
    byte result = !digiRead() | (!digiRead2() << 1);
    ledOn(saved);
    return result;
}

byte BlinkPlug::pushed () {
    if (debounce.idle() || debounce.poll()) {
        byte newState = state();
        if (newState != lastState) {
            debounce.set(100);
            byte nowOn = (lastState ^ newState) & newState;
            lastState = newState;
            return nowOn;
        }
    }
    return 0;
}

void MemoryPlug::load (word page, void* buf, byte offset, int count) {
    setAddress(0x50 + (page >> 8));
    send();
    write((byte) page);
    write(offset);
    receive();
    byte* p = (byte*) buf;
    while (--count >= 0)
        *p++ = read(count == 0);
    stop();
}

void MemoryPlug::save (word page, const void* buf, byte offset, int count) {
    setAddress(0x50 + (page >> 8));
    send();
    write((byte) page);
    write(offset);
    const byte* p = (const byte*) buf;
    while (--count >= 0)
        write(*p++);
    stop();
}

long MemoryStream::position (byte writing) const {
    long v = (curr - start) * step;
    if (v > 0 && !writing)
        --v; // get() advances differently than put()
    return (v << 8) | pos;
}

byte MemoryStream::get () {
    if (pos == 0) {
        dev.load(curr, buffer);
        curr += step;
    }
    return buffer[pos++];
}

void MemoryStream::put (byte data) {
    buffer[pos++] = data;
    if (pos == 0) {
        dev.save(curr, buffer);
        curr += step;
    }
}

word MemoryStream::flush () {
    while (pos != 0)
        put(0xFF);
    delay(5);
    return curr;
}

void MemoryStream::reset () {
    curr = start;
    pos = 0;
}

// uart register definitions
#define RHR     (0 << 3)
#define THR     (0 << 3)
#define DLL     (0 << 3)
#define DLH     (1 << 3)
#define FCR     (2 << 3)
#define LCR     (3 << 3)
#define RXLVL   (9 << 3)

void UartPlug::regSet (byte reg, byte value) {
  dev.send();
  dev.write(reg);
  dev.write(value);
}

void UartPlug::regRead (byte reg) {
  dev.send();
  dev.write(reg);
  dev.receive();
}

void UartPlug::begin (long baud) {
    word divisor = 230400 / baud;
    regSet(LCR, 0x80);          // divisor latch enable
    regSet(DLL, divisor);       // low byte
    regSet(DLH, divisor >> 8);  // high byte
    regSet(LCR, 0x03);          // 8 bits, no parity
    regSet(FCR, 0x07);          // fifo enable (and flush)
    dev.stop();
}

byte UartPlug::available () {
    if (in != out)
        return 1;
    out = 0;
    regRead(RXLVL);
    in = dev.read(1);
    if (in == 0)
        return 0;
    if (in > sizeof rxbuf)
        in = sizeof rxbuf;
    regRead(RHR);
    for (byte i = 0; i < in; ++i)
        rxbuf[i] = dev.read(i == in - 1);
    return 1;
}

int UartPlug::read () {
    return available() ? rxbuf[out++] : -1;
}

void UartPlug::flush () {
    regSet(FCR, 0x07); // flush both RX and TX queues
    dev.stop();
    in = out;
}

void UartPlug::write (byte data) {
    regSet(THR, data);
    dev.stop();
}

void DimmerPlug::setReg(byte reg, byte value) const {
    send();
    write(reg);
    write(value);
    stop();
}

byte DimmerPlug::getReg(byte reg) const {
    send();
    write(reg);
    receive();
    byte result = read(1);
    stop();
    return result;
}

const word* LuxPlug::getData() {
    send();
    write(0xA0 | DATA0LOW);
    receive();
    data.b[0] = read(0);
    data.b[1] = read(0);
    data.b[2] = read(0);
    data.b[3] = read(1);
    stop();
    return data.w;
}

#define LUX_SCALE	14	// scale by 2^14 
#define RATIO_SCALE 9	// scale ratio by 2^9
#define CH_SCALE    10	// scale channel values by 2^10 

word LuxPlug::calcLux(byte iGain, byte tInt) const
{
    unsigned long chScale; 
    switch (tInt) {
        case 0:  chScale = 0x7517; break; 
        case 1:  chScale = 0x0fe7; break; 
        default: chScale = (1 << CH_SCALE); break;
    }
    if (!iGain)
        chScale <<= 4;
    unsigned long channel0 = (data.w[0] * chScale) >> CH_SCALE; 
    unsigned long channel1 = (data.w[1] * chScale) >> CH_SCALE; 

    unsigned long ratio1 = 0; 
    if (channel0 != 0)
        ratio1 = (channel1 << (RATIO_SCALE+1)) / channel0;
    unsigned long ratio = (ratio1 + 1) >> 1;

    word b, m;
         if (ratio <= 0x0040) { b = 0x01F2; m = 0x01BE; } 
    else if (ratio <= 0x0080) { b = 0x0214; m = 0x02D1; } 
    else if (ratio <= 0x00C0) { b = 0x023F; m = 0x037B; } 
    else if (ratio <= 0x0100) { b = 0x0270; m = 0x03FE; } 
    else if (ratio <= 0x0138) { b = 0x016F; m = 0x01FC; }
    else if (ratio <= 0x019A) { b = 0x00D2; m = 0x00FB; }
    else if (ratio <= 0x029A) { b = 0x0018; m = 0x0012; }
    else                      { b = 0x0000; m = 0x0000; }

    unsigned long temp = channel0 * b - channel1 * m;
    temp += 1 << (LUX_SCALE-1);
    return temp >> LUX_SCALE;
}

const int* GravityPlug::getAxes() {
    send();
    write(0x02);
    receive();
    for (byte i = 0; i < 5; ++i)
        data.b[i] = read(0);
    data.b[5] = read(1);
    stop();
    data.w[0] = (data.b[0] >> 6) | (data.b[1] << 2);
    data.w[1] = (data.b[2] >> 6) | (data.b[3] << 2);
    data.w[2] = (data.b[4] >> 6) | (data.b[5] << 2);
    return data.w;
}

byte HeadingPlug::eepromByte(byte reg) const {
    eeprom.send();
    eeprom.write(reg);
    eeprom.receive();
    byte result = eeprom.read(1);
    eeprom.stop();
    return result;
}

void HeadingPlug::getConstants() {
    for (byte i = 0; i < 18; ++i)
        ((byte*) &C1)[i < 14 ? i^1 : i] = eepromByte(16 + i);
    // Serial.println(C1);
    // Serial.println(C2);
    // Serial.println(C3);
    // Serial.println(C4);
    // Serial.println(C5);
    // Serial.println(C6);
    // Serial.println(C7);
    // Serial.println(A, DEC);
    // Serial.println(B, DEC);
    // Serial.println(C, DEC);
    // Serial.println(D, DEC);
}

word HeadingPlug::adcValue(byte press) const {
    aux.digiWrite(1);
    adc.send();
    adc.write(0xFF);
    adc.write(0xE0 | (press << 4));
    adc.stop();
    delay(40);
    adc.send();
    adc.write(0xFD);
    adc.receive();
    byte msb = adc.read(0);
    int result = (msb << 8) | adc.read(1);
    adc.stop();
    aux.digiWrite(0);
    return result;
}

void HeadingPlug::setReset(byte reset) const {
    compass.send();
    compass.write(0x00);
    compass.write(reset ? 0x04 : 0x02);
    compass.stop();
    delay(2);
}

void HeadingPlug::begin() {
    // prepare ADC
    aux.mode(OUTPUT);
    aux.digiWrite(0);
    
    // generate 32768 Hz on IRQ pin (OC2B)
    TCCR2A = bit(COM2B0) | bit(WGM21);
    TCCR2B = bit(CS20);
    OCR2A = 243;
    aux.mode3(OUTPUT);
    
    // reset the magnetometer
    delay(10);
    setReset(0);
    
    getConstants();
}

void HeadingPlug::pressure(int& temp, int& pres) const {
    word D2 = adcValue(0);
    // Serial.print("D2 = ");
    // Serial.println(D2);
    int corr = (D2 - C5) >> 7;        
    // Serial.print("corr = ");
    // Serial.println(corr);
    int dUT = (D2 - C5) - (corr * (long) corr * (D2 >= C5 ? A : B) >> C);
    // Serial.print("dUT = ");
    // Serial.println(dUT);
    temp = 250 + (dUT * C6 >> 16) - (dUT >> D); 

    word D1 = adcValue(1);
    // Serial.print("D1 = ");
    // Serial.println(D1);
    word OFF = (C2 + ((C4 - 1024) * dUT >> 14)) << 2;
    // Serial.print("OFF = ");
    // Serial.println(OFF);
    word SENS = C1 + (C3 * dUT >> 10);
    // Serial.print("SENS = ");
    // Serial.println(SENS);
    word X = (SENS * (D1 - 7168L) >> 14) - OFF;
    // Serial.print("X = ");
    // Serial.println(X);
    pres = (X * 10L >> 5) + C7;
}

void HeadingPlug::heading(int& xaxis, int& yaxis) const {
    compass.send();
    compass.write(0x00);
    compass.write(0x01);
    compass.stop();
    delay(50);
    compass.send();
    compass.write(0x00);
    compass.receive();
    byte tmp, reg = compass.read(0);
    tmp = compass.read(0);
    xaxis = (tmp << 8) | compass.read(0);
    tmp = compass.read(0);
    yaxis = (tmp << 8) | compass.read(1);
    compass.stop();
    // Serial.println(reg, HEX);
}
