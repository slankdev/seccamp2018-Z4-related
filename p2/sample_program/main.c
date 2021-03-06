/* 
 * main.c
 * 
 * bash$ gcc main.c -o a.out
 * bash$ ./a.out
 * Open out.pcap with Wireshark.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

struct pcap_file_hdr {
  uint32_t magic_number;  /* magic number */
  uint16_t version_major; /* major version number */
  uint16_t version_minor; /* minor version number */
  int32_t  thiszone;      /* GMT to local correction */
  uint32_t sigfigs;       /* accuracy of timestamps */
  uint32_t snaplen;       /* max length of captured packets, in octets */
  uint32_t network;       /* data link type */
};

struct pcap_pkt_hdr {
  uint32_t ts_sec;   /* timestamp seconds */
  uint32_t ts_usec;  /* timestamp microseconds */
  uint32_t incl_len; /* number of octets of packet saved in file */
  uint32_t orig_len; /* actual length of packet */
};

static const unsigned char pkt1[98] = {
  0x82, 0xe6, 0x50, 0x71, 0xc7, 0x64, 0x88, 0xe9, /* ..Pq.d.. */
  0xfe, 0x57, 0xca, 0x3b, 0x08, 0x00, 0x45, 0x00, /* .W.;..E. */
  0x00, 0x54, 0xf3, 0x21, 0x00, 0x00, 0x40, 0x01, /* .T.!..@. */
  0x02, 0x2f, 0xc0, 0xa8, 0x02, 0x07, 0xc0, 0xa8, /* ./...... */
  0x02, 0x01, 0x08, 0x00, 0x74, 0xb5, 0xbe, 0x05, /* ....t... */
  0x00, 0x00, 0x5b, 0x6b, 0x18, 0x6b, 0x00, 0x04, /* ..[k.k.. */
  0x66, 0x67, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, /* fg...... */
  0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, /* ........ */
  0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, /* ........ */
  0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, /* .. !"#$% */
  0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, /* &'()*+,- */
  0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, /* ./012345 */
  0x36, 0x37                                      /* 67 */
};

static const unsigned char pkt2[98] = {
  0x88, 0xe9, 0xfe, 0x57, 0xca, 0x3b, 0x82, 0xe6, /* ...W.;.. */
  0x50, 0x71, 0xc7, 0x64, 0x08, 0x00, 0x45, 0x00, /* Pq.d..E. */
  0x00, 0x54, 0xcf, 0xb3, 0x00, 0x00, 0x40, 0x01, /* .T....@. */
  0x25, 0x9d, 0xc0, 0xa8, 0x02, 0x01, 0xc0, 0xa8, /* %....... */
  0x02, 0x07, 0x00, 0x00, 0x7c, 0xb5, 0xbe, 0x05, /* ....|... */
  0x00, 0x00, 0x5b, 0x6b, 0x18, 0x6b, 0x00, 0x04, /* ..[k.k.. */
  0x66, 0x67, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, /* fg...... */
  0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, /* ........ */
  0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, /* ........ */
  0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, /* .. !"#$% */
  0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, /* &'()*+,- */
  0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, /* ./012345 */
  0x36, 0x37                                      /* 67 */
};

void write_packet(FILE* fp, const void* pkt, size_t len) {
  struct pcap_pkt_hdr ph;
  ph.ts_sec = 0;
  ph.ts_usec = 0;
  ph.incl_len = len;
  ph.orig_len = len;
  fwrite(&ph, sizeof(ph), 1, fp);
  fwrite(pkt, len, 1, fp);
}

void write_filehdr(FILE* fp) {
  struct pcap_file_hdr fh;
  fh.magic_number = 0xa1b2c3d4;
  fh.version_major = 2;
  fh.version_minor = 4;
  fh.thiszone = 0;
  fh.sigfigs = 0;
  fh.snaplen = 65535;
  fh.network = 1;
  fwrite(&fh, sizeof(fh), 1, fp);
}

int main(int argc, char** argv)
{
  FILE* fp = fopen("out.pcap", "wb");
  write_filehdr(fp);
  write_packet(fp, pkt1, sizeof(pkt1));
  write_packet(fp, pkt2, sizeof(pkt2));
  fclose(fp);
}