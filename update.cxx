#include <string>
#include <conio.h>
using namespace std;
int get_file_size(string filename);
int GetFileSize(const char* szPackageURL);
int DownloadFile(const char* url, const char* pagefilename, int progressbar=1);
int CURLGET(const char* szURL, char* buffer, int len);
const char* szStoreAppURL="https://vc-mp-store-downloader.sourceforge.io/store.exe";
const char* szStoreGUIAppURL="https://vc-mp-store-downloader.sourceforge.io/store-gui.exe";
const char* szUpdateManagerURL="https://vc-mp-store-downloader.sourceforge.io/update.php";
void CheckForUpdate(long version, char* appname, char bRestartApp=0);
int main(int argc, char** argv)
{
	if(argc > 2)
	{
		char* p;
		long version = strtol(argv[2], &p, 10);
		if (*p) {
			printf("Error. Version must be integer");
			return 0;
		}
		if(strcmp(argv[1],"store")==0)
		{
			
			printf("Checking for update for application...\n");
			CheckForUpdate(version, "store");
		}else if(strcmp(argv[1],"store-gui")==0)
		{
			printf("Checking for update for GUI application...\n");
			char bRestartApp=0;
			if(argc>3)
				bRestartApp=1;
			CheckForUpdate(version, "store-gui", bRestartApp);
		}else
		{
			printf("Error: Neither store nor store-gui passed as argument");
			return 0;
		}
		/* if( argc > 3 )
		{
				//wait for user to close the window
				printf("Press any key to continue");
				_getch();
		} */
	}else printf("Usage: %s [store/store-gui] current_version [wait]", argv[0]);
	return 0;
}
void CheckForUpdate(long version, char* appname, char bRestartApp)
{
	char* buffer;
	buffer=(char*)malloc(1024);
	if(buffer)
	{
		string url=string(szUpdateManagerURL);
		url+="?current_version="+to_string(version)+"&app="+string(appname);
		int bSuccess=CURLGET(url.c_str(), buffer, 1024);
		if(bSuccess)
		{
			if(strcmp(buffer,"1")==0)
			{
				printf("App needs update. ");
				string szFile=string(appname)+".exe";
				printf("Killing current %s.exe if any..", appname);
				string szCmd="taskkill /im "+string(appname)+".exe /f";
				system(szCmd.c_str());
				int success;
				if(strcmp(appname, "store")==0)
					success=DownloadFile(szStoreAppURL, szFile.c_str(), 1);
				else if(strcmp(appname, "store-gui")==0)
					success=DownloadFile(szStoreGUIAppURL, szFile.c_str(), 1);
				else
				{
					printf("Invalid app: %s", appname);
					return ;
				}
				if(success)
				{
					printf("Done.");
					if(bRestartApp)
						system(appname);
				}
				else 
					printf("Failed when downloading file..");
			}else if(strcmp(buffer, "0")==0)
				printf("App %s does not need update", appname);
			else printf("Error. %s", buffer);
		}
	}else printf("Memory Allocation failed");
}
/*
void CheckForUpdate()
{
	int iFileSizeLocal=get_file_size("store.exe");
	if(iFileSizeLocal==-1) //store.exe not found
	{
		printf("Error. store.exe not found\n");
		return;
	}
	int iFileSizeOnline=GetFileSize(szStoreAppURL);
	if(iFileSizeOnline!=iFileSizeLocal)
	{
		printf("File size not matching. Update required\n");
		int success=DownloadFile(szStoreAppURL, "store.exe", 1);
		if(success)printf("Done.");else printf("Failed when downloading file..");
	}else printf("store.exe is up to date\n");
}

void CheckForUpdateForGUI()
{
	int iFileSizeLocal=get_file_size("store-gui.exe");
	if(iFileSizeLocal==-1) //store.exe not found
	{
		printf("Error. store-gui.exe not found");
		return;
	}
	int iFileSizeOnline=GetFileSize(szStoreGUIAppURL);
	if(iFileSizeOnline!=iFileSizeLocal)
	{
		printf("File size not matching. Update required\n");
		int success=DownloadFile(szStoreGUIAppURL, "store-gui.exe", 1);
		if(success)printf("Done.");else printf("Failed when downloading file..");
	}else printf("store-gui.exe is up to date");
}
*/