SVHN street number recognition

1. tool-needed

tensorflow
PIL
scipy

2. preprocessing data

Download data from SVHN, and put data to corresponding 3 folder (train, test, extra)

run mat_to_pickle.py: 
python mat_to_pickle.py train/digitStruct.mat test/digitStruct.mat extra/digitStruct.mat
(the order of the command can't change)

run image_to_data train test extra

3. training 
python Image_ConvNet_v2.py 0

4. show part of recognition result of test dataset
python Image_ConvNet_v2.py 1
