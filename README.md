# Car Brand Detector


Detection of Toyota car brand sign using randomized hough transform for ellipse detection with result clustering.


Reference paper: 


Basca, Cosmin A., Mihai Talos, and Remus Brad. "Randomized hough transform for ellipse
detection with result clustering." Computer as a Tool, 2005. EUROCON 2005. The International
Conference on. Vol. 2. IEEE, 2005.

Project was written in C++. As it was written for educational purposes it does not use built in OpenCV algorithms and structures other then the cv::Mat class 
that is used to store the processed images. Drawing function are used as well to highlight found toyota signs.

## Project building

To build the project you need the OpenCV library and CMake.
Create and go to "build" directory in the project's folder. Run:
'''
cmake ..
make
'''

## Example of usage

To run the algorithm on a single photo:
'''
./CarBrandDetector ../TestPhotos/toyota1.jpg
'''

To run the algorithm on all test photos:
'''
./CarBrandDetector find ../TestPhotos/*.jpg
'''


