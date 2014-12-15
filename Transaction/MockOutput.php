<?php

	class MockOutput {

		public function success($message) {
			$response = array(
				"result" => "success",
				"message" => $message);
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