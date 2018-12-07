[//]: # (Image References)

[image1]: ./images/CHistogram.png "Histogram"
[image2]: ./images/original.png "Original"
[image3]: ./images/normalised.png "Greyscaling"
[image4]: ./images/gts1.png "Traffic Sign 1"
[image5]: ./images/gts2.png "Traffic Sign 2"
[image6]: ./images/gts3.png "Traffic Sign 3"
[image7]: ./images/gts4.png "Traffic Sign 4"
[image8]: ./images/gts5.png "Traffic Sign 5"


# **Traffic Sign Recognition** 

---

**Build a Traffic Sign Recognition Project**

The goals / steps of this project are the following:
* Load the data set (which was provided in the data folder of the Jupyter environment)
* Explore, summarize and visualize the data set
* Design, train and test a model architecture
* Use the model to make predictions on new images
* Analyze the softmax probabilities of the new images
* Summarize the results with a written report




### Traffic Classifier Overview

### Data Set Summary & Exploration

The first step in building the classifier was reviewing the data to see what kind of traffic images there were. A quick review of the images themselves showed that there were a number of different signs taken under different lighting conditions, at different times of day and with varying degrees of clarity - either from a resolution or distance perspective. 

* The size of training set is 34799
* The size of the validation set is 4410
* The size of test set is 12630
* The shape of a traffic sign image is 32x32
* The number of unique classes/labels in the data set is 43

A full list of the classes is available [here](./classes.txt)

#### Data Overview

Here is an exploratory visualization of the data set. It is a bar chart showing how the data is segmented into the various traffic sign types.

![Image Type Overview of the dataset][image1]

### Design and Test a Model Architecture

#### 1. Image Pre-processing

As a first step, I decided to convert the images to a normalised 3 channel image with values between 0.1 and 0.9. This is important because the values typically range from 1 to 255 and this would make it harder to properly converge when the gradient descent or other optimisation equation started. Ideally the feature set should have a normalised distribution across features to make numerical computation simpler and more accurate. A good rule of thumb is to have a mean of 0.5 and a variance of 0. 

As part of the preprocessing I decided against converting the images to grayscale. This is because the colour information is actually an important part of determining the sign type and can be a differentiator in some cases. As an example - the German traffic signs (https://en.wikipedia.org/wiki/Road_signs_in_Germany) typically have a sign suggesting that something is allowed or specifying a limit (Busses permitted) and one suggesting that the same thing isn't (Busses not permitted). In this case, when busses are not permitted the red circle is included around it, changing the meaning completely.

I didn't add noise to these examples as they were already quite a diverse and challenging set of examples - however - it would be an important part of a real-world implementation..

Here is an example of a traffic sign image before and after grayscaling.

![alt text][image2] and ![alt text][image3]



#### 2. Neural Network Architecture

My final model consisted of the following layers:

| Layer         		|     Description	        					| 
|:---------------------:|:---------------------------------------------:| 
| Input         		| 32x32x3 RGB image   							| 
| Convolution 8x8     	| 1x1 stride, valid padding, outputs 25x25x10 	|
| RELU					|												|
| Max pooling	      	| 2x2 stride,  outputs 12x12x10				|
| Convolution 4x4     	| 1x1 stride, valid padding, outputs 9x9x30 	|
| RELU					|												|
| Max pooling	      	| 2x2 stride,  outputs 4x4x30				|
| Fully connected		| flattend 480       									|
| Fully connected		| flattend 172       									|
| Fully connected		| flattend 86       									|
| Output 	| flattened 43      									|



#### 3. Neural Net Training

To train the model, I tested various optimisers - the Adagrad and adadelta as well as the Adam optimiser. Over many trials there were many bad optimisations around the 4.5 and 5.4% accuracy points. However, the Adam optimiser with a small initial value (0.0001 and below) and a rho(decay) of around 0.9-0.99 performed best against a batch size of 100. It wasn't generally necessary to train beyond 20 - 25 epochs and the best accuracies (high of 94.1%) were typically obtained by the 10th epoch.

The best combination of points was:
Optimizer - AdamOptimiser
Learning Rate - 0.0006
Rho - 0.97
Batch size - 1000

It yielded a 99.1% accuracy rate in training and a 94.1% rate in validation along with a 91% accuracy rate against the test set.

#### 4. Optimisation Approach
To find the optimal values above, I constructed a grid with differing sizes of Batches, Epochs, Learning rates and other hyperparameters. I then updated each parameter around a range until it yielded no further change (positive or negative) to determine the optimal starting points and ranges to choose for each optimiser.

After that initial analysis, I focused on the optimiser, batch, epoch, learning rate and hyper parameter that gave the best results across the board and used that as a starting point to begin tweaking the values.

The rationale for this approach is that an optimisation is like a search space - by doing the inital analysis I was able to find a set of variables which gave a pretty good estimate of the minimum loss and helped the optimiser to more quickly locate the area of the search space that was best for starting optimisation in a similar way to a Stochastic Gradient Descent algorithm would.

I also changed the convolution amount in a way that would give the convolutional layers the best chance of detecting features based on the 32x32 image size. Starting with a 2x2 grid didn't make sense as the recognisatble features tended to span 4-8 pixels on average. 

The results with this approach were good but could be improved by adding a dropout layer to avoid overfitting against the data.
 

### Testing Model on New Images

#### 1. Real world test (Kind of)

Here are five German traffic signs that I found on the web:

![alt text][image4] ![alt text][image5] ![alt text][image6] 
![alt text][image7] ![alt text][image8]

The first image might be difficult to classify because it wasn't strictly a part of the collected data imageset. I chose it because it was close enough to the pedestrian crossing sign with many of the same elements, but it was different enough to see if it would be correctly selected.

#### 2. Real world predictions

Here are the results of the prediction:

| Image			        |     Prediction	        					| 
|:---------------------:|:---------------------------------------------:| 
| Pedestrian Crossing      		| Roundabout mandatory   									| 
| Stop Sign     			| Road narrows on the right 										|
| Road Narrowing				| Roundabout mandatory											|
| Slippery Road	      		| Road narrows on the right					 				|
| No Entry	| Roundabout mandatory      							|


The model was able to correctly guess 0 of the 5 traffic signs, which gives an accuracy of 0%. This is a large departure away from the test, validation and training set numbers in the 90's. It suggests that there may need to be alternate types of procedures such as dropout to prevent overfitting and even more data employed. It also suggests that noisier images may be necessary so that the model is able to correctly decipher what the correct points are that need to be correctly recognised.

#### 3. Real world Model accuracy

A quick review of the softmax probabilities shows that the probabilities are very close and that the model actually has very little certainty on which of the categories should be selected. The distribution still seems too disperse and isn't "normal" enough - suggesting that even though it converges rapidly to a solution that a sample size of 30,000 images may still be too small.

The code for making predictions on my final model is located in the final cell of the Ipython notebook.
