
#ifdef __cplusplus
extern "C"
{
#endif

#define I2CBUS "/dev/i2c-1"

   int i2cfd_;   // I2C file descriptor
   int verbose_; // debug flag, 0 = normal, 1 = debug mode

   typedef enum
   {
      channel1 = 0x01,
      channel2 = 0x02,
      channel3 = 0x04,
      channel4 = 0x08,
      channel5 = 0x10,
      channel6 = 0x20,
      channel7 = 0x40,
      channel8 = 0x80,
   } multiplexer_t;

void get_i2cbus_multiplexer(char*, char*);     // get the I2C bus file handle
int set_multiplexer(multiplexer_t);            // set the multiplexer channel

#ifdef __cplusplus
}
#endif