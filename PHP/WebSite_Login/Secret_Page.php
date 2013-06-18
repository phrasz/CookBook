<?php
	session_start();
	
	// !!! NOT "SECURE ENOUGH" !!!
	// Essentially this just checks for the variable to be set to a known "password"
	if(@$_SESSION['auth'] != "yes"){
		header("Location: Login_reg.php");
		exit();
	}
	echo "<head><title>Secret Page</title></head>";
	echo "<body>";
	
	echo "The User ID: {$_SESSION['logname']} , has logged in successfully!";

?>
</body></html>