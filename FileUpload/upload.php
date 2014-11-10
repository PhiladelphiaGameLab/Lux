<form action="upload.php" method="post" enctype="multipart/form-data">
  Please choose a file: <input type="file" name="uploadFile"><br>
  <input type="submit" value="Upload File">
  <input type="checkbox" name="should_overwrite" value="yes">Overwrite file if it already exists?<br>
</form>

<?php
include_once("../Core/db.php");

/*
 * Returns the name of the base directory in which a file with the
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

$base_upload_dir = "/Users/alexcannon/Sites/Lux/FileUpload/uploads/";
$file_type = $_FILES["uploadFile"]["type"];
$target_dir = $base_upload_dir . get_target_dir_for_file_type($file_type);
$target_path = $target_dir . basename( $_FILES["uploadFile"]["name"]);

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
    if (move_uploaded_file($_FILES["uploadFile"]["tmp_name"], $target_path)) {
        echo "The file ". basename( $_FILES["uploadFile"]["name"]). " has been uploaded.";
    } else {
        echo "Sorry, there was an error uploading your file: ". basename( $_FILES["uploadFile"]["name"]). ".";
        echo "Error: ". $_FILES["uploadFile"]["error"];
    }
}

// TODO: get current user from db, via Oauth?
$db = new Db();
$uploads_collection = $db->selectCollection('uploads');
$uploads_collection->insert(    array(  'timestamp' => time(),
                                        'uploaded_by' => 'John Doe',
                                        'file_path' => $target_path,
                                        'mime_type' => $file_type,
                                        'file_name' => basename( $_FILES['uploadFile']['name'] )));

// TODO: Insert arbitrary input from HTML form?
?>
