<?php
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Headers: Content-Type, Content-Range, Content-Disposition, Content-Description");

include_once("../Core/auth.php");
$auth = new OAuth("google");



// Authorization file that Completes OAuth based soley on creating a link from a javascript http request and placing it on the page
// Call this file and it first returns the Link, and then the redirect URI links to this page



  
