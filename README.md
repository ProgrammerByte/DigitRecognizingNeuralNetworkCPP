# DigitRecognizingNeuralNetworkCPP
A trained neural network which recognises handwritten digits which are drawn by the user. The user can draw onto a canvas (OpenGL window) and the neural network will output what it thinks the user has drawn onto the command prompt.  
  
This is my first endeavour with machine learning so this project uses a basic neural network along with the use of backpropagation and gradient descent algorithms to train the network. Once the code was completed it took approximately a week to train the neural network and during this time I experimented with the training algorithm including the learning rate and the batch size. I found that starting with a large learning rate and gradually decreasing it as the neural network improved in accuracy (changing the learning rate from 1 to 0.1) alongside a single batch yielded the best results especially later on in the training procedure. Once the training had been completed the neural network could recognise digits from the test data set correctly 85% of the time.  
  
I downloaded all training and test data used here: https://www.kaggle.com/c/digit-recognizer/data and I researched the theory behind neural networks from 3Blue1Brown's youtube video series (which can be found here: https://www.youtube.com/watch?v=aircAruvnKk&list=PLZHQObOWTQDNU6R1_67000Dx_ZCJB-3pi ).    
  
For completeness I have included both the project associated with training the neural network and the project which allows the user to draw their own handwritten digits onto a window.  
  
Controls (for the digit recogniser program):
-Left Click - Draw white pixels onto the canvas  
-Right Click - Draw black pixels onto the canvas  
-C - Clear the canvas so that all pixels are black  
-ESCAPE - Close the program  
