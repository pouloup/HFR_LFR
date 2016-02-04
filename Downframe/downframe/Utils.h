/**
 * Authors: Anthony ARCH - Pierre-Henri DAUVERGNE - Jérémy VIGOUR (ESIR)
**/
#pragma once

#include <opencv2\core.hpp>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif // M_PI

#ifndef EPS
#define EPS 2.2204E-16f
#endif // EPS

#ifndef C1
#define C1 (float) (0.01 * 255 * 0.01  * 255)
#endif C1 // C1 quality-metric

#ifndef C2
#define C2 (float) (0.03 * 255 * 0.03  * 255)
#endif C2 // C2 quality-metric

// sigma on block_size
double sigma(cv::Mat & m, int i, int j, int block_size);

// Covariance
double cov(cv::Mat & m1, cv::Mat & m2, int i, int j, int block_size);

// Mean squared error
double eqm(cv::Mat & img1, cv::Mat & img2);

//Compute the PSNR between 2 images
double psnr(cv::Mat & img_src, cv::Mat & img_compressed, int block_size);

// Compute the SSIM between 2 images
double ssim(cv::Mat & img_src, cv::Mat & img_compressed, int block_size, bool show_progress = false);

void compute_quality_metrics(char * file1, char * file2, int block_size);