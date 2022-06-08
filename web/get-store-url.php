
<?php
$var_datafile="data";
$var_server = $_POST['server'];

$var_z=strpos($var_server, ":");
if($var_z!==false)
{
if(strlen($var_server)>$var_z+1)
{
$var_port=substr($var_server,$var_z+1);
//echo $var_port;
if( is_numeric($var_port)===true)
{
//echo $var_port . "Is an integer";
$var_value = file_get_contents($var_datafile);
//echo "Original value is " . $var_value;


//echo "New Line must be " . $var_newline . "\n";
$var_i=strpos($var_value, "Server=" . $var_server);
if($var_i!==false)
{
$var_j=strpos($var_value, ", ZipFile=", $var_i);
$var_line;//stores Address
if($var_j!==false)
{
 
  $var_k=strpos($var_value, "\nServer=", $var_j);
  if($var_k===false)
  {
    //this means the line to be replaced is the last line
    $var_line=substr($var_value, $var_i);
  }else 
  {
    $var_line=substr($var_value, $var_i, $var_k-$var_i);
  }
	//echo $var_line;
	//Server=127.0.0.1:8192, ZipFile=https://sourceforge.net/projects/vc-mp-store-downloader/files/store-sample.zip/download//
	$var_z=strpos($var_line, ", ZipFile=");
	if($var_z!==false)
	{
		$var_store_url=substr($var_line, $var_z+strlen(", ZipFile="));
		echo $var_store_url;
	}else die("Error: Unknown");
}else echo "Error: (Fatal) file corrupted\n";
}else
{
	//This means the server is not found in the textfile. 
	die("Error: Server not found");
}

}else die("Error: Port not integer. Try again");
}else die("Error: Port not specified. Try again.");

}else die("Error: Server:Port not in correct format. Try again.");


?>