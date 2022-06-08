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

## More command line options of application - store
### 1. upgrade
```
store upgrade (optional_argument)
```
Provide optional_argment for not closing the window (Useful for calling from another applications). This argument can be anything like a letter "w" for example.
The upgrade command checks the file size of all the downloaded stores with those available online. If the filesizes do not match, it will re-download the files from the web.
### 2. appupdate
```
store appupdate (optional_argument)
```
Provide optional_argument for not closing the window after operation.
This command causes store.exe to call another application update.exe whose operation is specified below.

## Command Line Options of update.exe
This is an application to check for the updates of store.exe and store-gui.exe from online. If new updates are available, it will download them.
### 1. store/store-gui
```
update [store/store-gui] 
```
If for example, it is called with store-gui argument, then it checks update for application store-gui.exe

## Web
The folder /web contains html and php files. The program store.exe and update.exe sends GET and POST requests to these files when appropriately hosted. 
Note that the file data in folder web must be given permission chmod 777

### POST and GET Requests
```
File: get-store-url.php
Objective: To retrieve the URL of the store of a server, the details which are saved in data file.
Method: POST
Key: server
Value: ip:port
Response: URL on success, or "Error: "

File: file.php
Objective: Add details (Server IP and URL of its store) to the data file.
Method: POST
Key: server
Value: ip:port
Key 2: zipfileurl
Value: url
Response: Done. This will be reflected in the application. 
Response 2: Updated. This will be reflected in the application. (when the server:ip already exists)

File: file.php
Objective: Delete the details of a server.
Method: POST
Key: server
Value: ip-port
Key 2: zipfileurl
Value: delete
Response: Updated. This will be reflected in the application.
Response 2: Server:Port not found in database (on failure)

File: update.php
Objective: To check if update is available either for store.exe or store-gui.exe
Method: GET
Key: current_version
Value: 1000 (first version)
Key 2: app
Value: store (or) store-gui
Response: 1 (Means update available for app), 0 (Already the latest version)
Response (Secondary): Invalid app name ( When Value of Key 2 is neither 'store' nor 'store-gui' )
```

