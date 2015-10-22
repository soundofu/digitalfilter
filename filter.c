
/**
 *  32 raw bits in a 32-bit variable
 */

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

/**
 * For every respect bit in signal_and_result and signal_or_result:
 * 
 * signal_and_result = A(n) & A(n-1) & A(n-2) & ...
 * if any A(n) != 1, signal_and_result = 0;
 * signal_or_result = A(n) | A(n-1) | A(n-2) | ...
 * if any A(n) == 1, signal_or_result = 1.
 * 
 * INDEX      C1  C2  C3  C4  C5
 * A(n)       1   1   0   0   1
 * A(n-1)     1   0   1   0   1
 * A(n-2)     1   0   1   0   0
 * A(n-3)     1   0   1   0   1
 * A(n-4)     1   0   1   0   1
 * ------------------------------
 * AND        1   0   0   0   0
 * OR         1   1   1   0   1
 * XOR        0   1   1   0   1   1: Noise or signal change, 0: signal stable
 * !XOR       1   0   0   1   0   0: signal stable, 1: noise or signal change
 * !XOR & OR  1   0   0   0   0   
 * ------------------------------
 * last & XOR 0   1   1   0   1   if last == 1
 * result     1   1   1   0   1   this = (last & XOR) | (!XOR & OR)
 *                        |--------------- Signal changed
 *                |---|-------|----------- Noise, no change
 *            |--------------------------- Signal stable, no change
 * ------------------------------
 * last & XOR 0   0   0   0   0   if last == 0
 * result     1   0   0   0   0   this = (last & XOR) | (!XOR & OR)
 *            |--------------------------- Signal changed
 *                |---|-------|----------- Noise, no change
 *                        |--------------- Signal stable, no change
 * ------------------------------
 */

void SignalFilterEverytime(void) {
  signal_and_result &= signal_all_32.all;
  signal_or_result |= signal_all_32.all;
}

unsigned long last_result = 0;
unsigned long this_result = 0;
unsigned long SignalFilter(unsigned long last) {
  unsigned long tmp_result;
  signal_xor_result = signal_and_result ^ signal_or_result;
  tmp_result = (last & signal_xor_result) | (~signal_xor_result & signal_or_result);
  signal_and_result = 0xFFFFFFFF;
  signal_or_result = 0x00000000;
  return tmp_result;
}

#define FILTER_CONST  20
void UpdateSignal(void) {
  signal_all_32.bit.signal_0 = 1;
  signal_all_32.bit.signal_1 = 1;
}
void main(void) {
  int i=0;
  while (1) {
    i++;
    UpdateSignal();
    SignalFilterEverytime();
    if (i >= FILTER_CONST) {
      i = 0;
      this_result = SignalFilter(last_result);
      last_result = this_result;          // save for next time
    }
  }
}
