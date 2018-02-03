<?php

require_once "../include/config.php";

if (isset($_POST['submit'])) {
    $wifiID = $_POST['wifiID'];
    $wifiLocalIpAddress = $_POST['wifiLocalIpAddress'];

    // $query = "INSERT INTO ";

    echo "\nIP address: " . $wifiLocalIpAddress . "\n";
    
    // $ch = curl_init("http://192.168.0.102:105");
    // $ch = curl_init("http://192.168.0.102:105/blink");
    // $ch = curl_init("http://192.168.0.102:105/channels");
    // curl_setopt_array($ch, array(
    //     CURLOPT_RETURNTRANSFER => 1,
    //     CURLOPT_POST => 1,
    //     CURLOPT_POSTFIELDS => "data=" . json_encode($channel),
    //     CURLOPT_RETURNTRANSFER => true,
    //     CURLOPT_USERAGENT => 'Codular Sample cURL Request'
    // ));
    // $res = curl_exec($ch);
    // // if(!$res){
    // //     die('Error: "' . curl_error($ch) . '" - Code: ' . curl_errno($ch));
    // // }
    // echo $res;

    // curl_close($ch);

}
else {
    echo "Not submitted";
}