<?php
	//Connect
	$db = new PDO('sqlite:users.db');
	
	// !!! SECURITY WARNING: LACKS FILTERING !!!
	$sth = $db->prepare('UPDATE users SET verified = 1 WHERE email =? '. 'AND verify_string = ? AND verified =0');

	$res = $sth->execute(array($_GET['email'], $_GET['verify_string']));
	var_dump($res, $sth->rowCount());
	if(! $res){
		print "Server Connection Error! Please try again later!";
	}
	else{
		if($sth->rowCount() == 1){
			print "Thank you, your account is verified!";
		}
		else{
			print "Soory, you could not be verified!";
		}
	}
	
	
?>