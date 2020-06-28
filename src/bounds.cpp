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
    imshow("2 - g/w mask", white);

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

bool compare(Vec4i a, Vec4i b) {
    return true;
}

// Get reference / edge of court position
void sortLines(vector<Vec4i>& lines)
{
    int min; 
    Vec4i temp;
    for (int ii=0; ii < lines.size(); ii++)
    {
        min = ii;
        for (int j = ii+1; j < lines.size(); j++)
        {
            if (lines[ii][0] > lines[j][0])
            {
                temp = lines[ii];
                lines[ii] = lines[j];
                lines[j] = temp;
            }
        }
    }
}

Mat calibrate(const Mat& frame, const vector<Vec4i>& lines)
{
    auto width = lines[lines.size()-1][0] - lines[0][0]; // pixel width
    float pixelsPerMetric = width / courtWidth;

    float verticalAdjust = courtHeight * pixelsPerMetric;

    vector<Point2f> srcTri;
    vector<Point2f> dstTri;

    Mat rot_mat( 2, 3, CV_32FC1 );
    Mat warp_mat( 2, 3, CV_32FC1 );
    Mat src, warp_dst, warp_rotate_dst;

    warp_dst = Mat::zeros( frame.rows, frame.cols, frame.type() );

    srcTri.push_back(Point2f( lines[0][0], lines[0][1] )); // bottom left
    srcTri.push_back(Point2f( lines[0][2], lines[0][3] )); // top left
    srcTri.push_back(Point2f( lines[lines.size()-1][0], lines[lines.size()-1][1] )); // bottom right
    srcTri.push_back(Point2f( lines[lines.size()-1][2], lines[lines.size()-1][3] )); // top right

    dstTri.push_back(Point2f( lines[0][0], lines[0][1] ));
    dstTri.push_back(Point2f( lines[0][0], lines[0][1] +  verticalAdjust));
    dstTri.push_back(Point2f( lines[lines.size()-1][0], lines[lines.size()-1][1] ));
    dstTri.push_back(Point2f( lines[lines.size()-1][0], lines[lines.size()-1][1] + verticalAdjust ));

    warp_mat = findHomography( srcTri, dstTri, CV_RANSAC);

    Mat temp = getPerspectiveTransform(srcTri, dstTri);

    warpPerspective( frame, warp_dst, warp_mat, warp_dst.size() );
    namedWindow("Warped", WINDOW_NORMAL);
    imshow("Warped", warp_dst);
    vector<Point2f> input = { Point2f(400, 123) }; 
    vector<Point2f> output;
    perspectiveTransform(input, output, temp);

    return temp;
}