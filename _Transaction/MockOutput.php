<?php

	class MockOutput {

		public function success($message, $added, $removed) {
			$response = array(
				"result" => "success",
				"message" => $message);
			if($added != NULL) {
				$response["added"] = $added;
			}
			if($removed != NULL) {
				$response["removed"] = $removed;
			}
			echo json_encode($response);
		}

		public function failure($message) {
			$response = array(
				"result" => "failure",
				"message" => $message);
			echo json_encode($response);
		}
	}

?>  
