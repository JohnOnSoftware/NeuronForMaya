# Perception Neuron plugin for Maya 2017  

Please refer my 2 blogs of "[How to connect Perception Neuron with Maya](http://around-the-corner.typepad.com/adn/2015/10/how-to-connect-perception-neuron-with-maya-part-1.html)" for 
all the details. 


## Setup
- Build the plugin with Maya 2017, VS 2012.
- Copy the corresponding NeuronDataReader.dll under \\NeuronForMaya\Neuron SDK\lib\x64 to Maya binary file, or the plugin will be failed to load. 
- Load the NeuronForMaya.mll plugin.
- Open the Axis Neruon application, and set up the environment, please refer the blog post 1 for detail.
- Load and run NeuronForMaya.Mel.
- Turn on the live checkbox of mySkeleton_Device, you will see the skeleton is animated.

## Written By
Zhong Wu (Forge Partner Development)
