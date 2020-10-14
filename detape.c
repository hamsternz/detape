///////////////////////////////////////////////////////////
// detape.c : Decode IBM cassette tapes, saved as WAV files
///////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdint.h>

static int bit_of_byte = 0;
static int byte = 0;

/////////////////////////////////////////////////////////////
// add_bit() : Collect the bits into bytes, then output them
// Should really write it to a file
/////////////////////////////////////////////////////////////
static void add_bit(int i) {
   byte =( byte <<1 ) | (i ? 1 : 0);
   bit_of_byte++;
   if(bit_of_byte == 8) {
      byte &= 0xFF;
#if 0
      printf("%02x '%c'\n", byte,  byte < 127 && byte > 31 ? byte : '?');
#else
      printf("%c", byte < 127 && byte > 31 ? byte : '?');
#endif
      bit_of_byte=0;
   }
}

int main(int argc, char *argv[]) {
   int run_length = 0, last = 0;
   int syncs_needed = 255;
   FILE *f;

   // Buffer for two 16 bit samples
   int16_t buffer[2];

  
   // Check for file name 
   if(argc != 2) 
      return 0;

   // Open file
   f = fopen(argv[1], "rb");
   if( f == NULL)
      return 0;

   // Skip over the WAV file header
   fseek(f,4*27,SEEK_SET);

   while(fread(buffer,4,1,f)==1) { 
      if(buffer[0] >= 0 && last < 0) {
        // If it is a rising edge
        if(run_length >= 10 && run_length <= 13) {
           // The longer bits are '1' bits
           if(buffer[0] > 0) {
              if(syncs_needed == 0)
                add_bit(1);
              if(syncs_needed > 1) {
                 syncs_needed--;    
              }
           }
        } else if(run_length >= 4 && run_length <= 7) {
           // The shorter bits are '1' bits
           if(buffer[0] > 0) { 
              if(syncs_needed == 0)
                add_bit(0);
              if(syncs_needed == 1) {
                 syncs_needed = 0;    
                 bit_of_byte = 0;
              }
           }
        } else {
           // Have we dropped out of sync?
           if(syncs_needed == 0) {
              printf("\n\nBit error - out of sync\n\n");
              syncs_needed = 255;
           }
        }
        run_length = 1;
      } else if(buffer[0] < 0 && last >= 0) {
        // We can ignore falling edges
        // apart from resetting the run length
        run_length = 1;
      } else {
        run_length++;
      }
      // Remember the last value
      last = buffer[0];
   }
   fclose(f);
}
