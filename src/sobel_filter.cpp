#include "hls_stream.h"
#include "ap_axi_sdata.h"
#include "hls_math.h"
#include "ap_fixed.h"
#include <stdio.h>

#define COLS 1920
#define ROWS 1080

typedef ap_ufixed<16,8> pix_t;
typedef hls::stream<pix_t> pix_s;

void sobel_filter (pix_s &axis_src, pix_s &axis_dst) {
	#pragma HLS INTERFACE axis port=axis_src
    #pragma HLS INTERFACE axis port=axis_dst

	pix_t rd_tmp;
    for (int i=0; i<15; i++) {
    	if (!axis_src.empty()) rd_tmp = axis_src.read();
    	printf("Got %d, val: %d\n", i, rd_tmp.to_int());
    }
}
