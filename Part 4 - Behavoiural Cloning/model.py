import csv
import cv2
import numpy as np
from keras.models import Sequential
from keras.layers import Flatten, Dense, Lambda, BatchNormalization
from keras.layers import Conv2D, Convolution2D, MaxPooling2D, AveragePooling2D, Dropout, Cropping2D
from keras.callbacks import ModelCheckpoint
from sklearn.utils import shuffle

lines = []
images = []
measurements = []
correction = 0.2
output = 0


with open('/root/Desktop/data/driving_log.csv') as csvfile:
    reader = csv.reader(csvfile)
    for line in reader:
        lines.append(line)

for line in lines:
    #Adds the standard centered camera image
    image = cv2.imread(line[0])
    #image = cv2.cvtColor(image, cv2.COLOR_RGB2HSV) - Color spaces not necessary for getting the model to work so excluding
    steering_center = float(line[3])
    images.append(image)
    measurements.append(steering_center)
    #Adds a flipped centred camera image
    image_flipped = np.fliplr(image)
    images.append(image_flipped)
    measurements.append(steering_center * -1)
    #Adds the left camera image and correction measurements
    image_left = cv2.imread(line[1])
    images.append(image_left)
    measurements.append(steering_center + correction)
    #Adds the right camera image and corrective measurements
    image_right = cv2.imread(line[2])
    images.append(image_right)
    measurements.append(steering_center - correction)
          
X_linear = np.array(images)
y_linear = np.array(measurements)
X_train, y_train = shuffle(X_linear, y_linear, random_state=0)

#Defines the Keras model to train the model
model = Sequential()
model.add(Lambda(lambda x: x/255 - 0.5, input_shape=(160,320,3)))
model.add(Cropping2D(cropping=((65,25), (0,0))))
model.add(Conv2D(32,(5,5), strides=(2,2), activation='relu', padding='same', use_bias=True, bias_initializer='zeros'))
model.add(BatchNormalization())
model.add(Conv2D(48,(5,5), strides=(2,2), activation='relu', padding='same', use_bias=True, bias_initializer='zeros'))
model.add(BatchNormalization())
model.add(Conv2D(56,(5,5), strides=(2,2), activation='relu', padding='same', use_bias=True, bias_initializer='zeros'))
model.add(BatchNormalization())
model.add(Conv2D(64,(3,3), strides=(1,1), activation='relu', padding='same', use_bias=True, bias_initializer='zeros'))
model.add(BatchNormalization())
model.add(Conv2D(64,(3,3), strides=(1,1), activation='relu', padding='same', use_bias=True, bias_initializer='zeros'))
model.add(BatchNormalization())
model.add(Flatten())
model.add(Dense(100))
model.add(Dense(50))
model.add(Dense(10))
model.add(Dense(1))
#Model Loss function and optimiser
model.compile(loss='mse', optimizer='adam')
#This checkpoint allows the model to be saved after each epoch but only if there has been a validation improvement - very handy
checkpoint = ModelCheckpoint('model.h5', monitor='val_loss', verbose=1, save_best_only=True, mode='min')
checkpoint_list = [checkpoint]
#Trains the model
model.fit(X_train, y_train, validation_split=0.2, shuffle=True, nb_epoch=10, callbacks=checkpoint_list)

