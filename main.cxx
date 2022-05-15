#include "main.h"
#include <string>
#include <fstream>
#include <ctime>
#include <curl.h>
#include <math.h>

using namespace std;
int DownloadFile(const char* url, const char* pagefilename, int progressbar=1);
//string szDataFilePath="https://raw.githubusercontent.com/habi498/VCMP-Store-Downloader/main/data";
string szDataFilePath="https://npc-for-vcmp.sourceforge.io/data";

string szLocalDataFile="data";
string szfileParam1="Server=";
string szfileParam2=", ZipFile=";
int main(int argc, char** argv)
{
	string szfileParam1="Server=";
	string szfileParam2=", ZipFile=";
	if(argc > 1)
	{
		if(strcmp(argv[1],"update")==0)
		{
			time_t now = time(nullptr);
			string ts = to_string(now);
			szDataFilePath=szDataFilePath+string("?ts=")+ts;
			string path=szLocalDataFile;
			int success=DownloadFile(szDataFilePath.c_str(), path.c_str(),0);
			if(success)printf("Success");else printf("Failed");
			//string szCmd="curl "+szDataFilePath+" --output "+path;
			//printf("szCmd is %s\n",szCmd.c_str());
			///system(szCmd.c_str());
			return 0;
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
								string szFileName=string(argv[2]);
								size_t dw_indexOfColon=szFileName.find(":");
								if(dw_indexOfColon!=string::npos)
								{
									szFileName.replace(dw_indexOfColon,1,"-");
								}
								if (CreateDirectory("downloads", NULL) ||
									ERROR_ALREADY_EXISTS == GetLastError())
								{
									string path="downloads/"+szFileName;
									int success=DownloadFile(szPackageURL.c_str(), path.c_str());
									if(success)
									{
									printf("Success. Now unzipping\n");
									std::ifstream input( path.c_str(), std::ios::binary );
									if (input.is_open())
									{	
										char* memblock = new  char [4];
										input.read (memblock, 4);
										if(memblock[0]==0x50 && memblock[1]==0x4b &&
										memblock[2]==0x03 && memblock[3]==0x04)
										{
											//It is a zip file. 
											/*
											According to http://www.pkware.com/documents/casestudies/APPNOTE.TXT, a ZIP file starts with the "local file header signature"
							0x50, 0x4b, 0x03, 0x04 
							and https://stackoverflow.com/questions/18194688/how-can-i-determine-if-a-file-is-a-zip-file*/
										string szCmd2="unzip -o downloads/"+szFileName+" -d \"C:/Users/%username%/AppData/Roaming/VCMP/04beta/store/"+szFileName+"\"";
										system(szCmd2.c_str());
										}else printf("Failed. File is not a zip file");
									}
									}else printf("Failed");
								}
								else
								{
									 printf("Failed to create folder");
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
		}
	}
}
#define SKIP_PEER_VERIFICATION
 static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}
/* Credits: fvu @ stackoverflow.com
https://stackoverflow.com/questions/1637587/c-libcurl-console-progress-bar
*/
int progress_func(void* ptr, double TotalToDownload, double NowDownloaded, 
                    double TotalToUpload, double NowUploaded)
{
    // ensure that the file to be downloaded is not empty
    // because that would cause a division by zero error later on
    if (TotalToDownload <= 0.0) {
        return 0;
    }

    // how wide you want the progress meter to be
    int totaldotz=40;
    double fractiondownloaded = NowDownloaded / TotalToDownload;
    // part of the progressmeter that's already "full"
    int dotz = (int) round(fractiondownloaded * totaldotz);

    // create the "meter"
    int ii=0;
    printf("%3.0f%% [",fractiondownloaded*100);
    // part  that's full already
    for ( ; ii < dotz;ii++) {
        printf("=");
    }
    // remaining part (spaces)
    for ( ; ii < totaldotz;ii++) {
        printf(" ");
    }
    // and back to line begin - do not forget the fflush to avoid output buffering problems!
    printf("]\r");
    fflush(stdout);
    // if you don't return 0, the transfer will be aborted - see the documentation
    return 0; 
}
int DownloadFile(const char* url, const char* pagefilename, int progressbar)
{
  CURL *curl;
  CURLcode res;
  FILE *pagefile;
  curl_global_init(CURL_GLOBAL_DEFAULT);
  int success=0;
  curl = curl_easy_init();
  if(curl) { 
    curl_easy_setopt(curl, CURLOPT_URL, url);
 
#ifdef SKIP_PEER_VERIFICATION
    /*
     * If you want to connect to a site who is not using a certificate that is
     * signed by one of the certs in the CA bundle you have, you can skip the
     * verification of the server's certificate. This makes the connection
     * A LOT LESS SECURE.
     *
     * If you have a CA cert for the server stored someplace else than in the
     * default bundle, then the CURLOPT_CAPATH option might come handy for
     * you.
     */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif
 
#ifdef SKIP_HOSTNAME_VERIFICATION
    /*
     * If the site you are connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl will refuse to connect. You can skip
     * this check, but this will make the connection less secure.
     */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
 
     /* send all data to this function  */
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
 
  /* open the file */
  pagefile = fopen(pagefilename, "wb");
  if(pagefile) {
 
    /* write the page body to this file handle */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, pagefile);
 
	if(progressbar)
	{
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
		curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_func); 
	}
    /* get it! */
    res = curl_easy_perform(curl);
 
    /* close the header file */
    fclose(pagefile);
  }
 
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
	else success=1;
 
    /* always cleanup */
    curl_easy_cleanup(curl);
  }
 
  curl_global_cleanup();
 
  return success;
}