<?php

	class MockAuth {

		public function getClientId() {
			if(isset($_GET["acc_token"])) {
				return $_GET["acc_token"];
			} else {
				return NULL;
			}
		}

		public function isAdmin() {
			return isset($_GET["admin"]);
		}

		//isGlobal method?
	}

?>  
