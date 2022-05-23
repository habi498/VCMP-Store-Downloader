#include <curl.h>
#include <math.h>
#include <string>
#include <cerrno>
#include <cstdlib>
using namespace std;
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
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
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
int get_file_size(std::string filename) // path to file
{
    FILE *p_file = NULL;
    p_file = fopen(filename.c_str(),"rb");
    if(p_file)
	{
		fseek(p_file,0,SEEK_END);
		int size = ftell(p_file);
		fclose(p_file);
		return size;
	}
	return -1;
}
struct MemoryStruct {
  char *memory;
  size_t size;
};
 
static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  char *ptr = (char*)realloc(mem->memory, mem->size + realsize + 1);
  if(!ptr) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}
int CURLGET(const char* szURL, char* buffer, int len)
{
	int bSuccess=0;
	CURL *curl;
  CURLcode res;
 
  struct MemoryStruct chunk;
 
  chunk.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
  chunk.size = 0;    /* no data at this point */
 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* init the curl session */
  curl = curl_easy_init();
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
  /* specify URL to get */
  curl_easy_setopt(curl, CURLOPT_URL, szURL);
  
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
 
  /* send all data to this function  */
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
 
  /* we pass our 'chunk' struct to the callback function */
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
 
  /* some servers do not like requests that are made without a user-agent
     field, so we provide one */
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
 
//   curl_easy_setopt(curl, CURLOPT_HEADER, 1); 
  // curl_easy_setopt(curl, CURLOPT_NOBODY, 1); 
  /* get it! */
  res = curl_easy_perform(curl);
 
  /* check for errors */
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }
  else {
    /*
     * Now, our chunk.memory points to a memory block that is chunk.size
     * bytes big and contains the remote file.
     *
     * Do something nice with it!
     */
	if(len >chunk.size)
	{
		strncpy ( buffer, chunk.memory, chunk.size );
		*(buffer+chunk.size)='\0';
		bSuccess=1;
	}
	
  }
  /* cleanup curl stuff */
  curl_easy_cleanup(curl);
 
  free(chunk.memory);
 
  /* we are done with libcurl, so clean it up */
  curl_global_cleanup();
  return bSuccess;
	
}
int GetFileSize(const char* szPackageURL)
{
	int iFileSize=0;
	CURL *curl;
  CURLcode res;
 
  struct MemoryStruct chunk;
 
  chunk.memory = (char*)malloc(1);  /* will be grown as needed by the realloc above */
  chunk.size = 0;    /* no data at this point */
 
  curl_global_init(CURL_GLOBAL_ALL);
 
  /* init the curl session */
  curl = curl_easy_init();
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
  /* specify URL to get */
  curl_easy_setopt(curl, CURLOPT_URL, szPackageURL);
  
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
 
  /* send all data to this function  */
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
 
  /* we pass our 'chunk' struct to the callback function */
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
 
  /* some servers do not like requests that are made without a user-agent
     field, so we provide one */
  curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
 
   curl_easy_setopt(curl, CURLOPT_HEADER, 1); 
   curl_easy_setopt(curl, CURLOPT_NOBODY, 1); 
  /* get it! */
  res = curl_easy_perform(curl);
 
  /* check for errors */
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
  }
  else {
    /*
     * Now, our chunk.memory points to a memory block that is chunk.size
     * bytes big and contains the remote file.
     *
     * Do something nice with it!
     */
 
    //printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
	string szmemory=string(chunk.memory);
	//printf("File chunk is %s\n", szmemory.c_str());
	size_t dw_indexOfContLen1=szmemory.rfind("content-length:");
	size_t dw_indexOfContLen2=szmemory.rfind("Content-Length:");
	size_t dw_indexOfContLen;
	if(dw_indexOfContLen1==string::npos && dw_indexOfContLen2==string::npos)
	{
		goto end;
	}else if(dw_indexOfContLen1!=string::npos && dw_indexOfContLen2!=string::npos)
	{
		dw_indexOfContLen=dw_indexOfContLen1>dw_indexOfContLen2?dw_indexOfContLen1:dw_indexOfContLen2;
	}else if(dw_indexOfContLen1==string::npos)
	{
		dw_indexOfContLen=dw_indexOfContLen2;
	}else dw_indexOfContLen=dw_indexOfContLen1;
	//printf("indexof ContLen is %zu\n", dw_indexOfContLen);
	size_t dw_indexOfLF=szmemory.find("\r\n", dw_indexOfContLen);
	//printf("Index of LF is %zu\n", dw_indexOfLF);
	//printf("%s", szmemory.substr(dw_indexOfContLen, 10).c_str());
	if(dw_indexOfLF!=string::npos)
	{
		dw_indexOfContLen+=string("content-length:").length();
		string szSizeOfFile=szmemory.substr(dw_indexOfContLen, dw_indexOfLF-dw_indexOfContLen);
		//printf("The size of file is %s\n", szSizeOfFile.c_str());
		errno = 0;
        char* p_end;
        const long i = std::strtol(szSizeOfFile.c_str(), &p_end, 10);
        const bool range_error = errno == ERANGE;
		if(range_error)goto end;
		iFileSize=(int)i;
	}
  }
end:
  /* cleanup curl stuff */
  curl_easy_cleanup(curl);
 
  free(chunk.memory);
 
  /* we are done with libcurl, so clean it up */
  curl_global_cleanup();
	
  return iFileSize;
}
