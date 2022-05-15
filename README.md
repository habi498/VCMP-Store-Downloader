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
The Store-Downloader's masterlist is hosted at and URL's can be added through https://npc-for-vcmp.sourceforge.io/index54.html

The URL should be direct and point to a zip file
eg. ..../store.zip

Store Files Uploaded on formats other than .zip will not work (.rar, etc )

Files uploaded on Mediafire will not work, because on mediafire there is 'Click to Download' and the program cannot do that. 


