<form action="upload.php" method="post" enctype="multipart/form-data">
  Please choose a file: <input type="file" name="uploadFile"><br>
  <input type="submit" value="Upload File">
</form>

<?php
$target_dir = "/Users/alexcannon/Sites/Lux/FileUpload/uploads/";
$target_dir = $target_dir . basename( $_FILES["uploadFile"]["name"]);
$uploadOk=1;

// Check if file already exists
if (file_exists($target_dir . $_FILES["uploadFile"]["name"])) {
    echo "Sorry, file already exists.";
    $uploadOk = 0;
}

// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0) {
    echo "Sorry, your file was not uploaded.";
// if everything is ok, try to upload file
} else { 
    if (move_uploaded_file($_FILES["uploadFile"]["tmp_name"], $target_dir)) {
        echo "The file ". basename( $_FILES["uploadFile"]["name"]). " has been uploaded.";
    } else {
        echo "Sorry, there was an error uploading your file: ". basename( $_FILES["uploadFile"]["name"]). ".";
        echo "Error: ". $_FILES["uploadFile"]["error"];
    }
}
?>
