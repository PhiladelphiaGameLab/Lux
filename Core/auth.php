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
		$access_token = $LuxFunctions->fetch_avail("access_token");
		$DB = new db("System");
		$clientInfo = $DB->selectCollection("Users");
		$this->client_doc = $clientInfo->findOne(array("lux_info.access_token" => $access_token));
		if(!isset($this->client_doc)){
			$OUTPUT->error(1,"Access Code is invalid, missing, or has Expired");
		}
		if($this->isAdmin() && $LuxFunctions->is_avail("uid")){
			$this->client_doc = $clientInfo->findOne(array("_id" => new MongoId($LuxFunctions->fetch_avail("uid"))));
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
	function getPermissions(){
		return $this->client_doc["permissions"];
	}	
	function isAdmin($dept="all"){
		switch($dept){
			case "all":
				return $this->client_doc["lux_info"]["admin"];
			default:
				return $this->client_doc["lux_info"]["admin_depts"][$dept];
		}
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
		$LF = new LuxFunctions();
		$this->OUTPUT = new Output();
                $this->DB = new db();
                if(!is_array($_SESSION)){
			session_start();
		}
		$this->clientInfo = $this->DB->selectCollection("Users");
		$this->service = $serv;
		$_SESSION["href"] = $LF->fetch_avail("href");
		$this->check($LF->fetch_avail("href"));
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
                        case "google":
                                $state = 'done';
                                $cli_id = '1006161612314-1qct7m1r0bqt5ecb2sntrci253dv41s1.apps.googleusercontent.com';
                                $call = 'http://'. $_SERVER['HTTP_HOST'] .'/Auth/google.php';
                                $scope = 'email%20profile%20https://www.googleapis.com/auth/admin.directory.user';
                                $url = "https://accounts.google.com/o/oauth2/auth?state=$state&scope=$scope&redirect_uri=$call&response_type=code&client_id=$cli_id&approval_prompt=force&access_type=offline";
                                //$this->redirect_url = $_GET["href"];
                                return $url;
                }
        }


	function saveToDb(){
		$code = $_GET['code'];
        	$url = 'https://accounts.google.com/o/oauth2/token';
		$params = array(
			"code" => $code,
			"client_id" => "1006161612314-1qct7m1r0bqt5ecb2sntrci253dv41s1.apps.googleusercontent.com",
			"client_secret" => "Uka8meQZbY0KMFCnQ6nYb0Tw",
			"redirect_uri" => "http://".$_SERVER["HTTP_HOST"]."/Auth/google.php",
			"grant_type" => "authorization_code"
		);
		//var_dump($params);
		
		$curl = curl_init($url);
		curl_setopt($curl, CURLOPT_POST, true);
		curl_setopt($curl, CURLOPT_POSTFIELDS, $params);
		curl_setopt($curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
	        curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, false);
    		curl_setopt($curl, CURLOPT_RETURNTRANSFER, 1);
		
		$json_response = curl_exec($curl);
		$authObj = json_decode($json_response);
		$access_tok = $authObj->{'access_token'};
        	$getUrl = "https://www.googleapis.com/oauth2/v1/userinfo?access_token=".$access_tok;
                $getResponse = file_get_contents($getUrl);
                $get = json_decode($getResponse, true);
		//var_dump($get);

	//	$prevCheck = $this->clientInfo->findOne(array("id" => $get["id"]));

		$get["lux_info.access_token"] = $access_tok;
		$results = $this->clientInfo->update(array("id" => $get["id"]), array('$set' => array("lux_info.access_token"=>$access_tok, $values=>$get)), array("upsert" => true));
	/*	if(!isset($prevCheck)){
			$get["access_token"] = $this->acc_token;
			$this->clientInfo->insert($get);
		}else{

		} */
		echo var_dump($results);
		//echo "<br/><br/>redirect url: ".$this->redirect_url;
		//echo "<br/><br/>Location: http://". $_SERVER['HTTP_HOST']. "/" .$this->redirect_url."?access_token=".$access_tok;
		header("Location: http://". $_SERVER['HTTP_HOST']. "/" .$this->redirect_url."?access_token=".$access_tok);
		die();
	}

}

?>
