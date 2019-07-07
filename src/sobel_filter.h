#include "hls_stream.h"
#include "ap_fixed.h"
#include "ap_axi_sdata.h"

typedef ap_axiu<24,1,1,1> pix_t;
typedef hls::stream<pix_t> pix_s;

void sobel_filter(pix_s&, pix_s&);
