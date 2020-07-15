# gcsv2h5
C++ program tailored for machine learning applications that converts a graph structured dataset in Comma-Separated Value (CSV) format into an Hierarchical Data Format version 5 (HDF5) file.

The HDF5 format is a format to store large amounts of heterogeneous data in an efficient and organized way. For more information please read: [The HDF5 Data Format](https://www.google.com/search?client=safari&rls=en&q=HDF5&ie=UTF-8&oe=UTF-8)
** **
## Usage

The program assumes a cleaned CSV file with the edgelist data. The program also accepts a second CSV file with nodal features data. The user must call the program using the following command

./gcsv2h5 < Input CSV EdgeList > < Output H5 File > < File Delimiter > < Optional: Input CSV Features > < Optional: Name Ground Truth Column > 

* **Input CSV EdgeList**: Path to edgelist file with format | Source | Target | Weight (optional, if not provided set to 1) | Timestamp (optional, if not provided set to 0) |. See EdgeListSample.txt for an example
* **Ouput H5 File**: Path to where H5 output file will be stored
* **File Delimiter**: Delimiter of input CSV file(s), it can be one of {comma, tab, space}
* **Optional: Input CSV Features**: File with nodal features. Each row is a vector of features associated to a node. One of the columns must correspond to the quantity to predict. First column must contain IDs that match names of vertices in the edgelist. See NodeFeaturesSample.txt for an example
* **Optional: Name Ground Truth Column**: Name of the column in the features file used as ground truth. This parameter is required only if a features file is input. 

The resulting HDF5 will create, in the root group "/", the following
* **Attributes**: "NumberFeatures", "NumberTimestamps", "NumberNodes", and "FeaturesNames" with the header info of the features file if input.
* **Datasets**: For each timestamp, "EdgeList\_stamp" and "Weights\_stamp" datasets are created. Moreover, a dataset called "NameOfNodes" is created containing the vertices names of each H5 file data indices. Lastly, if a features file is input, then datasets called "Features" and "GroundTruth" with their corresponding data are created. 

To test and experiment with the program, try running it with the sample files as follows: ./gcsv EdgeListSample.txt output.h5 comma NodeFeaturesSample.txt ClassMembership
Then, you can navigate the file ouput.h5 using the h5dump tool incided with the HDF5 library. 

** **
## Installation 
The code depends on the **HDF5 Library and the C++ API**. You can install it from [here](https://www.hdfgroup.org/downloads/hdf5/source-code/). For Linux and Mac OS X there are simpler ways using apt-get ([see here](https://launchpad.net/ubuntu/+source/hdf5)) or homebrew ([see here](https://formulae.brew.sh/formula/hdf5)), respectively.  

Then, to compile the code, change, in the makefile, the library and include paths of the HDF5 library to match your installation. Then, simply run the make command in the command line.
