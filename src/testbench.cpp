#include "sobel_filter.h"
#include "ap_fixed.h"
#include "ap_int.h"
#include "hls_stream.h"
#include "ap_axi_sdata.h"
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "hls_opencv.h"

int main (void) {
    pix_s inStream, outStream;

    // import image as cv matrix
    cv::Mat inMat = cv::imread("C:/projects/sobel_filter/airplane.bmp");
    assert(inMat.empty() != 1);

    // convert from cv matrix to hls stream
    cvMat2AXIvideo(inMat, inStream);

    // pass image to module
    sobel_filter(inStream, outStream);

    // convert output stream to cv matrix
    cv::Mat outMat(512, 512, CV_8UC3);
    AXIvideo2cvMat(outStream, outMat);

    // write image to disk
    cv::imwrite("C:/projects/sobel_filter/airplane_dut.bmp", outMat);

    return 0;
}
