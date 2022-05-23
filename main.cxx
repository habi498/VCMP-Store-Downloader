#include <string>
#include <fstream>
#include <ctime>
#include <curl.h>

#include <conio.h>

using namespace std;
#define MAX_SERVERS 1024
#define VERSION 1000
int DownloadFile(const char* url, const char* pagefilename, int progressbar=1);
unsigned char DoUpgrade(string szServerIP, string szPackageURL);
unsigned char DownloadAndInstallPackage(string szServerIP, string szPackageURL);
int GetFileSize(const char* szPackageURL);
int get_file_size(std::string filename); // path to file
string szDataFilePath="https://vc-mp-store-downloader.sourceforge.io/data";

string szLocalDataFile="data";
string szfileParam1="Server=";
string szfileParam2=", ZipFile=";
int DoUpdate()
{
	string szfileParam1="Server=";
	string szfileParam2=", ZipFile=";
	time_t now = time(nullptr);
	string ts = to_string(now);
	szDataFilePath=szDataFilePath+string("?ts=")+ts;
	string path=szLocalDataFile;
	int success=DownloadFile(szDataFilePath.c_str(), path.c_str(),0);
	return success;
}
int main(int argc, char** argv)
{
	if(argc > 1)
	{
		if(strcmp(argv[1],"update")==0)
		{
			int success=DoUpdate();
			if(success)printf("Success");
			else printf("Failed");
			return 0;
		}else if(strcmp(argv[1],"upgrade")==0)
		{
			printf("Updating from Masterlist...");
			DoUpdate();
			printf("Done\n");
			string szServerIPArray[MAX_SERVERS];//Store 1024 Server IP
			string szStoreURLArray[MAX_SERVERS];
			unsigned short iServerCount=0;
			string line;
				ifstream myfile(szLocalDataFile.c_str());
				if (myfile.is_open())
				{
					bool bPackageFound=false;
					printf("Determining which packages need upgrades...");
					while ( getline (myfile,line) )
					{
						size_t dw_indexOfServer=line.find(szfileParam1);
						if(dw_indexOfServer!=string::npos)
						{
							dw_indexOfServer+=szfileParam1.length(); //to obtain server ip next
							size_t dw_indexOfURL=line.find(szfileParam2);
							if( dw_indexOfURL != string::npos )
							{
								string szServerIP=line.substr(dw_indexOfServer, dw_indexOfURL-dw_indexOfServer);
								dw_indexOfURL+=szfileParam2.length();
								string szPackageURL=line.substr(dw_indexOfURL);
								if(DoUpgrade(szServerIP, szPackageURL))
								{
									szServerIPArray[iServerCount]=szServerIP;
									szStoreURLArray[iServerCount]=szPackageURL;
									iServerCount++;
								}
							}
						}
					}
					printf("Done\n");
					myfile.close();
					if(iServerCount>0)
					{
						printf("%d packages needs upgrades based on check by file size. They are ", iServerCount);
						string Packages="";
						for(int i=0; i<iServerCount; i++)
						{
							Packages+=szServerIPArray[i]+"\t";
						}
						printf("%s\n.", Packages.c_str());
						for(int i=0; i<iServerCount; i++)
						{
							printf("Installing package %d of %d\n", i+1, iServerCount);
							printf("Name: %s Location: %s\n", szServerIPArray[i].c_str(), szStoreURLArray[i].c_str());
							unsigned char bSuccess=DownloadAndInstallPackage(szServerIPArray[i], szStoreURLArray[i]);
						}
					}else printf("All packages are in par with the latest of the stores available\n");
				}else printf("Unable to search for packages");
				if( argc > 2 )
				{
					//wait for user to close the window
					printf("Press any key to continue");
					_getch();
				}
		}else if(strcmp(argv[1],"install")==0)
		{
			if(argc > 2)
			{
				string line;
				ifstream myfile(szLocalDataFile.c_str());
				if (myfile.is_open())
				{
					bool bPackageFound=false;
					while ( getline (myfile,line) )
					{
						string szSearchParam=szfileParam1+string(argv[2])+string(szfileParam2);
						if(line.find(szSearchParam)!=string::npos)
						{
							printf("Package %s \n", argv[2]);
							bPackageFound=true;
							
							
							size_t dw_indexOfURL=line.find(szfileParam2);
							if( dw_indexOfURL != string::npos )
							{
								dw_indexOfURL+=szfileParam2.length();
								string szPackageURL=line.substr(dw_indexOfURL);
								printf("Package URL is %s\n", szPackageURL.c_str());
								unsigned char bSuccess=DownloadAndInstallPackage(string(argv[2]), szPackageURL);
								if(bSuccess)
								{
									if (argc > 3)
									{
										//wait for user to close the window
										printf("Press any key to continue");
										_getch();
									}
								}
							}
								
						}
						
					}
					myfile.close();
					if(!bPackageFound)
						printf("Package %s not found.", argv[2]);
				}else printf("Unable to search for packages");
				
			}else printf("Package name not specified\n");
			return 0;
		}else if(strcmp(argv[1],"appupdate")==0)
		{
			string szCmd = "update.exe store "+to_string(VERSION);
			if(argc >2 )
				szCmd+=" "+string(argv[2]);
			system(szCmd.c_str());
		}else printf("Invalid argument %s", argv[1]);
	}else printf("No argument passed. Use %s update or install", argv[0]);
}
#define SKIP_PEER_VERIFICATION



