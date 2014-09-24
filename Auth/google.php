<?php
// Authorization file that Completes OAuth based soley on creating a link from a javascript http request and placing it on the page
// Call this file and it first returns the Link, and then the redirect URI links to this page

//require('/var/www/Lux2/core/db.php');


session_start();

// this creates the button and passes it back to the javascript
if ( !isset($_GET['code'])) {
        //the first call to google, this recieves the code to be used later
        $state = 'done';
        $cli_id = '1006161612314-1qct7m1r0bqt5ecb2sntrci253dv41s1.apps.googleusercontent.com';
        $call = 'http://'. $_SERVER['HTTP_HOST'] . '/Auth/google.php';
        $scope = 'email%20profile%20https://www.googleapis.com/auth/admin.directory.user';
        $url = "https://accounts.google.com/o/oauth2/auth?state=$state&scope=$scope&redirect_uri=$call&response_type=code&client_id=$cli_id&approval_prompt=force&access_type=offline";
      	echo $url;
	if(isset($_GET['href'])){
		$_SESSION['href'] = $_GET['href'];
	}else{
		$_SESSION['href'] = "loggedin2.html";
	}
}else{
// This completes the rest of the OAuth processes when the button is clicked

        //use the code in the url to get the access token try to get an access token
        $code = $_GET['code'];
        $url = 'https://accounts.google.com/o/oauth2/token';
        $params = array("code" => $code,
// read these from a config file from the server?
            "client_id" => "1006161612314-1qct7m1r0bqt5ecb2sntrci253dv41s1.apps.googleusercontent.com",
            "client_secret" => "Uka8meQZbY0KMFCnQ6nYb0Tw",
            "redirect_uri" => "http://" . $_SERVER['HTTP_HOST'] . "/Auth/google.php",
            "grant_type" => "authorization_code"
        );
        $options = array(
            'http' => array(
                'header'  => "Content-type: application/x-www-form-urlencoded\r\n",
                'method'  => 'POST',
                'content' => http_build_query($params),
            ),
        );
        $context = stream_context_create($options);
        $result = file_get_contents($url, false, $context);

        $access_obj = json_decode($result);
        $_SESSION['access'] = $access_obj->{'access_token'};
        //get the refresh token, used in refresh.php
        $_SESSION['refresh'] = $access_obj->{'refresh_token'};


        //get the user's information from the access_token
        $ac_tok = $_SESSION['access'];
        $getUrl = "https://www.googleapis.com/oauth2/v1/userinfo?access_token=$ac_tok";
        $getResponse = file_get_contents($getUrl);
        $get = json_decode($getResponse);
        $email = $get->{'email'};
	
	// just gets the user's email for now and counts that as being enough information for everything else

	// search for document where email:$email

	// if does not exist- create document where email:$email
	// Put timestamp of last update to information as Now?
	// insert other things about the person (name, last name, whatever else google will give to us). 
	

	// upsert into mongo as either a new User or An existing User


	// This SHOULD NOT BE HARD CODED, WE SHOULD BE ABLE TO PASS THE SESSION VARIABLE 
        //header("Location: http://ec2-54-88-133-189.compute-1.amazonaws.com/loggedin.html ");
        header("Location: http://". $_SERVER['HTTP_HOST']. "/" .$_SESSION['href']."?acc_token=".$ac_tok);
}
  

