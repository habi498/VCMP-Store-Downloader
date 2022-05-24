# VCMP-Store-Downloader
## Installing using cmake

Download the project, unzip it.
Open cmd and browse to the folder. 
```
mkdir out
cd out
cmake ../
cmake --build . --config Release
```

## Using Store-Downloader
```
store update
store install xxx.xxx.xxx.xxx:PORT
```
(Replace xxx with the server's ip address and PORT by a number)

## Adding URL's to Store-Downloader
The Store-Downloader's masterlist is hosted at and URL's can be added through https://vc-mp-store-downloader.sourceforge.io/index.html

The URL should be direct and point to a zip file
eg. ..../store.zip

Store Files Uploaded on formats other than .zip will not work (.rar, etc )

Files uploaded on Mediafire will not work, because on mediafire there is 'Click to Download' and the program cannot do that. 

## Store - GUI Version ( How to build )
There is a Graphical User Interface version named store-gui.exe through which the user can select and install the stores. It is coded in Autohotkey. The binaries of Autohotkey is provided with the application. So most probably the following will work. 
```
mkdir out
cd out
cmake ../ -DMAKE_GUI=True
cmake --build . --config Release
```
Running the above commands instead of previous one (the one provided in the beginning) will not only create the console application, but creates the GUI-application also. That is the difference.
The sources of this gui-application can be found in the folder "store-gui-src". 
