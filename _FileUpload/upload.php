<form action="upload.php" method="post" enctype="multipart/form-data">
  Please choose a file: <input type="file" name="upload_file"><br>
  <input type="submit" value="Upload File">
  Rename file to...: (optional):<br>
  <input type="text" name="new_name">
</form>

<?php
include_once("../Core/db.php");
include_once("../Core/auth.php");
    
/*
 * Returns the name of the sub-directory in which a file with the
 * given file type should be stored.
 *
 * @param   file_type the type of the file, a string
 * @returns 'images', 'video', 'audio', 'text', or 'etc'
 */
function get_target_dir_for_file_type($file_type) {
    $regex_filetype_patterns = array(
                                'images' => '/^image/',
                                'video' => '/^video/',
                                'audio' => '/^audio/',
                                'text' => '/^text/'
                            );

    foreach( $regex_filetype_patterns as $dir => $regex ) {
        if (preg_match($regex, $file_type)) {
            return $dir . "/";
        }
    }
    return 'etc/';
}

$db = new Db();
$auth = new Auth();

$uploads_collection = $db->selectCollection('uploads');
$client_id = $auth->getClientId();

$base_upload_dir = "/Users/alexcannon/Sites/Lux/FileUpload/uploads/";
$file_type = $_FILES["upload_file"]["type"];
$target_dir = $base_upload_dir . get_target_dir_for_file_type($file_type);

// TODO: Insert filename, filetype, directory_path, userid, timestamp into db 
$insert_array = array(  'client_id' => $client_id,
                        'location' => $target_dir,
                        'file_type' => $file_type,
                        'timestamp' => time()
                    );

$uploads_collection->insert($insert_array);
$target_path = $target_dir . $insert_array['_id'] . pathinfo($_FILES["upload_file"]["name"], PATHINFO_EXTENSION);

echo "Target path: " . $target_path;

// Create target dir if needed
if (!file_exists($target_dir)) {
    mkdir($target_dir, 0777, true);
}

$uploadOk=1;

// Check if file already exists
if (!isset($_POST['should_overwrite'])) {
    echo "overwrite not set";
    if (file_exists($target_path)) {
        echo "Sorry, file already exists.";
        $uploadOk = 0;
    }
}
// TODO: Perform other checks? e.g. file size?

// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0) {
    echo "Sorry, your file was not uploaded.";
// if everything is ok, try to upload file
} else { 
    if (move_uploaded_file($_FILES["upload_file"]["tmp_name"], $target_path)) {
        echo "The file ". basename( $_FILES["upload_file"]["name"]). " has been uploaded.";
    } else {
        echo "Sorry, there was an error uploading your file: ". basename( $_FILES["upload_file"]["name"]). ".";
        echo "Error: ". $_FILES["upload_file"]["error"];
    }
}

// TODO: get current user from db, via Oauth?
$db = new Db();
$uploads_collection = $db->selectCollection('uploads');
$uploads_collection->insert(    array(  'timestamp' => time(),
                                        'uploaded_by' => 'John Doe',
                                        'file_path' => $target_path,
                                        'mime_type' => $file_type,
                                        'file_name' => basename( $_FILES['upload_file']['name'] )));

// TODO: Insert arbitrary input from HTML form?
?>
  
