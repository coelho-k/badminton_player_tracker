#include "bounds.hpp"

Mat courtMask(Mat& frame)
{
    // Output frames
    Mat dst, bw, mask1, mask2, mask0;

    // Get edges
    Canny(frame, dst, 0, 50, 5);

    // converting the frame to hls color format
    cvtColor(frame,dst, COLOR_BGR2HLS); 

    inRange(dst, Scalar(36,0,0), Scalar(86,255,255), mask0);
    frame.copyTo(bw, mask0);
    imshow("1 - green mask", bw);

    // white mask to detect white lines
    inRange(bw, Scalar(0,200,0), Scalar(255,255,255), mask1);
    Mat white;
    frame.copyTo(white, mask1);
    imshow("2 - g/w/y mask", white);

    /* Get bottom court 
    // Point bottom = determineBoundary(linesP);
    // circle(frame, bottom, 20, colors[1], 2);
    cvtColor( white, src_gray, COLOR_BGR2GRAY );
    Mat dst = Mat::zeros( src.size(), CV_32FC1 );
    cornerHarris( src_gray, dst, blockSize, apertureSize, k );
    Mat dst_norm, dst_norm_scaled;
    normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    convertScaleAbs( dst_norm, dst_norm_scaled );
    for( int i = 0; i < dst_norm.rows ; i++ )
    {
        for( int j = 0; j < dst_norm.cols; j++ )
        {
            if( (int) dst_norm.at<float>(i,j) > thresh )
            {
                circle( dst_norm_scaled, Point(j,i), 5,  Scalar(0), 2, 8, 0 );
            }
        }
    }
    imshow("2 - CORNERS", dst_norm_scaled);
    */

   // yellow mask
    // inRange(bw, Scalar(10,0,100), Scalar(40,255,255), mask2);

    // Sum the masks
    // Mat mm = mask1 + mask2;

    // remove the background from gray frame
    // subtractor->apply(bw, dst);
    // imshow("3 - BG removal", dst);

    // combining the result of masking and background removal 
    //bitwise_or(mm, bw, dst);
    //imshow("3 - white mask as well", dst);

    // Intermediate output
    // imshow("intermediate output", bw);
    return white;
}

// Hough Line Transform
vector<Vec4i> getLines(Mat& masked)
{
    Mat cdst, cdstP;

    // Copy edges to the images that will display the results in BGR
    cvtColor( masked, masked, COLOR_BGR2GRAY );
    cvtColor(masked, cdst, COLOR_GRAY2BGR);
    cdstP = cdst.clone();

    // Probabilistic Line Transform
    vector<Vec4i> linesP; // will hold the results of the detection
    HoughLinesP(masked, linesP, 1, CV_PI/180, 50, 50, 10 ); // runs the actual detection

    // Draw the lines
    for( size_t i = 0; i < linesP.size(); i++ )
    {
        Vec4i l = linesP[i];
        line( cdstP, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
    }

    imshow("3 - cdstP", cdstP);

    return linesP;
}

// Get reference / edge of court position
Point determineBoundary(const vector<Vec4i>& lines)
{
    vector<Point> pts;
    for (int ii=0; ii < lines.size(); ii++)
    {
        Vec4i l = lines[ii];
        pts.push_back(Point(l[0],l[1]));
        pts.push_back(Point(l[2],l[3]));
    }

    sort(pts.begin(), pts.end(), [](Point pt1, Point pt2){ return (pt1.x < pt2.x); });

    return pts[0];
}