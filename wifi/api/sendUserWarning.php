<?php

    $url = 'https://fcm.googleapis.com/fcm/send';

    $channel = $_POST['channel'];

    // $notification = array("title" => "Warning from SmartBasha!", "body" => "Your light is on!!");

    $dataArray = array(
        "title" => "Warning from SmartBasha!",
        "body" => "Your light is on!!",
        "channel" => $channel
    );

    $fields = array(
        'to' => '/topics/smartBasha', 
        'priority' => 'high', 
        'data' => $dataArray
        // 'notification' => $notification
    ); 
    $headers = array(
        'Authorization:key = AIzaSyBuD0istWuxO3gxqr2qe44k2dzcHG_zd1k', 
        'Content-Type: application/json'
    ); 
   $ch = curl_init(); 
   curl_setopt($ch, CURLOPT_URL, $url); 
   curl_setopt($ch, CURLOPT_POST, true); 
   curl_setopt($ch, CURLOPT_HTTPHEADER, $headers); 
   curl_setopt($ch, CURLOPT_RETURNTRANSFER, true); 
   curl_setopt ($ch, CURLOPT_SSL_VERIFYHOST, 0); 
   curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false); 
   curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($fields)); 
//    echo "\n" . json_encode($fields); 
   $result = curl_exec($ch); 
//    echo "\n" . $result; 
   if ($result === FALSE) {
       die('Curl failed: ' . curl_error($ch)); 
   }
   curl_close($ch); 
   echo $result;