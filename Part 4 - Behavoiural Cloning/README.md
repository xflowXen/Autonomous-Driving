# **Behavioral Cloning** 

**Behavioral Cloning Project**

The goals / steps of this project are the following:
* Use the simulator to collect data of good driving behavior
* Build, a convolution neural network in Keras that predicts steering angles from images
* Train and validate the model with a training and validation set
* Test that the model successfully drives around track one without leaving the road
* Summarize the results with a written report


[//]: # (Image References)

[image1]: ./images/finalmodel.png "Final Model Visualization"
[image2]: ./images/center1.jpg "Center Image"
[image3]: ./images/center2.jpg "Flipped Image"
[image4]: ./images/left.jpg "Left Image"
[image5]: ./images/right.jpg "Right Image"



### Project Files
The following files have been included:

**Model and Code Files**
* [model.py](model.py) - The script to create and train the model
* [drive.py](drive.py) - script to be used to drive the car in autonomous mode
* [model.h5](https://www.dropbox.com/s/5ra5jny2rymtaet/model.h5?dl=0)  - contains a trained neural network

**Videos of Autonomous driving with the model**
* [Track 1](https://youtu.be/ZsfDXsD4Anc) - Shows the model driving around track 1
* [Track 2](video/track2.avi) - Shows the model attempting to drive around track 2

**Writeup and Analysis**
* writeup_report.md  - This report summarising the steps taken to develop the model
* [Model Development Record](data/Steps.xlsx) - Shows the steps taken to develop the model and get it to a working state


### Model Development
Using the Udacity provided simulator and my drive.py file, the car can be driven autonomously around the track by executing 
```sh
python drive.py model.h5
```
The [model.py](model.py) file contains the code for training and saving the convolution neural network. The file shows the pipeline I used for training and validating the model, and it contains some comments to explain how the code works.

The model consists of a convolution neural network with 5x5 and 3x3 filter sizes and depths between 32 and 64 (model.py lines 46-64) 

The model includes RELU layers to introduce nonlinearity, and the data is normalized in the model using a Keras lambda layer (code line 48). 

As can be seen from the Model development record, early attempts at using dropout didn't yield the results expected so I opted to use several Batch Normalisation layers to reduce overfitting. Batch Normalisation was quite effective at regularising the layers and allowing them to learn without losing data as the model worked much better when it was enabled. The Batch Normalisation layers also improved the training performance and meant that no dropout was needed in the final model - as expected based on the [paper](https://arxiv.org/abs/1502.03167) introducing the method.

The model was trained and validated against Track 2. While it didn't complete the course, the model's ability to correctly stay on the road for the first few turns shows that the model is indeed quite general. However, more work to generalise will be needed and additional data (like staying in lane and making sharp uphill corners) would be needed before this model could tackle real world conditions (or even track 2 for that matter).

The model used an adam optimizer, so the learning rate was not tuned manually ([model.py](model.py) line 66).

### Data Collection

Full details of the training data used can be found in the [Model Development Record](data/Steps.xlsx). To collect the data I drove several laps around the track with recovery from the left and the right as well as centre lane driving. Training data was chosen to keep the vehicle driving on the road as it was quite apparent when mistakes were made in data collection that the model would quickly degrade. 

Sample training data can be found in the data folder.

### Model Testing
In order to determine how well the model was working, I split the training data into a training set and a validation set ([model.py](model.py) line 71). The validation set was used to determine the performance of the model as there were quite a few times while training that the model fit generated an extremely low validation loss against the training set but a relatively high one against the validation set. This suggested that the model was overfitting to the data in the training set.

The data used in the validation and training sets was selected at random using the skutil tools. ([model.py](model.py) lines 8 and 44).

The full set of tests used can be found in the [Model Development Record](data/Steps.xlsx) - which includes use of dropout and batch normalisation techniques to reduce overfitting.

The final step was to run the simulator to see how well the car was driving around track one. There were a few spots where the vehicle fell off the track and initially I responded by collecting more data. Over time though it became apparent that more data wasn't necessarily as good as high quality data to train from. So I started augmenting with the data that had been collected from the D-1 dataset which included 2 solid laps around the track and some recovery driving. 

This was then flipped and used in the model to increase the amount of high quality data the model had available to work on.

At the end of the process, the vehicle is able to drive autonomously around the track without leaving the road.

### Final Model Architecture and Performance

The final model architecture can be seen in using one of the formats below: 

* [SVG](model.svg)
* [PDF](Model.pdf)

![Final model architecture][image1]

In addition the performance of this model on the tracks can be seen in the videos below:

* [Track 1](https://www.youtube.com/watch?v=ZsfDXsD4Anc&t=63s)
* [Track 2 Attempt](video/track2.avi)

----
### Further Details on the  Creation of the Training Set & Training Process

To capture good driving behavior, I first recorded two laps on track one using center lane driving. Here is an example image of center lane driving:

![alt text][image2]

I then enhanced the dataset to include the flipped images by using numpy's flipr function (see model.py line 30)

![alt text][image3]


Next I included the data from the left and right cameras in order to provide depth for the model to understand when it was getting too far away from the center of the road. This is done in model.py at lines 34 to 40.

![alt text][image4]
![alt text][image5]

While I did collect data for track 2, I didn't have enough time to be able to train the model on this data.
