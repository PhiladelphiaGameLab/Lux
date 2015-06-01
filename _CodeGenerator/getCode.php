<?php
$chars = "abcdef0ghijk1lmno2pqrstu3vwxyz4ABCD5EFGH6IJK7LM8NOP9QRSTUVWXZ"; 
$i = 0; 
$pass = '' ; 

for($i=0; $i <= 5; $i++) {
        $tmp = substr($chars, rand()%62, 1); 
        $pass .= substr($chars, rand()%62, 1); 
}
echo $pass;

?>

  
