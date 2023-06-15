<?php

$DB_conn = mysqli_connect(
  $host = "localhost:3306",
  $dbname = "whatsyourmovie_DataBase",
  $username = "whatsyourmovie_nico",
  $password = "NIC839ALE328"

);

try {
    $conn = new PDO("mysql:host=$host;dbname=$dbname;" ,$username, $password);
    }
    catch(PDOException $e){
        die('Connection failed'.$e->getMessage());
    }

//$DB_conn = mysqli_connect($host, $username, $password, $dbname);

//Consultar sino da error la conexión a la base
//if(!$DB_conn) {
//    echo "Error: No se pudo conectar a MySQL." . NEWLINE;
//    echo "Nro de error: " . mysqli_connect_errno() . NEWLINE;
//    echo "Mensaje de depuración: " . mysqli_connect_error() . NEWLINE;
//    exit;
//}


?>