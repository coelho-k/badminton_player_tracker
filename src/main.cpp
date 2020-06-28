#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/video.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include <cmath>
#include <algorithm>
#include "body.hpp"
#include "bounds.hpp"

using namespace cv;
using namespace std;

vector<string> trackerTypes = {"BOOSTING", "MIL", "KCF", "TLD", "MEDIANFLOW", "GOTURN", "MOSSE", "CSRT"}; 

// create tracker by name
Ptr<Tracker> createTrackerByName(string trackerType) 
{
  Ptr<Tracker> tracker;
  if (trackerType ==  trackerTypes[0])
    tracker = TrackerBoosting::create();
  else if (trackerType == trackerTypes[1])
    tracker = TrackerMIL::create();
  else if (trackerType == trackerTypes[2])
    tracker = TrackerKCF::create();
  else if (trackerType == trackerTypes[3])
    tracker = TrackerTLD::create();
  else if (trackerType == trackerTypes[4])
    tracker = TrackerMedianFlow::create();
  else if (trackerType == trackerTypes[5])
    tracker = TrackerGOTURN::create();
  else if (trackerType == trackerTypes[6])
    tracker = TrackerMOSSE::create();
  else if (trackerType == trackerTypes[7])
    tracker = TrackerCSRT::create();
  else {
    cout << "Incorrect tracker name" << endl;
    cout << "Available trackers are: " << endl;
    for (vector<string>::iterator it = trackerTypes.begin() ; it != trackerTypes.end(); ++it)
      std::cout << " " << *it << endl;
  }
  return tracker;
}

// Fill the vector with random colors
void getRandomColors(vector<Scalar> &colors, int numColors)
{
  RNG rng(0);
  for(int i=0; i < numColors; i++)
    colors.push_back(Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255))); 
}

// Create athlete body instances
vector<Body> playersList;

int main(int argc, char * argv[]) 
{
  cout << "Default tracking algoritm is CSRT" << endl;
  cout << "Available tracking algorithms are:" << endl;
  for (vector<string>::iterator it = trackerTypes.begin() ; it != trackerTypes.end(); ++it)
    std::cout << " " << *it << endl;
  
  // Set tracker type. Change this to try different trackers.
  string trackerType = "CSRT";

  // set default values for tracking algorithm and video
  string videoPath = "lee.mp4";
  
  // Initialize MultiTracker with tracking algo
  vector<Rect> bboxes;

  // create a video capture object to read videos
  cv::VideoCapture cap(videoPath);
  Mat frame;

  // Output frames
  Mat dst, bw, cdst, cdstP, mask1, mask2, mask0;

  // quit if unable to read video file
  if(!cap.isOpened()) 
  {
    cout << "Error opening video file " << videoPath << endl;
    return -1;
  }

  // read first frame
  cap >> frame;
  
  // draw bounding boxes over objects
  // selectROI's default behaviour is to draw box starting from the center
  // when fromCenter is set to false, you can draw box starting from top left corner
  bool showCrosshair = true;
  bool fromCenter = false;
  cout << "\n==========================================================\n";
  cout << "OpenCV says press c to cancel objects selection process" << endl;
  cout << "It doesn't work. Press Escape to exit selection process" << endl;
  cout << "\n==========================================================\n";
  cv::selectROIs("MultiTracker", frame, bboxes, showCrosshair, fromCenter);
  
  // quit if there are no objects to track
  if(bboxes.size() < 1)
    return 0;
  
  vector<Scalar> colors;  
  getRandomColors(colors, bboxes.size()); 
  
  // Create multitracker
  Ptr<MultiTracker> multiTracker = cv::MultiTracker::create();

  // initialize multitracker
  for(int i=0; i < bboxes.size(); i++)
  {
	  playersList.push_back(Body());
	  multiTracker->add(createTrackerByName(trackerType), frame, Rect2d(bboxes[i]));
  }

  // Get masks
  Mat white = courtMask(frame);

  // Get lines
  vector<Vec4i> linesP = getLines(white);

  // Court boundary
  sortLines(linesP);

  //--------------------------------------------------------------------------------------------------
  vector<Point2f> srcTri;
  vector<Point2f> dstTri;

  Mat rot_mat( 2, 3, CV_32FC1 );
  Mat warp_mat( 2, 3, CV_32FC1 );
  Mat src, warp_dst, warp_rotate_dst;

  warp_dst = Mat::zeros( frame.rows, frame.cols, frame.type() );

  srcTri.push_back(Point2f( 120, 327 ));
  srcTri.push_back(Point2f( 490, 329 ));
  srcTri.push_back(Point2f( 400, 123 ));
  srcTri.push_back(Point2f( 201, 123 ));

  dstTri.push_back(Point2f( 120, 327 ));
  dstTri.push_back(Point2f( 490, 329 ));
  dstTri.push_back(Point2f( 490, 1138 ));
  dstTri.push_back(Point2f( 120, 1138 ));

  warp_mat = findHomography( srcTri, dstTri, CV_RANSAC);

  Mat temp = getPerspectiveTransform(srcTri, dstTri);

  warpPerspective( frame, warp_dst, warp_mat, warp_dst.size() );
  namedWindow("Warped", WINDOW_NORMAL);
  imshow("Warped", warp_dst);
  vector<Point2f> input = { Point2f(400, 123) }; 
  vector<Point2f> output;
  perspectiveTransform(input, output, temp);
  //--------------------------------------------------------------------------------------------------
  
  // process video and track objects
  cout << "\n==========================================================\n";
  cout << "Started tracking, press ESC to quit." << endl;
  while(cap.isOpened()) 
  {
    // get frame from the video
    cap >> frame;
  
    // stop the program if reached end of video
    if (frame.empty()) break;

    //update the tracking result with new frame
    multiTracker->update(frame);

    // draw tracked objects
    for(unsigned i=0; i<multiTracker->getObjects().size(); i++)
    {
      rectangle(frame, multiTracker->getObjects()[i], colors[i], 2, 1);

	    // Updates player's position data
      // No real benefit of the move here
      // Experimenting
	    playersList[i].setPosition(move(multiTracker->getObjects()[i].x + multiTracker->getObjects()[i].width / 2), 
                                move(multiTracker->getObjects()[i].y + multiTracker->getObjects()[i].height / 2));
    }

    //for(int i=0; i < bboxes.size(); i++)
    //{
    //  float cp = playersList[i].courtPosition(minMax);
    //}
  

    // show frame
    imshow("MultiTracker", frame);
    
    // quit on x button
    if  (waitKey(1) == 27) break;

    imshow("tracking", frame);
    
   }

}
