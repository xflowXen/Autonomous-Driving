**Advanced Lane Finding Project**

[//]: # (Image References)

[image1]: ./camera_cal/calibration1.jpg "Sample Test Image"
[image2]: ./camera_cal/calibration1_undist.jpg "Undistorted"
[image3]: ./examples/ptransform.png "Perspective Transformation"
[image4]: ./examples/channels.png "Colour Channels"
[image5]: ./examples/gradient.png "Gradient Detection"
[image6]: ./examples/combined.png "Combined Binary"
[image7]: ./examples/lanedetect.png "Detected Curve"
[image8]: ./examples/overlay.png "Final Image"

The first solution to finding lanes on the road utilised a pipeline which:
1. Read the raw image into a computable array format (based on RGB encodings)
2. Region masking the images to select only the area of interest
3. Detecting lines using the OpenCV canny edge detection and after applying a custom Gaussian smoothing factor and greyscale 
4. Applying a Hough transform to the edge detected image to select only the lane lines
5. Highligting the lane lines in the image and extrapolating them to the edge of the area of interest

However, it was not a production ready method of processing as it didn't always find the lane lines correctly - and in some cases found incorrect lines. It goes without saying that in the real world this could cause automated navigation systems to crash or be otherwise unreliable.

Below is an updated method for finding correct lane lines based which:
1. Eliminates distortion from the camera
2. Improves lane detection via
    * Analysis based on alternate colour spaces
    * Improved edge detection using Sobel operators
    * Image perspective transformation and transformed image histograms 



* Compute the camera calibration matrix and distortion coefficients given a set of chessboard images.
* Apply a distortion correction to raw images.
* Use color transforms, gradients, etc., to create a thresholded binary image.
* Apply a perspective transform to rectify binary image ("birds-eye view").
* Detect lane pixels and fit to find the lane boundary.
* Determine the curvature of the lane and vehicle position with respect to center.
* Warp the detected lane boundaries back onto the original image.
* Output visual display of the lane boundaries and numerical estimation of lane curvature and vehicle position.


### Step 1 - Detecting and eliminating camera distortion
---
The code for this step is contained in the first code cell of the IPython notebook located in "./P2_Solution.ipynb". I started by preparing "object points", which will be the (x, y, z) coordinates of the chessboard corners in the world. Here I am assuming the chessboard is fixed on the (x, y) plane at z=0, such that the object points are the same for each calibration image.  Thus, `objp` is just a replicated array of coordinates, and `objpoints` will be appended with a copy of it every time I successfully detect all chessboard corners in a test image.  `imgpoints` will be appended with the (x, y) pixel position of each of the corners in the image plane with each successful chessboard detection.  

I then used the output `objpoints` and `imgpoints` to compute the camera calibration and distortion coefficients using the `cv2.calibrateCamera()` function.  I applied this distortion correction to a distorted test image:

![alt text][image1]

using the `cv2.undistort()` function and obtained this result:

![alt text][image2]


### Step 2. Image Pipeline processing for detecting lane lines

The code for this step is contained in the third code cell of the IPython notebook located in "./P2_Solution.ipynb". The second cell shows the previous pipeline for reference while the third shows a modified version which uses more advanced techniques to accomplish a better detection result. 

#### Perspective Transformation
The first technique employed that was different from the original pipeline was the use of a perspective transform. The perspective transform gives a birds eye view of the area of interest and allows for more accurate determination of the lane lines. An example of the transformation from one of the images is below:

![alt text][image3]

#### Color Spaces and Thresholds

The next technique employed was to analyse the perspective transformed image through a variety of colour spaces. More specifically the pipeline uses the RGB, HSL, LAB and YCrCb colour spaces. Each colour space was tweaked using its own threshold in order to maximise the probability of lane line detection and in some cases (like RGB) each color space was given its own threshold. 

An example of the output results from various colour spaces is shown here:

![alt text][image4]

#### Gradient analysis

The next tehnique employed on the perspective transformed image was gradient analysis using the Sobel operator. An example of the binary output from this gradient analysis is shown below:

![alt text][image5]

This operator was very good at delineating gradients where there was a strong contrast between the colour of the lane line and of the road - however, whenever the colour difference was small and especially where the image became blurred due to the perspective transform - gradient analysis became less and less useful.

This suggests that other image processing techniques such as image enhancement and colour correction may be important analyses to include in the pipeline. 

#### Combined Analysis
All of the images generated from the perspective tranformed colour and gradient analysis was then fused into a single binary image. The code for this can also be seen in the third  code cell of the solution notebook but an example is provided below:

An example of the composite image from the above color spaces is shown below:

![alt text][image6]

The highest probability channels on the test set turned out to be the:

Red and Green channels from the RGB space,
Saturation and Lightness from the HSL space,
Lightness from the LAB space and 
Chroma (Y) channel from the YCbCr space.

Its worth noting that some of these channels are quite similar and attempt to replicate the way that human vision works. Its important to also note that our brain does also do alot of other types of processing (like "filling in the blanks" that can't be fully replicated by analysis of color space alone.


#### Lane Line and Lane Curvature detection
Once the final image that provided a high probability of line detection was generated - the pipeline then performed a detection of the lines by generating a histogram of the binary image. As expected, this resulted in the most results being detected in the range of pixels where the lane lines were. 

This information was then used to deduce where the lane lines were and to generate a sliding window of detecting "on" pixels in the binary image. 9 windows were used in total to split the identified lines into segments for more accurate detection. An example of the binary image showing the detection points is below:

This image data was then used to generate a curve (instead of a line in the previous example) by using the cv2.polylines function (see code cell 3). The curve generated defined a curve with the form y = x^2 + bx + c which allowed the lane curvature to be calculated as in the example below:

![alt text][image7]


### Lane Line Perspective 
Once the polynomial curve had been detected - it was drawn on the perspective transformed image and then finally reverted back to its original perspective as shown below:

![alt text][image8]

---

### Pipeline Video Example

Here's a [link to the final video result](./output_video/project_video.mp4)

---

### Discussion

The above process highlights the method of determining lane lines using advanced methods. However, these methods are still quite static. 

In the test cases where the road is quite light and matches the color spaces, however, more advanced methods will need to be employed such as glare detection and reduction and increasing the contrast of images as a pre-processing step. The process above was able to generate a high quality image without resorting to using the Sobel operator as the additional color spaces provided enough accurate data to determine the lane lines but with good image pre-processing it would be possible to rely on this operator for higher probability detections.

Additionally, as the conditions are subject to change requiring different thresholding values for effective detection - a better approach may be identfying the unique range of conditions that the car will need to be driven under - collect data for each of these conditions and then employing neural networks which are flexible enough to detect the lane lines under each given condition.

The pipeline as defined above is also inefficient and can be sped up if a more robust image processing method were used.
