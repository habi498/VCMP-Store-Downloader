
<?php
$var_datafile="data";
$var_server = $_POST['server'];
$var_zipfileurl = $_POST['zipfileurl'];
if(strlen($var_zipfileurl)==0)
	die("URL of store file incorrect");
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

$var_newline="Server=" . $var_server . ", ZipFile=" . $var_zipfileurl;
//echo "New Line must be " . $var_newline . "\n";
$var_i=strpos($var_value, "Server=" . $var_server);
if($var_i!==false)
{
$var_j=strpos($var_value, ", ZipFile=", $var_i);
$var_line;
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
    $var_newvalue;
	if(strcmp($var_zipfileurl,"delete")===0)
	{
		$var_newline="";
		$var_newvalue=str_replace(
    $var_line,
    $var_newline,
    $var_value);
	//Remove blank spaces
		$var_newvalue=str_replace(
    "\n\n",
    "\n",
    $var_newvalue);
	//Check last line is LF
	$var_x=strpos($var_newvalue, "\n",-1);
	if($var_x!==false)
	{
		//Last line is empty
		$var_newvalue=substr($var_newvalue, 0, -1);
		//echo "Last \\n removed" ;
	}
	//Check first line is LF
	$var_w=strpos($var_newvalue, "\n");
	if($var_w===0)
	{
		//First line is empty
		$var_newvalue=substr($var_newvalue, 1);
		//echo "First L.F removed";
	}
	}else
	{
	//echo "Old line found is " . $var_line;
    $var_newvalue=str_replace(
    $var_line,
    $var_newline,
    $var_value);
	}
    //echo "New value is " . $var_newvalue;
	$var_y=file_put_contents($var_datafile, $var_newvalue);
	if($var_y!==false)
		echo "Updated. This will be reflected in the application.";
	else
		die("An error occured while writing data");
}else echo "Error, file corrupted\n";
}else
{
	if(strcmp($var_zipfileurl,"delete")===0)
	{
		echo "Server:Port not found in database";
	}else
	{
	//This means the server is not found in the textfile. So just append it.
	$myfile = fopen($var_datafile, "a") or die("Unable to open file!");
	if(strlen($var_value)!=0)
		fwrite($myfile, "\n");
	fwrite($myfile, $var_newline);
	fclose($myfile);
	echo "Done. This will be reflected in the application.";
	}
}

}else die("Port not integer. Try again");
}else die("Port not specified. Try again.");

}else die("var_server:Port not in correct format. Try again.");


?>