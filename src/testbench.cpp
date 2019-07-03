#include "sobel_filter.h"
#include "ap_fixed.h"
#include "hls_stream.h"
#include "ap_axi_sdata.h"
#include <stdio.h>

int main (void) {
	pix_s pixels;
	pix_s ret;

	pix_t tmp;
	tmp = 14.2;
	pixels.write(tmp);

	sobel_filter(pixels, ret);

	tmp = ret.read();
    printf("Got %f out\n", tmp.to_float());

	return 0;
}
