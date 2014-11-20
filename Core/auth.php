<?php
include_once("lux-functions.php");
include_once("db.php");
include_once("output.php");

class Auth{

	private $client_doc;

	function __construct(){
		// test to make sure that access code is legit
		$LuxFunctions = new LuxFunctions();
		$OUTPUT = new Output(); 
		$access_token = $LuxFunctions->is_avail("access_token");
		$DB = new db();
		$clientInfo = $DB->selectCollection("ClientInfo");
		$this->client_doc = $clientInfo->findOne(array("access_token" => $access_token));
		if(!isset($this->client_doc)){
			$OUTPUT->error("Access Code is invalid or has Expired");
		}
	}
	
	function getClientId(){
		return $this->client_doc["_id"];
	}
	
	function getClientInfo(){
		return $this->client_doc;
	}
	
	function getClientGroups(){
		return $this->client_doc["groups"];
	}
	
	function getClientEmail(){
		return $this->client_doc["email"];
	}

	function getClientName(){
		return $this->client_doc["name"];
	}
}

class AuthLogin{

	private $client_doc;
	
	function __construct($identifier){

		$OUTPUT = new Output();
		$DB = new db();
		$clientInfo = $DB->selectCollection("ClientInfo");
		
		// find user by the identifier
		// if the user is found
		// return the users current access token if one exists
		// otherwise, create a new access token and return that
		// do other stuff
	}


}

class OAuth{

	private $client_id;
	private $client_secret;
	private $redirect_url;
	private $grant_type;
	private $acc_token;
	private $refresh;
	private $DB;
	private $clientInfo;
	private $OUTPUT;
	private $service;

	function __construct($serv){
		$this->OUTPUT = new Output();
                $this->DB = new db();
                $this->clientInfo = $this->DB->selectCollection("ClientInfo");
		$this->service = $serv;
	}

	function check($info){
		$this->redirect_url = $info;
		if(!isset($_GET['code'])){
                        echo $this->getURL();
                }else{
                        $this->saveToDb();
                }
	}

	
	function getURL(){
                switch($this->service){
                        case "Google":
                                $state = 'done';
                                $cli_id = '1006161612314-1qct7m1r0bqt5ecb2sntrci253dv41s1.apps.googleusercontent.com';
                                $call = 'http://'. $_SERVER['HTTP_HOST'] . '/Auth/google.php';
                                $scope = 'email%20profile%20https://www.googleapis.com/auth/admin.directory.user';
                                $url = "https://accounts.google.com/o/oauth2/auth?state=$state&scope=$scope&redirect_uri=$call&response_type=code&client_id=$cli_id&approval_prompt=force&access_type=offline";
                                $this->redirect_url = $_GET["href"];
                                return $url;
                }
        }


	function saveToDb(){
		$code = $_GET['code'];
        	$url = 'https://accounts.google.com/o/oauth2/token';
        	var_dump($this->redirect_url);
		$params = array("code" => $code,
            		"client_id" => "1006161612314-1qct7m1r0bqt5ecb2sntrci253dv41s1.apps.googleusercontent.com",
			"client_secret" => "Uka8meQZbY0KMFCnQ6nYb0Tw",
            		"redirect_uri" => "http://". $_SERVER['HTTP_HOST']. "/".$this->redirect_url,
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
        	$this->acc_token = $access_obj->{'access_token'};
        	$refresh = $access_obj->{'refresh_token'};
        	$getUrl = "https://www.googleapis.com/oauth2/v1/userinfo?access_token=$this->acc_token";
        	$getResponse = file_get_contents($getUrl);
        	$get = json_decode($getResponse, true);
		
		$prevCheck = $this->clientInfo->findOne(array("id" => $get["id"]));
		if(!isset($prevCheck)){
			$get["access_token"] = $this->acc_token;
			$this->clientInfo->insert($get);
		}

		header("Location: http://". $_SERVER['HTTP_HOST']. "/" .$_SESSION['href']."?access_token=".$this->acc_token);
	}

}

?>