unsigned char DoUpgrade(string szServerIP, string szPackageURL)
{
	string szFileName=szServerIP;
	size_t dw_indexOfColon = szFileName.find(":");
	if (dw_indexOfColon != string::npos)
	{
		szFileName.replace(dw_indexOfColon, 1, "-");
	}
	int iFileSize=get_file_size("downloads/"+szFileName);
	if(iFileSize==-1)
	{
		//File does not exist
		return 1;
	}
	if(iFileSize==0)return 1;//precaution
	//curl checks for file, header etc here
	int iFileSizeNew=GetFileSize(szPackageURL.c_str());
	if(iFileSizeNew==0)
	{
		//cannot determine new file size
		return 1;
	}
	if(iFileSize==iFileSizeNew)
	{
		//conclude it is same file
		//so return 0
		return 0;
	}
	return 1;
}
unsigned char DownloadAndInstallPackage(string szServerIP, string szPackageURL)
{
	string szFileName=szServerIP;
	size_t dw_indexOfColon = szFileName.find(":");
	if (dw_indexOfColon != string::npos)
	{
		szFileName.replace(dw_indexOfColon, 1, "-");
	}
	if (CreateDirectory("downloads", NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{
		string path = "downloads/" + szFileName;
		int success = DownloadFile(szPackageURL.c_str(), path.c_str());
		if (success)
		{
			printf("Download success\n");
			std::ifstream input(path.c_str(), std::ios::binary);
			if (input.is_open())
			{
				char* memblock = new  char[4];
				input.read(memblock, 4);
				input.close();
				if (memblock[0] == 0x50 && memblock[1] == 0x4b &&
					memblock[2] == 0x03 && memblock[3] == 0x04)
				{
					//It is a zip file. 
					/*
					According to http://www.pkware.com/documents/casestudies/APPNOTE.TXT, a ZIP file starts with the "local file header signature"
	0x50, 0x4b, 0x03, 0x04
	and https://stackoverflow.com/questions/18194688/how-can-i-determine-if-a-file-is-a-zip-file*/
					printf("Success: File is a zip file. Now unzipping to VCMP local store folder\n");
					string szCmd2 = "unzip -o downloads/" + szFileName + " -d \"C:/Users/%username%/AppData/Roaming/VCMP/04beta/store/" + szFileName + "\"";
					system(szCmd2.c_str());
					return 1;
					
				}
				else
				{
					printf("Failed. File is not a zip file\n");
					//delete it
					string szCmd3 = "del \"downloads\\" + szFileName + "\"";
					system(szCmd3.c_str());
				}
				delete[] memblock;
			}
		}
		else printf("Failed");
	}
	else
	{
		printf("Failed to create folder");
	}
	return 0;
}
