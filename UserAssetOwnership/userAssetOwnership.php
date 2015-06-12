<?php
include_once("../Core/lux-functions.php");
include_once("../Core/db.php");
include_once("../Core/output.php");
include_once("../Core/auth.php");

class UserAssetOwnership{

	public static function check($asset, $collectionName, $operation){
		// get the Asset's view and edit rights on the Asset
		$OUTPUT = new Output(); 
		$AUTH = new Auth();	
		$DB = new db("Assets");
		$assetCollection = $DB->selectCollection($collectionName);
		$permissions = $assetCollection->findOne(array("_id"=>$asset), array("permissions"=>1));
		if($permissions["2"] == 1){
			return true;
		}else if($permissions["3"] == 1 && $operation == 1){
			return true;
		}else{
			// see if user has view and edit rights on the Asset		
			if($AUTH->isAdmin()){
				return true;
			}else{
				$permissions = $AUTH->getPermissions();
				// check if the asset id is in any of the Permissions arrays
				$permission = 4;
				foreach($permissions as $key => $value){
					if(in_array($asset, $value) && $key < $permission){
						$permission = $key;
					}
				}				
				if($permission < 4){
					return true;
				}else if($permission == 3 && $operation == 1){
					return true;
				}else{
					// Throw an error if they are trying to do something they aren't allowed to
					$OUTPUT->error(3, "User does not have access to alter or view this document");
				}
			}
			
		}
	}
}
  
