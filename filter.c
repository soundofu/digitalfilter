
typedef union {
  struct {
    unsigned long signal_0:1;
    unsigned long signal_1:1;
    unsigned long signal_2:1;
    unsigned long signal_3:1;
    unsigned long signal_4:1;
    unsigned long signal_5:1;
    unsigned long signal_6:1;
    unsigned long signal_7:1;
    unsigned long signal_8:1;
    unsigned long signal_9:1;
    unsigned long signal_10:1;
    unsigned long signal_11:1;
    unsigned long signal_12:1;
    unsigned long signal_13:1;
    unsigned long signal_14:1;
    unsigned long signal_15:1;
    unsigned long signal_16:1;
    unsigned long signal_17:1;
    unsigned long signal_18:1;
    unsigned long signal_19:1;
    unsigned long signal_20:1;
    unsigned long signal_21:1;
    unsigned long signal_22:1;
    unsigned long signal_23:1;
    unsigned long signal_24:1;
    unsigned long signal_25:1;
    unsigned long signal_26:1;
    unsigned long signal_27:1;
    unsigned long signal_28:1;
    unsigned long signal_29:1;
    unsigned long signal_30:1;
    unsigned long signal_31:1;
  }bit;
  unsigned long all;
}SIGNAL;

SIGNAL signal_all_32 = {0};

unsigned long signal_and_result = 0;
unsigned long signal_or_result = 0;
unsigned long signal_xor_result = 0;

void SignalFilterEverytime(void) {
  signal_and_result &= signal_all_32.all;
  signal_or_result |= signal_all_32.all;
}

unsigned long SignalFilter(void) {
  signal_xor_result = signal_and_result ^ signal_or_result;
  signal_and_result = 0xFFFFFFFF;
  signal_or_result = 0x00000000;
  return (!signal_xor_result & signal_or_result);
}
