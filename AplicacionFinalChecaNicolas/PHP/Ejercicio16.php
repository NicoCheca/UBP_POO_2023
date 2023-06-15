<?php
    
    session_start();
    require 'databasePOO.php';
    
    $records = $conn->prepare('SELECT id,alias,email,password FROM Users WHERE email=:email');
    $records->bindParam(':email', $_GET['email']);
    $records->execute();
    $results = $records->fetch(PDO::FETCH_ASSOC);

    if (isset($_GET['l_search'])) {
        $latest_search = $_GET['l_search'];
    }
    
    if(count($results) > 0 && password_verify($_GET['password'], $results['password'])){
    
        if (isset($latest_search)){
            
            $latest_search_query = $conn->prepare("UPDATE Users SET latestSearch =:latestSearch WHERE email =:email");
            $latest_search_query->bindParam(':email', $_GET['email']);
            $latest_search_query->bindParam(':latestSearch', $latest_search);
            $latest_search_query->execute();
        
        }
        
        $email = $_GET['email'];
        
        $latest_search_get_query = $conn->prepare("SELECT latestSearch FROM Users WHERE email =:email");
        $latest_search_get_query->bindParam(':email', $_GET['email']);
        $latest_search_get_query->execute();
        $resultadosDos = $latest_search_get_query->fetch(PDO::FETCH_ASSOC);
        
        if (!empty($resultadosDos)) {
            echo $resultadosDos['latestSearch'];
        }
        
    }
    else{
        $message = 'Sorry, credentials dont match';
        echo($message);
    }
    
?>

