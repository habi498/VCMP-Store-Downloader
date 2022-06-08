<?php
define("STORE_NEWVERSION", 1000);
define("GUI_NEWVERSION", 1000);
$var_oldver = $_GET['current_version'];
$var_app=$_GET['app'];
if(empty($var_oldver))die("Error, You did not provided your version");
if(empty($var_app))die("Error, You did not provided your app name");
if( is_numeric($var_oldver)===true)
{
	$int_oldver = (int)$var_oldver;
	if(strcmp($var_app,"store")===0)
	{
		if($var_oldver<STORE_NEWVERSION)
			echo "1";
		else
			echo "0";
	}else if(strcmp($var_app,"store-gui")===0)
	{
		if($var_oldver<GUI_NEWVERSION)
			echo "1";
		else
			echo "0";
	}else die("Invalid app name");
}else die("Error, the version you provided must be an integer");
?>