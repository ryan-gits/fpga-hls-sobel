#include "hls_stream.h"
#include "ap_axi_sdata.h"
#include <stdio.h>
#include <math.h>

#define COLS 512
#define ROWS 512
#define N_BUFFERS 3

typedef ap_axiu<24,1,1,1> pix_t;
typedef hls::stream<pix_t> pix_s;

ap_uint<24> rgb2gray (ap_uint<24> pixel) {
    ap_uint<8> r, g, b, gray_val;

    r = pixel >> 16;
    b = pixel >> 8 & 0xFF;
    g = pixel & 0xFF;

    gray_val = (0.3 * r) + (0.59 * g) + (0.11 * b);
    return ((ap_uint<24>)gray_val << 16) | ((ap_uint<16>)gray_val << 8) | gray_val;
}

pix_t processData(pix_t (&lineBuffer)[512][N_BUFFERS], int col, int rdIdx) {
    ap_uint<8> kernel [3][3];
    pix_t prevCol [3][3];

    return lineBuffer[col][rdIdx];
}

void filter (pix_s& axis_src, pix_s& axis_dst) {
    #pragma HLS INTERFACE axis port=axis_src
    #pragma HLS INTERFACE axis port=axis_ds

    pix_t rdTmp, pixFinal;
    pix_t lineBuffer [512][N_BUFFERS];

    int wrIdx, rdIdx = 0;
    int bufferVld = 0;

    // iterate for number of rows in image, plus flush / process remaining pipeline
    ROW_LOOP : for (int i=0; i<ROWS+N_BUFFERS-1; i++) {
        // have enough data to start processing on a pixel by pixel basis
        // once we start receiving the final kernel row (no edge padding)
        if (i >= N_BUFFERS-1) bufferVld = 1;

        COL_LOOP : for (int j=0; j<COLS; j++) {
            #pragma HLS pipeline II=1

        	// load circular line buffers and convert to grayscale
        	if (i < ROWS) {
                rdTmp = axis_src.read();
                rdTmp.data = rgb2gray(rdTmp.data);
                lineBuffer[j][wrIdx] = rdTmp;
        	}

        	// process data once we have enough to work with
        	if (bufferVld) {
        	    pixFinal = processData(lineBuffer, j, rdIdx);
        	    axis_dst.write(pixFinal);
        	}
        }

        // circular rd and wr buffer pointers
        wrIdx = (wrIdx == N_BUFFERS-1) ? 0 : ++wrIdx;
        if (bufferVld) rdIdx = (rdIdx == N_BUFFERS-1) ? 0 : ++rdIdx;
    }
}

/*
// we have enough data ready to process
if (buffer_vld) {
	// edge cases
	if (i == N_BUFFERS) {
		// top left corner
		if (j == 0) {

	    // top right corner
		} else if (j == COLS) {

		}
	} else if (i == ROWS+N_BUFFERS) {
		// bottom left corner
		if (j == 0) {

	    // bottom right corner
		} else if (j == COLS) {

		}
	}
}
*/


/* 268322 max clocks for HLS core IP
#pragma HLS dataflow

hls::AXIvideo2Mat(axis_src, inMat);
hls::Sobel<1,0,3>(inMat, outMat);
hls::Mat2AXIvideo(outMat, axis_dst);
*/
