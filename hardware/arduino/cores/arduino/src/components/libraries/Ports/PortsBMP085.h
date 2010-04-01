// Port library interface to BMP085 sensors connected via I2C
// 2009-02-17 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: PortsBMP085.h 4727 2009-12-08 21:39:49Z jcw $

class BMP085 : public DeviceI2C {
    int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
    uint16_t ac4, ac5, ac6;
    
    uint16_t readWord(uint8_t last) const
        { uint16_t v = read(0) << 8; return v | read(last); }
    void readFromReg(uint8_t reg) const
        { send(); write(reg); receive(); }
            
public:
    enum { TEMP, PRES };
    uint16_t meas[2];
    
    BMP085 (const PortI2C& p) : DeviceI2C (p, 0x77) {}
    
    uint8_t startMeas(uint8_t type) const;
    uint16_t getResult(uint8_t type);
    
    uint16_t measure(uint8_t type)
        { delay(startMeas(type)); return getResult(type); }

    void getCalibData();
    void calculate(int16_t& tval, int32_t& pval) const;
};
